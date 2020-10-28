/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
		QtnPropertyDelegateDouble *delegate, QDoubleSpinBox &editor);

protected:
	virtual void updateEditor() override;
	virtual void updateValue() override;

private:
	QtnPropertyDelegateDouble *m_delegate;
};

QtnPropertyDelegateDouble::QtnPropertyDelegateDouble(
	QtnPropertyDoubleBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyDoubleBase>(owner)
	, m_multiplier(1.0)
	, m_min(qInf())
	, m_max(qInf())
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
	spinBox->setDecimals(5);
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
	strValue = locale.toString(currentValue(), 'g', 5);
	strValue.append(m_suffix);
	return true;
}

void QtnPropertyDelegateDouble::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
	info.loadAttribute(qtnMultiplierAttr(), m_multiplier);
	m_min = info.attributes.value(qtnMinAttr());
	m_max = info.attributes.value(qtnMinAttr());

	if (!qIsFinite(m_multiplier) || qFuzzyCompare(m_multiplier, 0.0))
	{
		m_multiplier = 1;
	}
	fixMinMaxVariant<double>(m_min, m_max);
}

double QtnPropertyDelegateDouble::minValue() const
{
	return (qIsFinite(m_min) ? m_min : owner().minValue()) * m_multiplier;
}

double QtnPropertyDelegateDouble::maxValue() const
{
	return (qIsFinite(m_max) ? m_max : owner().maxValue()) * m_multiplier;
}

double QtnPropertyDelegateDouble::multiplier() const
{
	return m_multiplier;
}

double QtnPropertyDelegateDouble::currentValue() const
{
	return std::min(
		maxValue(), std::max(minValue(), owner().value() * m_multiplier));
}

QtnPropertyDoubleSpinBoxHandler::QtnPropertyDoubleSpinBoxHandler(
	QtnPropertyDelegateDouble *delegate, QDoubleSpinBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
	, m_delegate(delegate)
{
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

	editor().setSingleStep(property().stepValue());
	editor().setReadOnly(!stateProperty()->isEditableByUser());
	editor().setRange(m_delegate->minValue(), m_delegate->maxValue());

	if (stateProperty()->isMultiValue())
	{
		editor().setValue(editor().minimum());
		editor().setSpecialValueText(
			QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		editor().setValue(m_delegate->currentValue());
		editor().setSpecialValueText(QString());
	}

	editor().selectAll();

	updating--;
}

void QtnPropertyDoubleSpinBoxHandler::updateValue()
{
	if (this->propertyBase())
	{
		this->property().setValue(newValue / m_delegate->multiplier(),
			this->delegate()->editReason());
	}
}
