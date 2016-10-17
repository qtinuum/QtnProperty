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
#include "Utils/UnsignedSpinBox.h"

#include <QSpinBox>
#include <QLineEdit>
#include <QLocale>

class QtnPropertyUIntSpinBoxHandler: public QtnPropertyEditorHandler<QtnPropertyUIntBase, QtnUnsignedSpinBox>
{
public:
	QtnPropertyUIntSpinBoxHandler(QtnPropertyUIntBase &property,
								  QtnUnsignedSpinBox &editor);

private:
	virtual void updateEditor() override;

	void onTextEdited(const QString &text);
	void onValueChanged(int value);
	void onEditingFinished();

	unsigned block;
};

static bool regUIntDelegate = QtnPropertyDelegateFactory::staticInstance()
								.registerDelegateDefault(&QtnPropertyUIntBase::staticMetaObject
								, &qtnCreateDelegate<QtnPropertyDelegateUInt, QtnPropertyUIntBase>
								, "SpinBox");

QWidget* QtnPropertyDelegateUInt::createValueEditorImpl(QWidget *parent, const QRect& rect, QtnInplaceInfo *inplaceInfo)
{
	auto spinBox = new QtnUnsignedSpinBox(parent);
	spinBox->setGeometry(rect);

	new QtnPropertyUIntSpinBoxHandler(owner(), *spinBox);

	spinBox->selectAll();

	if (owner().isEditableByUser())
		qtnInitNumEdit(spinBox, inplaceInfo, NUM_UNSIGNED_INT);

	return spinBox;
}

bool QtnPropertyDelegateUInt::acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<QtnPropertyUIntBase>::acceptKeyPressedForInplaceEditImpl(keyEvent))
		return true;

	return qtnAcceptForNumEdit(keyEvent, NUM_UNSIGNED_INT);
}

bool QtnPropertyDelegateUInt::propertyValueToStr(QString& strValue) const
{
	strValue = QLocale().toString(owner().value());
	return true;
}

QtnPropertyUIntSpinBoxHandler::QtnPropertyUIntSpinBoxHandler(QtnPropertyUIntBase &property, QtnUnsignedSpinBox &editor)
	: QtnPropertyEditorHandlerType(property, editor)
	, block(0)
{
	if (!property.isEditableByUser())
		editor.setReadOnly(true);

	editor.setRange(QtnUnsignedSpinBox::qtn_u2i(property.minValue()),
					QtnUnsignedSpinBox::qtn_u2i(property.maxValue()));
	editor.setSingleStep(QtnUnsignedSpinBox::qtn_u2i(property.stepValue()));

	updateEditor();

	editor.setKeyboardTracking(false);
	editor.installEventFilter(this);
	QObject::connect(&editor,
					 static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
					 this,
					 &QtnPropertyUIntSpinBoxHandler::onValueChanged);
	QObject::connect(editor.lineEdit(),
					 &QLineEdit::textEdited,
					 this,
					 &QtnPropertyUIntSpinBoxHandler::onTextEdited);
	QObject::connect(&editor,
					 &QSpinBox::editingFinished,
					 this,
					 &QtnPropertyUIntSpinBoxHandler::onEditingFinished);
}

void QtnPropertyUIntSpinBoxHandler::updateEditor()
{
	block++;

	if (property().valueIsHidden())
		editor().setValue(0);
	else
		editor().setValue(QtnUnsignedSpinBox::qtn_u2i(property().value()));

	block--;
}

void QtnPropertyUIntSpinBoxHandler::onTextEdited(const QString &text)
{
	bool ok = false;
	auto value = text.toUInt(&ok);

	if (ok)
	{
		block++;
		editor().setValue(QtnUnsignedSpinBox::qtn_u2i(value));
		block--;
	}
}

void QtnPropertyUIntSpinBoxHandler::onValueChanged(int value)
{
	if (block > 0)
		return;

	property().edit(QtnUnsignedSpinBox::qtn_i2u(value));
}

void QtnPropertyUIntSpinBoxHandler::onEditingFinished()
{
	if (canApply())
		property().edit(QtnUnsignedSpinBox::qtn_i2u(editor().value()));

	applyReset();
}
