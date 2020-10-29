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

#include "PropertyDelegateFloat.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/Delegates/Utils/PropertyDelegateSliderBox.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Utils/DoubleSpinBox.h"
#include "QtnProperty/MultiProperty.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QCoreApplication>
#include <QLocale>

QByteArray qtnMultiplierAttr()
{
	return QByteArrayLiteral("multiplier");
}

QByteArray qtnPrecisionAttr()
{
	return QByteArrayLiteral("precision");
}

class QtnPropertyFloatSpinBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyFloatBase, QDoubleSpinBox>
{
public:
	QtnPropertyFloatSpinBoxHandler(
		QtnPropertyDelegateFloat *delegate, QDoubleSpinBox &editor);

protected:
	virtual void updateEditor() override;
	virtual void updateValue() override;

	void onDoubleValueChanged(double value);

private:
	QtnPropertyDelegateFloat *m_delegate;
};

QtnPropertyDelegateFloat::QtnPropertyDelegateFloat(QtnPropertyFloatBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyFloatBase>(owner)
	, m_multiplier(1.f)
	, m_precision(std::numeric_limits<float>::digits10)
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

float QtnPropertyDelegateFloat::stepValue() const
{
	return m_step.isValid() ? m_step.toFloat() : owner().stepValue();
}

QWidget *QtnPropertyDelegateFloat::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto spinBox = new QtnDoubleSpinBox(parent);
	spinBox->setDecimals(m_precision);
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
	strValue = QLocale().toString(currentValue(), 'g', m_precision);
	strValue.append(m_suffix);
	return true;
}

void QtnPropertyDelegateFloat::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
	info.loadAttribute(qtnMultiplierAttr(), m_multiplier);
	info.loadAttribute(qtnPrecisionAttr(), m_precision);

	m_step = info.attributes.value(qtnStepAttr());
	if (m_step.isValid())
	{
		bool ok;
		float step = m_step.toFloat(&ok);
		if (!ok)
		{
			m_step = QVariant();
		} else
		{
			m_step = step;
		}
	}

	m_precision = qBound(0, m_precision, std::numeric_limits<float>::digits10);

	m_min = info.attributes.value(qtnMinAttr());
	m_max = info.attributes.value(qtnMaxAttr());

	if (!qIsFinite(m_multiplier) || qFuzzyCompare(m_multiplier, 0.f))
	{
		m_multiplier = 1.f;
	}
	fixMinMaxVariant<float>(m_min, m_max);
}

float QtnPropertyDelegateFloat::minValue() const
{
	return (m_min.isValid() ? m_min.toFloat() : owner().minValue()) *
		m_multiplier;
}

float QtnPropertyDelegateFloat::maxValue() const
{
	return (m_max.isValid() ? m_max.toFloat() : owner().maxValue()) *
		m_multiplier;
}

float QtnPropertyDelegateFloat::multiplier() const
{
	return m_multiplier;
}

float QtnPropertyDelegateFloat::currentValue() const
{
	return qBound(minValue(), owner().value() * m_multiplier, maxValue());
}

QtnPropertyFloatSpinBoxHandler::QtnPropertyFloatSpinBoxHandler(
	QtnPropertyDelegateFloat *delegate, QDoubleSpinBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
	, m_delegate(delegate)
{
	updateEditor();

	editor.setKeyboardTracking(false);
	editor.installEventFilter(this);
	QObject::connect(&editor,
		static_cast<void (QDoubleSpinBox::*)(double)>(
			&QDoubleSpinBox::valueChanged),
		this, &QtnPropertyFloatSpinBoxHandler::onDoubleValueChanged);
}

void QtnPropertyFloatSpinBoxHandler::updateEditor()
{
	updating++;

	editor().setReadOnly(!stateProperty()->isEditableByUser());
	editor().setSingleStep(m_delegate->stepValue());
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

void QtnPropertyFloatSpinBoxHandler::updateValue()
{
	if (this->propertyBase())
	{
		this->property().setValue(newValue / m_delegate->multiplier(),
			this->delegate()->editReason());
	}
}

void QtnPropertyFloatSpinBoxHandler::onDoubleValueChanged(double value)
{
	onValueChanged(float(value));
}
