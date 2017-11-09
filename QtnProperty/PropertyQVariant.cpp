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

#include "PropertyQVariant.h"

#include "QObjectPropertySet.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorAux.h"
#include "Delegates/PropertyEditorHandler.h"
#include "MultiProperty.h"

#include "CustomPropertyEditorDialog.h"

#include <QEvent>
#include <QKeyEvent>

class QtnPropertyQVariantEditBttnHandler
	: public QtnPropertyEditorBttnHandler<QtnPropertyQVariantBase,
		  QtnLineEditBttn>
{
public:
	QtnPropertyQVariantEditBttnHandler(
		QtnPropertyQVariantBase &property, QtnLineEditBttn &editor);

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
	: QtnSinglePropertyBase<const QVariant &>(parent)
{
}

bool QtnPropertyQVariantBase::fromStrImpl(const QString &str, bool edit)
{
	return setValue(str, edit);
}

bool QtnPropertyQVariantBase::toStrImpl(QString &str) const
{
	str = value().toString();
	return true;
}

bool QtnPropertyQVariantBase::fromVariantImpl(const QVariant &var, bool edit)
{
	return edit ? this->edit(var) : setValue(var);
}

bool QtnPropertyQVariantBase::toVariantImpl(QVariant &var) const
{
	var = value();
	return var.isValid();
}

QtnPropertyQVariantCallback::QtnPropertyQVariantCallback(
	QObject *object, const QMetaProperty &meta_property)
	: QtnSinglePropertyCallback<QtnPropertyQVariantBase>(nullptr)
{
	setCallbackValueGet([this, object, meta_property]() -> const QVariant & {
		value = meta_property.read(object);
		return value;
	});

	setCallbackValueSet([this, object, meta_property](const QVariant &value) {
		this->value = value;
		meta_property.write(object, value);
	});

	setCallbackValueAccepted(
		[this](const QVariant &) -> bool { return isEditableByUser(); });

	setCallbackValueEqual(
		[this, object, meta_property](const QVariant &value) -> bool {
			this->value = meta_property.read(object);

			return (this->value.isValid() == value.isValid() &&
				value.type() == this->value.type() && value == this->value);
		});
}

QtnPropertyQVariant::QtnPropertyQVariant(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQVariantBase>(parent)
{
}

bool QtnPropertyQVariant::Register()
{
	qtnRegisterMetaPropertyFactory(QMetaType::QVariant,
		[](QObject *object,
			const QMetaProperty &metaProperty) -> QtnProperty * {
			return new QtnPropertyQVariantCallback(object, metaProperty);
		});

	return QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
		&QtnPropertyQVariantBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQVariant,
			QtnPropertyQVariantBase>,
		QByteArrayLiteral("QVariant"));
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

	return tr("(Empty)");
}

QtnPropertyDelegateQVariant::QtnPropertyDelegateQVariant(
	QtnPropertyQVariantBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyQVariantBase>(owner)
{
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

	new QtnPropertyQVariantEditBttnHandler(owner(), *editor);

	qtnInitLineEdit(editor->lineEdit, inplaceInfo);
	return editor;
}

bool QtnPropertyDelegateQVariant::propertyValueToStr(QString &strValue) const
{
	auto &value = owner().value();
	strValue = QtnPropertyQVariant::valueToString(value);

	if (strValue.isEmpty())
		strValue = QtnPropertyQVariant::getPlaceholderStr(value.type());

	return true;
}

void QtnPropertyDelegateQVariant::drawValueImpl(QStylePainter &painter,
	const QRect &rect, const QStyle::State &state, bool *needTooltip) const
{
	QPen oldPen = painter.pen();

	if (QtnPropertyQVariant::valueToString(owner().value()).isEmpty())
		painter.setPen(Qt::darkGray);

	Inherited::drawValueImpl(painter, rect, state, needTooltip);
	painter.setPen(oldPen);
}

QtnPropertyQVariantEditBttnHandler::QtnPropertyQVariantEditBttnHandler(
	QtnPropertyQVariantBase &property, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(property, editor)
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
	edit->setReadOnly(!property().isEditableByUser());

	if (property().valueIsHidden())
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
				property().edit(text);

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
	auto &value = property->value();

	if (!is_object && text != value.toString())
	{
		data = text;
	} else
		data = value;

	auto dialogContainer = this->dialogContainer;
	reverted = true;
	dialog->setReadOnly(!property->isEditableByUser());

	volatile bool destroyed = false;
	auto connection = QObject::connect(this, &QObject::destroyed,
		[&destroyed]() mutable { destroyed = true; });

	if (dialog->execute(property->name(), data) && !destroyed)
		property->edit(data);

	if (!destroyed)
	{
		QObject::disconnect(connection);
		updateEditor();
	}

	Q_UNUSED(dialogContainer);
}

void QtnPropertyQVariantEditBttnHandler::onApplyData(const QVariant &data)
{
	property().edit(data);
	updateEditor();
}
