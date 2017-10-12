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

#include "PropertyDelegateInt.h"
#include "Core/PropertyInt.h"
#include "Delegates/PropertyEditorAux.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorHandler.h"

#include <QCoreApplication>
#include <QSpinBox>
#include <QLocale>

class QtnPropertyIntSpinBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyIntBase, QSpinBox>
{
public:
	QtnPropertyIntSpinBoxHandler(
		QtnPropertyIntBase &property, QSpinBox &editor);

protected:
	virtual void updateEditor() override;
};

static bool regIntDelegate = QtnPropertyDelegateFactory::staticInstance()
	.registerDelegateDefault(
		&QtnPropertyIntBase::staticMetaObject
		, &qtnCreateDelegate<QtnPropertyDelegateInt, QtnPropertyIntBase>
		, "SpinBox");

QtnPropertyDelegateInt::QtnPropertyDelegateInt(QtnPropertyIntBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyIntBase>(owner)
{
}

QWidget *QtnPropertyDelegateInt::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto spinBox = new QSpinBox(parent);
	spinBox->setGeometry(rect);

	new QtnPropertyIntSpinBoxHandler(owner(), *spinBox);

	spinBox->selectAll();

	if (owner().isEditableByUser())
		qtnInitNumEdit(spinBox, inplaceInfo, NUM_SIGNED_INT);

	return spinBox;
}

bool QtnPropertyDelegateInt::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<QtnPropertyIntBase>::
		acceptKeyPressedForInplaceEditImpl(keyEvent))
	{
		return true;
	}

	return qtnAcceptForNumEdit(keyEvent, NUM_SIGNED_INT);
}

bool QtnPropertyDelegateInt::propertyValueToStr(QString &strValue) const
{
	strValue = QLocale().toString(owner().value());
	return true;
}

QtnPropertyIntSpinBoxHandler::QtnPropertyIntSpinBoxHandler(
	QtnPropertyIntBase &property, QSpinBox &editor)
	: QtnPropertyEditorHandlerVT(property, editor)
{
	if (!property.isEditableByUser())
		editor.setReadOnly(true);

	editor.setRange(property.minValue(), property.maxValue());
	editor.setSingleStep(property.stepValue());

	updateEditor();

	editor.setKeyboardTracking(false);
	editor.installEventFilter(this);
	QObject::connect(
		&editor,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&QtnPropertyIntSpinBoxHandler::onValueChanged);
}

void QtnPropertyIntSpinBoxHandler::updateEditor()
{
	updating++;

	if (property().valueIsHidden())
		editor().setValue(0);
	else
		editor().setValue(property());

	editor().selectAll();

	updating--;
}
