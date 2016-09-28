#include "PropertyUInt64.h"

#include "QObjectPropertySet.h"
#include "Delegates/PropertyEditorAux.h"
#include "Delegates/PropertyDelegateFactory.h"

#include <QLocale>
#include <QKeyEvent>

QtnPropertyUInt64Base::QtnPropertyUInt64Base(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<quint64>>(parent)
{
}

bool QtnPropertyUInt64Base::fromStrImpl(const QString &str, bool edit)
{
	bool ok = false;
	ValueType value = str.toULongLong(&ok);
	if (!ok)
	{
		value = QLocale().toULongLong(str, &ok);
		if (!ok)
			return false;
	}

	return setValue(value, edit);
}

bool QtnPropertyUInt64Base::toStrImpl(QString &str) const
{
	str = QString::number(value());
	return true;
}

bool QtnPropertyUInt64Base::fromVariantImpl(const QVariant &var, bool edit)
{
	bool ok = false;
	ValueType value = var.toULongLong(&ok);
	if (!ok)
		return false;

	return setValue(value, edit);
}


QtnPropertyUInt64Callback::QtnPropertyUInt64Callback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyUInt64Base>(parent)
{
}


QtnPropertyUInt64::QtnPropertyUInt64(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyUInt64Base>(parent)
{
}

void QtnPropertyUInt64::Register()
{
	qtnRegisterMetaPropertyFactory(QVariant::ULongLong, qtnCreateFactory<QtnPropertyUInt64Callback>());

	QtnPropertyDelegateFactory::staticInstance()
		.registerDelegateDefault(&QtnPropertyUInt64Base::staticMetaObject
		, &qtnCreateDelegate<QtnPropertyDelegateUInt64, QtnPropertyUInt64Base>
		, "LineEdit");
}

QtnPropertyDelegateUInt64::QtnPropertyDelegateUInt64(QtnPropertyUInt64Base &owner)
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
		auto keyEvent = static_cast<QKeyEvent*>(event);

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

bool QtnPropertyDelegateUInt64::acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<QtnPropertyUInt64Base>::acceptKeyPressedForInplaceEditImpl(keyEvent))
		return true;

	// accept any printable key
	return qtnAcceptForLineEdit(keyEvent);
}

QWidget *QtnPropertyDelegateUInt64::createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	editor = static_cast<QLineEdit *>(createValueEditorLineEdit(parent, rect, !owner().isEditableByUser(), inplaceInfo));

	editor->installEventFilter(this);
	QObject::connect(editor, &QLineEdit::editingFinished,
					 this, &QtnPropertyDelegateUInt64::onEditingFinished);

	QObject::connect(editor, &QObject::destroyed,
					 this, &QtnPropertyDelegateUInt64::onEditorDestroyed);

	return editor;
}

bool QtnPropertyDelegateUInt64::propertyValueToStr(QString &strValue) const
{
	auto value = owner().value();
	strValue = QLocale().toString(value);
	return true;
}

void QtnPropertyDelegateUInt64::onEditingFinished()
{
	bool ok = false;

	if (!reverted && (applied || !owner().valueIsHidden()))
	{
		auto value = QLocale().toULongLong(editor->text(), &ok);
		ok = ok && value >= owner().minValue() && value <= owner().maxValue();

		if (ok)
			owner().edit(value);
	}

	if (!ok)
		updateEditor();

	reverted = false;
	applied = false;
}

void QtnPropertyDelegateUInt64::onEditorDestroyed()
{
	editor = nullptr;
}

void QtnPropertyDelegateUInt64::updateEditor()
{
	if (owner().valueIsHidden())
		editor->clear();
	else
	{
		QString str;
		propertyValueToStr(str);
		editor->setText(str);
	}
}
