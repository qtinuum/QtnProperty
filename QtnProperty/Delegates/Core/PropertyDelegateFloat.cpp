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

#include "PropertyDelegateFloat.h"
#include "Delegates/Utils/PropertyEditorAux.h"
#include "Delegates/Utils/PropertyEditorHandler.h"
#include "Delegates/Utils/PropertyDelegateSliderBox.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Utils/DoubleSpinBox.h"
#include "MultiProperty.h"
#include "PropertyDelegateAttrs.h"

#include <QCoreApplication>
#include <QLocale>

class QtnPropertyFloatSpinBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyFloatBase, QDoubleSpinBox>
{
public:
	QtnPropertyFloatSpinBoxHandler(
		QtnPropertyDelegate *delegate, QDoubleSpinBox &editor);

protected:
	virtual void updateEditor() override;
	void onValueChanged(double value);
};

QtnPropertyDelegateFloat::QtnPropertyDelegateFloat(QtnPropertyFloatBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyFloatBase>(owner)
{
}

void QtnPropertyDelegateFloat::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyFloatBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateFloat, QtnPropertyFloatBase>,
		qtnSpinBoxDelegate());

	factory.registerDelegate(&QtnPropertyFloatBase::staticMetaObject,
		&qtnCreateDelegate<
			QtnPropertyDelegateSlideBoxTyped<QtnPropertyFloatBase>,
			QtnPropertyFloatBase>,
		qtnSliderBoxDelegate());
}

QWidget *QtnPropertyDelegateFloat::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto spinBox = new QtnDoubleSpinBox(parent);
	spinBox->setDecimals(6);
	spinBox->setSuffix(m_suffix);
	spinBox->setGeometry(rect);

	new QtnPropertyFloatSpinBoxHandler(this, *spinBox);

	spinBox->selectAll();

	if (stateProperty()->isEditableByUser())
		qtnInitNumEdit(spinBox, inplaceInfo, NUM_FLOAT);

	return spinBox;
}

bool QtnPropertyDelegateFloat::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<
			QtnPropertyFloatBase>::acceptKeyPressedForInplaceEditImpl(keyEvent))
	{
		return true;
	}

	return qtnAcceptForNumEdit(keyEvent, NUM_FLOAT);
}

bool QtnPropertyDelegateFloat::propertyValueToStrImpl(QString &strValue) const
{
	QLocale locale;
	strValue = locale.toString(owner().value(), 'g', 6);
	strValue.append(m_suffix);
	return true;
}

void QtnPropertyDelegateFloat::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
}

QtnPropertyFloatSpinBoxHandler::QtnPropertyFloatSpinBoxHandler(
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
		this, &QtnPropertyFloatSpinBoxHandler::onValueChanged);
}

void QtnPropertyFloatSpinBoxHandler::updateEditor()
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

void QtnPropertyFloatSpinBoxHandler::onValueChanged(double value)
{
	QtnPropertyEditorHandlerVT::onValueChanged(ValueTypeStore(value));
}
