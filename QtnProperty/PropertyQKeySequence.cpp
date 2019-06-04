/*******************************************************************************
Copyright (c) 2017-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyQKeySequence.h"

#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/Utils/PropertyEditorAux.h"
#include "Delegates/Utils/PropertyEditorHandler.h"

#include "Core/PropertyQString.h"
#include "MultiProperty.h"

#include <QKeySequenceEdit>
#include <QHBoxLayout>

class QtnPropertyQKeySequenceEditHandler
	: public QtnPropertyEditorHandler<QtnPropertyQKeySequenceBase,
		  QKeySequenceEdit>
{
public:
	QtnPropertyQKeySequenceEditHandler(
		QtnPropertyDelegate *delegate, QKeySequenceEdit &editor);

protected:
	virtual void updateEditor() override;
	void updateValue(const QKeySequence &seq);
};

QtnPropertyQKeySequenceBase::QtnPropertyQKeySequenceBase(QObject *parent)
	: QtnSinglePropertyBase(parent)
{
}

bool QtnPropertyQKeySequenceBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	return setValue(
		QKeySequence::fromString(str, QKeySequence::PortableText), reason);
}

bool QtnPropertyQKeySequenceBase::toStrImpl(QString &str) const
{
	str = value().toString(QKeySequence::PortableText);
	return true;
}

bool QtnPropertyQKeySequenceBase::fromVariantImpl(
	const QVariant &var, QtnPropertyChangeReason reason)
{
	QKeySequence keySequence;
	switch (var.type())
	{
		case QVariant::String:
			return fromStrImpl(var.toString(), reason);

		case QVariant::KeySequence:
			keySequence = var.value<QKeySequence>();
			break;

		default:
			break;
	}

	return setValue(keySequence, reason);
}

bool QtnPropertyQKeySequenceBase::toVariantImpl(QVariant &var) const
{
	var.setValue(value());
	return var.isValid();
}

QtnPropertyQKeySequenceCallback::QtnPropertyQKeySequenceCallback(
	QObject *parent)
	: QtnSinglePropertyCallback(parent)
{
}

QtnPropertyQKeySequence::QtnPropertyQKeySequence(QObject *parent)
	: QtnSinglePropertyValue(parent)
{
}

QtnPropertyDelegateQKeySequence::QtnPropertyDelegateQKeySequence(
	QtnPropertyQKeySequenceBase &owner)
	: QtnPropertyDelegateTyped(owner)
{
}

void QtnPropertyDelegateQKeySequence::Register(
	QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(
		&QtnPropertyQKeySequenceBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQKeySequence,
			QtnPropertyQKeySequenceBase>,
		"QKeySequence");
}

QWidget *QtnPropertyDelegateQKeySequence::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *)
{
	if (!stateProperty()->isEditableByUser())
	{
		auto lineEdit = new QLineEdit(parent);
		lineEdit->setGeometry(rect);
		lineEdit->setReadOnly(true);
		auto text = owner().value().toString(QKeySequence::NativeText);
		lineEdit->setText(text);
		lineEdit->setPlaceholderText(
			QtnPropertyQString::getEmptyPlaceholderStr());
		lineEdit->selectAll();
		return lineEdit;
	}

	auto editor = new QKeySequenceEdit(parent);
	editor->setGeometry(rect);
	new QtnPropertyQKeySequenceEditHandler(this, *editor);
	return editor;
}

bool QtnPropertyDelegateQKeySequence::propertyValueToStrImpl(
	QString &strValue) const
{
	strValue = owner().value().toString(QKeySequence::NativeText);

	if (strValue.isEmpty())
		strValue = QtnPropertyQString::getEmptyPlaceholderStr();

	return true;
}

QtnPropertyQKeySequenceEditHandler::QtnPropertyQKeySequenceEditHandler(
	QtnPropertyDelegate *delegate, QKeySequenceEdit &editor)
	: QtnPropertyEditorHandler(delegate, editor)
{
	updateEditor();

	QObject::connect(&editor, &QKeySequenceEdit::keySequenceChanged, this,
		&QtnPropertyQKeySequenceEditHandler::updateValue);
}

void QtnPropertyQKeySequenceEditHandler::updateEditor()
{
	if (stateProperty()->isMultiValue())
	{
		editor().clear();
	} else
	{
		editor().setKeySequence(property().value());
	}
}

void QtnPropertyQKeySequenceEditHandler::updateValue(const QKeySequence &seq)
{
	if (propertyBase())
		property().setValue(seq, delegate()->editReason());
}
