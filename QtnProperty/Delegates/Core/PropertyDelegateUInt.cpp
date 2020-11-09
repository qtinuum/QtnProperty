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

#include "PropertyDelegateUInt.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/Delegates/Utils/PropertyDelegateSliderBox.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Utils/QtnInt64SpinBox.h"
#include "QtnProperty/MultiProperty.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QCoreApplication>
#include <QLocale>

class QtnPropertyUIntSpinBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyUIntBase, QtnInt64SpinBox>
{
public:
	QtnPropertyUIntSpinBoxHandler(
		QtnPropertyDelegateUInt *delegate, QtnInt64SpinBox &editor);

protected:
	virtual void updateEditor() override;
	void onValueChanged(qint64 value);

private:
	QtnPropertyDelegateUInt *m_delegate;
};

QtnPropertyDelegateUInt::QtnPropertyDelegateUInt(QtnPropertyUIntBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyUIntBase>(owner)
{
}

void QtnPropertyDelegateUInt::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyUIntBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateUInt, QtnPropertyUIntBase>,
		qtnSpinBoxDelegate());

	factory.registerDelegate(&QtnPropertyUIntBase::staticMetaObject,
		&qtnCreateDelegate<
			QtnPropertyDelegateSlideBoxTyped<QtnPropertyUIntBase>,
			QtnPropertyUIntBase>,
		qtnSliderBoxDelegate());
}

uint QtnPropertyDelegateUInt::stepValue() const
{
	return m_step.isValid() ? m_step.toUInt() : owner().stepValue();
}

uint QtnPropertyDelegateUInt::minValue() const
{
	return m_min.isValid() ? m_min.toUInt() : owner().minValue();
}

uint QtnPropertyDelegateUInt::maxValue() const
{
	return m_max.isValid() ? m_max.toUInt() : owner().maxValue();
}

uint QtnPropertyDelegateUInt::currentValue() const
{
	return qBound(minValue(), owner().value(), maxValue());
}

QWidget *QtnPropertyDelegateUInt::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto spinBox = new QtnInt64SpinBox(parent);
	spinBox->setSuffix(m_suffix);
	spinBox->setGeometry(rect);

	new QtnPropertyUIntSpinBoxHandler(this, *spinBox);

	spinBox->selectAll();

	if (stateProperty()->isEditableByUser())
		qtnInitNumEdit(spinBox, inplaceInfo, NUM_UNSIGNED_INT);

	return spinBox;
}

bool QtnPropertyDelegateUInt::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<
			QtnPropertyUIntBase>::acceptKeyPressedForInplaceEditImpl(keyEvent))
		return true;

	return qtnAcceptForNumEdit(keyEvent, NUM_UNSIGNED_INT);
}

void QtnPropertyDelegateUInt::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
	m_min = info.attributes.value(qtnMinAttr());
	m_max = info.attributes.value(qtnMaxAttr());
	m_step = info.attributes.value(qtnStepAttr());
	if (m_step.isValid())
	{
		bool ok;
		uint step = m_step.toUInt(&ok);
		if (!ok)
		{
			m_step = QVariant();
		} else
		{
			m_step = step;
		}
	}
	fixMinMaxVariant<uint>(m_min, m_max);
}

bool QtnPropertyDelegateUInt::propertyValueToStrImpl(QString &strValue) const
{
	strValue = QLocale().toString(currentValue());
	strValue.append(m_suffix);
	return true;
}

QtnPropertyUIntSpinBoxHandler::QtnPropertyUIntSpinBoxHandler(
	QtnPropertyDelegateUInt *delegate, QtnInt64SpinBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
	, m_delegate(delegate)
{
	updateEditor();

	editor.setKeyboardTracking(false);
	editor.installEventFilter(this);
	QObject::connect(&editor,
		static_cast<void (QtnInt64SpinBox::*)(qint64)>(
			&QtnInt64SpinBox::valueChanged),
		this, &QtnPropertyUIntSpinBoxHandler::onValueChanged);
}

void QtnPropertyUIntSpinBoxHandler::updateEditor()
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

void QtnPropertyUIntSpinBoxHandler::onValueChanged(qint64 value)
{
	QtnPropertyEditorHandlerVT::onValueChanged(ValueTypeStore(value));
}
