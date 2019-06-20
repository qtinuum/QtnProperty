/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include "PropertyDelegateDouble.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/Delegates/Utils/PropertyDelegateSliderBox.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Utils/DoubleSpinBox.h"
#include "QtnProperty/MultiProperty.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QCoreApplication>
#include <QLocale>

class QtnPropertyDoubleSpinBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyDoubleBase, QDoubleSpinBox>
{
public:
	QtnPropertyDoubleSpinBoxHandler(
		QtnPropertyDelegate *delegate, QDoubleSpinBox &editor);

protected:
	virtual void updateEditor() override;
};

QtnPropertyDelegateDouble::QtnPropertyDelegateDouble(
	QtnPropertyDoubleBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyDoubleBase>(owner)
{
}

void QtnPropertyDelegateDouble::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyDoubleBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateDouble, QtnPropertyDoubleBase>,
		qtnSpinBoxDelegate());

	factory.registerDelegate(&QtnPropertyDoubleBase::staticMetaObject,
		&qtnCreateDelegate<
			QtnPropertyDelegateSlideBoxTyped<QtnPropertyDoubleBase>,
			QtnPropertyDoubleBase>,
		qtnSliderBoxDelegate());
}

QWidget *QtnPropertyDelegateDouble::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto spinBox = new QtnDoubleSpinBox(parent);
	spinBox->setDecimals(15);
	spinBox->setSuffix(m_suffix);
	spinBox->setGeometry(rect);

	new QtnPropertyDoubleSpinBoxHandler(this, *spinBox);

	spinBox->selectAll();

	if (stateProperty()->isEditableByUser())
		qtnInitNumEdit(spinBox, inplaceInfo, NUM_FLOAT);

	return spinBox;
}

bool QtnPropertyDelegateDouble::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<QtnPropertyDoubleBase>::
			acceptKeyPressedForInplaceEditImpl(keyEvent))
	{
		return true;
	}

	return qtnAcceptForNumEdit(keyEvent, NUM_FLOAT);
}

bool QtnPropertyDelegateDouble::propertyValueToStrImpl(QString &strValue) const
{
	QLocale locale;
	strValue = locale.toString(owner().value(), 'g', 15);
	strValue.append(m_suffix);
	return true;
}

void QtnPropertyDelegateDouble::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
}

QtnPropertyDoubleSpinBoxHandler::QtnPropertyDoubleSpinBoxHandler(
	QtnPropertyDelegate *delegate, QDoubleSpinBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
{
	if (!stateProperty()->isEditableByUser())
		editor.setReadOnly(true);

	auto &p = property();
	editor.setRange(p.minValue(), p.maxValue());
	editor.setSingleStep(p.stepValue());

	updateEditor();

	editor.setKeyboardTracking(false);
	editor.installEventFilter(this);
	QObject::connect(&editor,
		static_cast<void (QDoubleSpinBox::*)(double)>(
			&QDoubleSpinBox::valueChanged),
		this, &QtnPropertyDoubleSpinBoxHandler::onValueChanged);
}

void QtnPropertyDoubleSpinBoxHandler::updateEditor()
{
	updating++;

	if (stateProperty()->isMultiValue())
	{
		editor().setValue(editor().minimum());
		editor().setSpecialValueText(
			QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		editor().setValue(property().value());
		editor().setSpecialValueText(QString());
	}

	editor().selectAll();

	updating--;
}
