/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "PropertyDelegateDouble.h"
#include "../../../Core/Core/PropertyDouble.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include "Utils/DoubleSpinBox.h"

#include <QLocale>

class QtnPropertyDoubleSpinBoxHandler
	: public QtnPropertyEditorHandler<QtnPropertyDoubleBase, QDoubleSpinBox>
{
public:
	QtnPropertyDoubleSpinBoxHandler(QtnPropertyDoubleBase &property,
									QDoubleSpinBox &editor);

protected:
	virtual void updateEditor() override;

private:
	void onValueChanged(double value);

	unsigned updating;
};

static bool regDoubleDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyDoubleBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateDouble, QtnPropertyDoubleBase>
                                , "SpinBox");

QtnPropertyDelegateDouble::QtnPropertyDelegateDouble(QtnPropertyDoubleBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyDoubleBase>(owner)
	, percent_suffix(false)
{
}

QWidget* QtnPropertyDelegateDouble::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
	auto spinBox = new QtnDoubleSpinBox(parent);
	spinBox->setHavePercentSuffix(percent_suffix);
    spinBox->setGeometry(rect);

    new QtnPropertyDoubleSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool QtnPropertyDelegateDouble::propertyValueToStr(QString& strValue) const
{
	QLocale locale;
	strValue = locale.toString(owner().value(), 'g', 10);
	if (percent_suffix)
		strValue.append(locale.percent());
	return true;
}

QtnPropertyDoubleSpinBoxHandler::QtnPropertyDoubleSpinBoxHandler(
		QtnPropertyDoubleBase &property, QDoubleSpinBox &editor)
	: QtnPropertyEditorHandlerType(property, editor)
	, updating(0)
{
	if (!property.isEditableByUser())
		editor.setReadOnly(true);

	editor.setRange(property.minValue(), property.maxValue());
	editor.setSingleStep(property.stepValue());
	editor.setDecimals(12);

	updateEditor();

	editor.setKeyboardTracking(false);
	editor.installEventFilter(this);
	QObject::connect(&editor,
					 static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
					 this,
					 &QtnPropertyDoubleSpinBoxHandler::onValueChanged);
}

void QtnPropertyDoubleSpinBoxHandler::updateEditor()
{
	updating++;

	if (property().valueIsHidden())
		editor().setValue(0);
	else
		editor().setValue(property());

	updating--;
}

void QtnPropertyDoubleSpinBoxHandler::onValueChanged(double value)
{
	if (updating > 0)
		return;

	property().edit(value);
}
