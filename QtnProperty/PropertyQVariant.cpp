/*******************************************************************************
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyQVariant.h"

#include "QObjectPropertySet.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/Utils/PropertyEditorAux.h"
#include "Delegates/Utils/PropertyEditorHandler.h"
#include "MultiProperty.h"
#include "Core/PropertyQString.h"

#include "CustomPropertyEditorDialog.h"

#include <QEvent>
#include <QKeyEvent>

class QtnPropertyQVariantEditBttnHandler
	: public QtnPropertyEditorBttnHandler<QtnPropertyQVariantBase,
		  QtnLineEditBttn>
{
public:
	QtnPropertyQVariantEditBttnHandler(
		QtnPropertyDelegate *delegate, QtnLineEditBttn &editor);

protected:
	virtual void revertInput() override;
	virtual void onToolButtonClick() override;
	virtual void updateEditor() override;

private:
	void onEditingFinished();
	void onToolButtonClicked(bool);
	void onApplyData(const QVariant &data);

	CustomPropertyEditorDialog *dialog;
	DialogContainerPtr dialogContainer;
	bool is_object;
};

QtnPropertyQVariantBase::QtnPropertyQVariantBase(QObject *parent)
	: QtnSinglePropertyBase(parent)
{
}

bool QtnPropertyQVariantBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	return setValue(str, reason);
}

bool QtnPropertyQVariantBase::toStrImpl(QString &str) const
{
	str = value().toString();
	return true;
}

bool QtnPropertyQVariantBase::fromVariantImpl(
	const QVariant &var, QtnPropertyChangeReason reason)
{
	return setValue(var, reason);
}

bool QtnPropertyQVariantBase::toVariantImpl(QVariant &var) const
{
	var = value();
	return var.isValid();
}

QtnPropertyQVariantCallback::QtnPropertyQVariantCallback(
	QObject *object, const QMetaProperty &metaProperty, QObject *parent)
	: QtnSinglePropertyCallback(parent)
{
	setCallbackValueGet([object, metaProperty]() -> QVariant {
		return metaProperty.read(object);
	});

	setCallbackValueSet([object, metaProperty](QVariant value, QtnPropertyChangeReason /*reason*/) {
		metaProperty.write(object, value);
	});

	setCallbackValueEqual([object, metaProperty](QVariant value) -> bool {
		auto thisValue = metaProperty.read(object);

		return thisValue.userType() == value.userType() && thisValue == value;
	});
}

QtnPropertyQVariantCallback::QtnPropertyQVariantCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQVariantBase>(parent)
{
}

QtnPropertyQVariant::QtnPropertyQVariant(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQVariantBase>(parent)
{
}

QString QtnPropertyQVariant::valueToString(const QVariant &value)
{
	return !variantIsObject(value.type()) ? value.toString() : QString();
}

bool QtnPropertyQVariant::variantIsObject(QVariant::Type type)
{
	switch (type)
	{
		case QVariant::Map:
		case QVariant::List:
			return true;

		default:
			break;
	}

	return false;
}

QString QtnPropertyQVariant::getPlaceholderStr(QVariant::Type type)
{
	switch (type)
	{
		case QVariant::Map:
			return tr("(Dictionary)");

		case QVariant::List:
			return tr("(List)");

		default:
			break;
	}

	return QtnPropertyQString::getEmptyPlaceholderStr();
}

QtnPropertyDelegateQVariant::QtnPropertyDelegateQVariant(
	QtnPropertyQVariantBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyQVariantBase>(owner)
{
}

void QtnPropertyDelegateQVariant::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQVariantBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQVariant,
			QtnPropertyQVariantBase>,
		"QVariant");
}

bool QtnPropertyDelegateQVariant::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<QtnPropertyQVariantBase>::
			acceptKeyPressedForInplaceEditImpl(keyEvent))
		return true;

	// accept any printable key
	return qtnAcceptForLineEdit(keyEvent);
}

QWidget *QtnPropertyDelegateQVariant::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto editor = new QtnLineEditBttn(parent);
	editor->setGeometry(rect);

	new QtnPropertyQVariantEditBttnHandler(this, *editor);

	qtnInitLineEdit(editor->lineEdit, inplaceInfo);
	return editor;
}

bool QtnPropertyDelegateQVariant::propertyValueToStrImpl(
	QString &strValue) const
{
	auto value = owner().value();
	strValue = QtnPropertyQVariant::valueToString(value);

	if (strValue.isEmpty())
		strValue = QtnPropertyQVariant::getPlaceholderStr(value.type());

	return true;
}

void QtnPropertyDelegateQVariant::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	if (stateProperty()->isMultiValue())
	{
		QtnPropertyDelegateTyped::drawValueImpl(painter, rect);
		return;
	}

	QPen oldPen = painter.pen();

	if (QtnPropertyQVariant::valueToString(owner().value()).isEmpty())
		painter.setPen(Qt::darkGray);

	Inherited::drawValueImpl(painter, rect);
	painter.setPen(oldPen);
}

QtnPropertyQVariantEditBttnHandler::QtnPropertyQVariantEditBttnHandler(
	QtnPropertyDelegate *delegate, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(delegate, editor)
	, dialog(new CustomPropertyEditorDialog(&editor))
	, is_object(false)
{
	dialogContainer = connectDialog(dialog);
	updateEditor();

	editor.lineEdit->installEventFilter(this);

	QObject::connect(editor.toolButton, &QToolButton::clicked, this,
		&QtnPropertyQVariantEditBttnHandler::onToolButtonClicked);

	QObject::connect(editor.lineEdit, &QLineEdit::editingFinished, this,
		&QtnPropertyQVariantEditBttnHandler::onEditingFinished);

	QObject::connect(dialog, &CustomPropertyEditorDialog::apply, this,
		&QtnPropertyQVariantEditBttnHandler::onApplyData);
}

void QtnPropertyQVariantEditBttnHandler::revertInput()
{
	reverted = true;
}

void QtnPropertyQVariantEditBttnHandler::onToolButtonClick()
{
	onToolButtonClicked(false);
}

void QtnPropertyQVariantEditBttnHandler::updateEditor()
{
	auto edit = editor().lineEdit;
	edit->setReadOnly(!stateProperty()->isEditableByUser());

	if (stateProperty()->isMultiValue())
	{
		edit->clear();
		edit->setPlaceholderText(QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		QVariant value;
		value = property().value();

		if (QtnPropertyQVariant::variantIsObject(value.type()))
		{
			is_object = true;
			edit->setText(QString());
		} else
		{
			is_object = false;
			edit->setText(value.toString());
		}

		edit->setPlaceholderText(
			QtnPropertyQVariant::getPlaceholderStr(value.type()));
		edit->selectAll();
	}
}

void QtnPropertyQVariantEditBttnHandler::onEditingFinished()
{
	if (canApply())
	{
		auto text = editor().lineEdit->text();

		if (!is_object || !text.isEmpty())
		{
			if (is_object || text != property().value().toString())
				property().setValue(text, delegate()->editReason());

			updateEditor();
		}
	}

	applyReset();
}

void QtnPropertyQVariantEditBttnHandler::onToolButtonClicked(bool)
{
	QVariant data;
	auto text = editor().lineEdit->text();

	auto property = &this->property();
	auto value = property->value();

	if (!is_object && text != value.toString())
	{
		data = text;
	} else
		data = value;

	auto dialogContainer = this->dialogContainer;
	reverted = true;
	dialog->setReadOnly(!stateProperty()->isEditableByUser());

	volatile bool destroyed = false;
	auto connection = QObject::connect(this, &QObject::destroyed,
		[&destroyed]() mutable { destroyed = true; });

	if (dialog->execute(property->name(), data) && !destroyed)
		property->setValue(data, delegate()->editReason());

	if (!destroyed)
	{
		QObject::disconnect(connection);
		updateEditor();
	}

	Q_UNUSED(dialogContainer);
}

void QtnPropertyQVariantEditBttnHandler::onApplyData(const QVariant &data)
{
	property().setValue(data, delegate()->editReason());
	updateEditor();
}
