/*******************************************************************************
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

#include "PropertyInt64.h"

#include "QObjectPropertySet.h"

#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/Utils/PropertyEditorAux.h"
#include "Delegates/Utils/PropertyEditorHandler.h"
#include "Delegates/Utils/PropertyDelegateSliderBox.h"
#include "Utils/QtnInt64SpinBox.h"
#include "MultiProperty.h"
#include "PropertyDelegateAttrs.h"

#include <QLocale>
#include <QKeyEvent>

class QtnPropertyInt64SpinBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyInt64Base, QtnInt64SpinBox>
{
public:
	QtnPropertyInt64SpinBoxHandler(
		QtnPropertyDelegateInt64 *delegate, QtnInt64SpinBox &editor);

private:
	virtual void updateEditor() override;

private:
	QtnPropertyDelegateInt64 *m_delegate;
};

QtnPropertyInt64Base::QtnPropertyInt64Base(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<qint64>>(parent)
{
}

bool QtnPropertyInt64Base::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	bool ok = false;
	ValueType value = str.toLongLong(&ok);

	if (!ok)
		return false;

	return setValue(value, reason);
}

bool QtnPropertyInt64Base::toStrImpl(QString &str) const
{
	str = QString::number(value());
	return true;
}

bool QtnPropertyInt64Base::fromVariantImpl(
	const QVariant &var, QtnPropertyChangeReason reason)
{
	bool ok = false;
	ValueType value = var.toLongLong(&ok);

	if (!ok)
		return false;

	return setValue(value, reason);
}

QtnPropertyInt64::QtnPropertyInt64(QObject *parent)
	: QtnNumericPropertyValue<QtnPropertyInt64Base>(parent)
{
}

void QtnPropertyDelegateInt64::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyInt64Base::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateInt64, QtnPropertyInt64Base>,
		qtnSpinBoxDelegate());

	factory.registerDelegate(&QtnPropertyInt64Base::staticMetaObject,
		&qtnCreateDelegate<
			QtnPropertyDelegateSlideBoxTyped<QtnPropertyInt64Base>,
			QtnPropertyInt64Base>,
		qtnSliderBoxDelegate());
}

qint64 QtnPropertyDelegateInt64::stepValue() const
{
	return m_step.isValid() ? m_step.toLongLong() : owner().stepValue();
}

qint64 QtnPropertyDelegateInt64::minValue() const
{
	return m_min.isValid() ? m_min.toLongLong() : owner().minValue();
}

qint64 QtnPropertyDelegateInt64::maxValue() const
{
	return m_max.isValid() ? m_max.toLongLong() : owner().maxValue();
}

qint64 QtnPropertyDelegateInt64::currentValue() const
{
	return qBound(minValue(), owner().value(), maxValue());
}

QtnPropertyDelegateInt64::QtnPropertyDelegateInt64(QtnPropertyInt64Base &owner)
	: QtnPropertyDelegateTyped<QtnPropertyInt64Base>(owner)
{
}

bool QtnPropertyDelegateInt64::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<
			QtnPropertyInt64Base>::acceptKeyPressedForInplaceEditImpl(keyEvent))
		return true;

	return qtnAcceptForNumEdit(keyEvent, NUM_SIGNED_INT);
}

QWidget *QtnPropertyDelegateInt64::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto spinBox = new QtnInt64SpinBox(parent);
	spinBox->setSuffix(m_suffix);
	spinBox->setGeometry(rect);

	new QtnPropertyInt64SpinBoxHandler(this, *spinBox);

	spinBox->selectAll();

	if (stateProperty()->isEditableByUser())
		qtnInitNumEdit(spinBox, inplaceInfo, NUM_SIGNED_INT);

	return spinBox;
}

void QtnPropertyDelegateInt64::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
	m_min = info.attributes.value(qtnMinAttr());
	m_max = info.attributes.value(qtnMaxAttr());
	m_step = info.attributes.value(qtnStepAttr());
	if (m_step.isValid())
	{
		bool ok;
		qint64 step = m_step.toLongLong(&ok);
		if (!ok)
		{
			m_step = QVariant();
		} else
		{
			m_step = step;
		}
	}
	fixMinMaxVariant<qint64>(m_min, m_max);
}

bool QtnPropertyDelegateInt64::propertyValueToStrImpl(QString &strValue) const
{
	strValue = QLocale().toString(currentValue());
	strValue.append(m_suffix);
	return true;
}

QtnPropertyInt64Callback::QtnPropertyInt64Callback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyInt64Base>(parent)
{
}

QtnPropertyInt64SpinBoxHandler::QtnPropertyInt64SpinBoxHandler(
	QtnPropertyDelegateInt64 *delegate, QtnInt64SpinBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
	, m_delegate(delegate)
{
	updateEditor();

	editor.setKeyboardTracking(false);
	editor.installEventFilter(this);
	QObject::connect(&editor,
		static_cast<void (QtnInt64SpinBox::*)(qint64)>(
			&QtnInt64SpinBox::valueChanged),
		this, &QtnPropertyInt64SpinBoxHandler::onValueChanged);
}

void QtnPropertyInt64SpinBoxHandler::updateEditor()
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
