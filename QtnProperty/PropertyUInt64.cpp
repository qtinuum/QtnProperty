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

#include "PropertyUInt64.h"

#include "QObjectPropertySet.h"
#include "Delegates/Utils/PropertyEditorAux.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/Utils/PropertyDelegateSliderBox.h"
#include "PropertyDelegateAttrs.h"

#include <QLocale>
#include <QKeyEvent>
#include <QLineEdit>

QtnPropertyUInt64Base::QtnPropertyUInt64Base(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<quint64>>(parent)
{
}

bool QtnPropertyUInt64Base::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	bool ok = false;
	ValueType value = str.toULongLong(&ok);

	if (!ok)
		return false;

	return setValue(value, reason);
}

bool QtnPropertyUInt64Base::toStrImpl(QString &str) const
{
	str = QString::number(value());
	return true;
}

bool QtnPropertyUInt64Base::fromVariantImpl(
	const QVariant &var, QtnPropertyChangeReason reason)
{
	bool ok = false;
	ValueType value = var.toULongLong(&ok);

	if (!ok)
		return false;

	return setValue(value, reason);
}

QtnPropertyUInt64Callback::QtnPropertyUInt64Callback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyUInt64Base>(parent)
{
}

QtnPropertyUInt64::QtnPropertyUInt64(QObject *parent)
	: QtnNumericPropertyValue<QtnPropertyUInt64Base>(parent)
{
}

void QtnPropertyDelegateUInt64::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyUInt64Base::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateUInt64, QtnPropertyUInt64Base>,
		qtnLineEditDelegate());

	factory.registerDelegate(&QtnPropertyUInt64Base::staticMetaObject,
		&qtnCreateDelegate<
			QtnPropertyDelegateSlideBoxTyped<QtnPropertyUInt64Base>,
			QtnPropertyUInt64Base>,
		qtnSliderBoxDelegate());
}

quint64 QtnPropertyDelegateUInt64::minValue() const
{
	return m_min.isValid() ? m_min.toULongLong() : owner().minValue();
}

quint64 QtnPropertyDelegateUInt64::maxValue() const
{
	return m_max.isValid() ? m_max.toULongLong() : owner().maxValue();
}

quint64 QtnPropertyDelegateUInt64::currentValue() const
{
	return qBound(minValue(), owner().value(), maxValue());
}

QtnPropertyDelegateUInt64::QtnPropertyDelegateUInt64(
	QtnPropertyUInt64Base &owner)
	: QObject(nullptr)
	, QtnPropertyDelegateTyped<QtnPropertyUInt64Base>(owner)
	, editor(nullptr)
	, reverted(false)
	, applied(false)
{
}

bool QtnPropertyDelegateUInt64::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		auto keyEvent = static_cast<QKeyEvent *>(event);

		switch (keyEvent->key())
		{
			case Qt::Key_Escape:
				reverted = true;
				updateEditor();
				break;

			case Qt::Key_Enter:
			case Qt::Key_Return:
				applied = true;
				break;

			default:
				break;
		}
	}

	return QObject::eventFilter(obj, event);
}

bool QtnPropertyDelegateUInt64::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<QtnPropertyUInt64Base>::
			acceptKeyPressedForInplaceEditImpl(keyEvent))
		return true;

	return qtnAcceptForNumEdit(keyEvent, NUM_UNSIGNED_INT);
}

QWidget *QtnPropertyDelegateUInt64::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	editor = createValueEditorLineEdit(
		parent, rect, !stateProperty()->isEditableByUser(), inplaceInfo);

	updateEditor();
	reverted = false;
	applied = false;

	editor->installEventFilter(this);
	QObject::connect(editor, &QLineEdit::editingFinished, this,
		&QtnPropertyDelegateUInt64::onEditingFinished);

	return editor;
}

bool QtnPropertyDelegateUInt64::propertyValueToStrImpl(QString &strValue) const
{
	strValue = QLocale().toString(currentValue());
	strValue.append(m_suffix);
	return true;
}

void QtnPropertyDelegateUInt64::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
	m_min = info.attributes.value(qtnMinAttr());
	m_max = info.attributes.value(qtnMaxAttr());
	fixMinMaxVariant<quint64>(m_min, m_max);
}

void QtnPropertyDelegateUInt64::onEditingFinished()
{
	bool ok = false;

	if (!reverted && (applied || !stateProperty()->isMultiValue()))
	{
		auto str = editor->text().trimmed();
		if (!m_suffix.isEmpty() && str.endsWith(m_suffix))
		{
			str = str.left(str.length() - m_suffix.length()).trimmed();
		}
		QLocale locale;
		str.remove(locale.groupSeparator());
		auto value = locale.toULongLong(str, &ok);
		if (!ok)
		{
			value = str.toULongLong(&ok);
		}
		ok = ok && value >= minValue() && value <= maxValue();

		if (ok)
			owner().setValue(value, editReason());
	}

	if (!ok)
		updateEditor();

	reverted = false;
	applied = false;
}

void QtnPropertyDelegateUInt64::updateEditor()
{
	if (stateProperty()->isMultiValue())
	{
		editor->clear();
	} else
	{
		QString str;
		propertyValueToStrImpl(str);
		str.remove(QLocale().groupSeparator());
		editor->setText(str);

		editor->selectAll();
	}
}
