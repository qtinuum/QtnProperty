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

#include "PropertyDelegateUInt.h"
#include "Core/PropertyUInt.h"
#include "Delegates/PropertyEditorAux.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorHandler.h"
#include "Utils/QtnInt64SpinBox.h"
#include "MultiProperty.h"
#include "PropertyDelegateAttrs.h"

#include <QSpinBox>
#include <QLineEdit>
#include <QLocale>

class QtnPropertyUIntSpinBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyUIntBase, QtnInt64SpinBox>
{
public:
	QtnPropertyUIntSpinBoxHandler(
		QtnPropertyUIntBase &property, QtnInt64SpinBox &editor);

private:
	virtual void updateEditor() override;
	void onValueChanged(qint64 value);
};

QtnPropertyDelegateUInt::QtnPropertyDelegateUInt(QtnPropertyUIntBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyUIntBase>(owner)
{
}

bool QtnPropertyDelegateUInt::Register()
{
	return QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
		&QtnPropertyUIntBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateUInt, QtnPropertyUIntBase>,
		qtnSpinBoxDelegate());
}

QWidget *QtnPropertyDelegateUInt::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto spinBox = new QtnInt64SpinBox(parent);
	spinBox->setSuffix(suffix);
	spinBox->setGeometry(rect);

	new QtnPropertyUIntSpinBoxHandler(owner(), *spinBox);

	spinBox->selectAll();

	if (owner().isEditableByUser())
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
	const QtnPropertyDelegateAttributes &attributes)
{
	qtnGetAttribute(attributes, qtnSuffixAttr(), suffix);
}

bool QtnPropertyDelegateUInt::propertyValueToStr(QString &strValue) const
{
	strValue = QLocale().toString(owner().value());
	return true;
}

QtnPropertyUIntSpinBoxHandler::QtnPropertyUIntSpinBoxHandler(
	QtnPropertyUIntBase &property, QtnInt64SpinBox &editor)
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
		this, &QtnPropertyUIntSpinBoxHandler::onValueChanged);
}

void QtnPropertyUIntSpinBoxHandler::updateEditor()
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

void QtnPropertyUIntSpinBoxHandler::onValueChanged(qint64 value)
{
	QtnPropertyEditorHandlerVT::onValueChanged(ValueTypeStore(value));
}
