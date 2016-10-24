/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyDelegateFloat.h"
#include "Core/PropertyFloat.h"
#include "Delegates/PropertyEditorAux.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorHandler.h"

#include "Utils/DoubleSpinBox.h"

#include <QLocale>

class QtnPropertyFloatSpinBoxHandler: public QtnPropertyEditorHandler<QtnPropertyFloatBase, QDoubleSpinBox>
{
public:
	QtnPropertyFloatSpinBoxHandler(QtnPropertyFloatBase& property, QDoubleSpinBox& editor);

protected:
	void updateEditor() override;

private:
	void onValueChanged(double value);

	unsigned updating;
};

static bool regFloatDelegate = QtnPropertyDelegateFactory::staticInstance()
								.registerDelegateDefault(&QtnPropertyFloatBase::staticMetaObject
								, &qtnCreateDelegate<QtnPropertyDelegateFloat, QtnPropertyFloatBase>
								, "SpinBox");

QtnPropertyDelegateFloat::QtnPropertyDelegateFloat(QtnPropertyFloatBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyFloatBase>(owner)
	, percent_suffix(false)
{
}

QWidget* QtnPropertyDelegateFloat::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
	auto spinBox = new QtnDoubleSpinBox(parent);
	spinBox->setHavePercentSuffix(percent_suffix);
	spinBox->setGeometry(rect);

	new QtnPropertyFloatSpinBoxHandler(owner(), *spinBox);

	spinBox->selectAll();

	if (owner().isEditableByUser())
		qtnInitNumEdit(spinBox, inplaceInfo, NUM_FLOAT);

	return spinBox;
}

bool QtnPropertyDelegateFloat::acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<QtnPropertyFloatBase>::acceptKeyPressedForInplaceEditImpl(keyEvent))
		return true;

	return qtnAcceptForNumEdit(keyEvent, NUM_FLOAT);
}

bool QtnPropertyDelegateFloat::propertyValueToStr(QString& strValue) const
{
	strValue = QLocale().toString(owner().value(), 'g', 6);
	return true;
}

QtnPropertyFloatSpinBoxHandler::QtnPropertyFloatSpinBoxHandler(QtnPropertyFloatBase &property, QDoubleSpinBox &editor)
	: QtnPropertyEditorHandlerType(property, editor)
	, updating(0)
{
	if (!property.isEditableByUser())
		editor.setReadOnly(true);

	editor.setRange(property.minValue(), property.maxValue());
	editor.setSingleStep(property.stepValue());
	editor.setDecimals(6);

	updateEditor();

	editor.setKeyboardTracking(false);
	editor.installEventFilter(this);
	QObject::connect(&editor,
					 static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
					 this,
					 &QtnPropertyFloatSpinBoxHandler::onValueChanged);
}

void QtnPropertyFloatSpinBoxHandler::updateEditor()
{
	updating++;

	if (property().valueIsHidden())
		editor().setValue(0);
	else
		editor().setValue(property());

	editor().selectAll();

	updating--;
}

void QtnPropertyFloatSpinBoxHandler::onValueChanged(double value)
{
	if (updating > 0)
		return;

	property().edit(static_cast<float>(value));
}
