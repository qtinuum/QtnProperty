/*
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

#include "PropertyInt64.h"

#include "QObjectPropertySet.h"

#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorAux.h"
#include "Delegates/PropertyEditorHandler.h"
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
		QtnPropertyInt64Base &property, QtnInt64SpinBox &editor);

private:
	virtual void updateEditor() override;
};

QtnPropertyInt64Base::QtnPropertyInt64Base(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<qint64>>(parent)
{
}

bool QtnPropertyInt64Base::fromStrImpl(const QString &str, bool edit)
{
	bool ok = false;
	ValueType value = str.toLongLong(&ok);

	if (!ok)
	{
		value = QLocale().toLongLong(str, &ok);

		if (!ok)
			return false;
	}

	return setValue(value, edit);
}

bool QtnPropertyInt64Base::toStrImpl(QString &str) const
{
	str = QString::number(value());
	return true;
}

bool QtnPropertyInt64Base::fromVariantImpl(const QVariant &var, bool edit)
{
	bool ok = false;
	ValueType value = var.toLongLong(&ok);

	if (!ok)
		return false;

	return setValue(value, edit);
}

QtnPropertyInt64::QtnPropertyInt64(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyInt64Base>(parent)
{
}

bool QtnPropertyInt64::Register()
{
	qtnRegisterMetaPropertyFactory(
		QVariant::LongLong, qtnCreateFactory<QtnPropertyInt64Callback>());

	return QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
		&QtnPropertyInt64Base::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateInt64, QtnPropertyInt64Base>,
		qtnSpinBoxDelegate());
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
	spinBox->setSuffix(suffix);
	spinBox->setGeometry(rect);

	new QtnPropertyInt64SpinBoxHandler(owner(), *spinBox);

	spinBox->selectAll();

	if (owner().isEditableByUser())
		qtnInitNumEdit(spinBox, inplaceInfo, NUM_SIGNED_INT);

	return spinBox;
}

void QtnPropertyDelegateInt64::applyAttributesImpl(
	const QtnPropertyDelegateAttributes &attributes)
{
	qtnGetAttribute(attributes, qtnSuffixAttr(), suffix);
}

bool QtnPropertyDelegateInt64::propertyValueToStr(QString &strValue) const
{
	auto value = owner().value();
	strValue = QLocale().toString(value);
	return true;
}

QtnPropertyInt64Callback::QtnPropertyInt64Callback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyInt64Base>(parent)
{
}

QtnPropertyInt64SpinBoxHandler::QtnPropertyInt64SpinBoxHandler(
	QtnPropertyInt64Base &property, QtnInt64SpinBox &editor)
	: QtnPropertyEditorHandlerVT(property, editor)
{
	if (!property.isEditableByUser())
		editor.setReadOnly(true);

	editor.setRange(property.minValue(), property.maxValue());
	editor.setSingleStep(property.stepValue());

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

	if (property().valueIsHidden())
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
