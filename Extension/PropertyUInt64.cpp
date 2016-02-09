#include "PropertyUInt64.h"

#include "Extension.h"

#include "QtnProperty/Widget/Delegates/PropertyEditorAux.h"
#include "QtnProperty/Widget/Delegates/PropertyDelegateFactory.h"

#include <QLocale>
#include <QKeyEvent>

QtnPropertyUInt64Base::QtnPropertyUInt64Base(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<quint64>>(parent)
{
}

bool QtnPropertyUInt64Base::fromStrImpl(const QString &str)
{
	bool ok = false;
	ValueType value = str.toULongLong(&ok);
	if (!ok)
	{
		value = QLocale().toULongLong(str, &ok);
		if (!ok)
			return false;
	}

	return setValue(value);
}

bool QtnPropertyUInt64Base::toStrImpl(QString &str) const
{
	str = QString::number(value());
	return true;
}

bool QtnPropertyUInt64Base::fromVariantImpl(const QVariant &var)
{
	bool ok = false;
	ValueType value = var.toULongLong(&ok);
	if (!ok)
		return false;

	return setValue(value);
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
{

}

bool QtnPropertyDelegateUInt64::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		// revert all changes
		if (keyEvent->key() == Qt::Key_Escape)
			updateEditor();
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
	auto value = QLocale().toULongLong(editor->text(), &ok);

	if (ok && value >= owner().minValue() && value <= owner().maxValue())
		owner().setValue(value);
	else
		updateEditor();
}

void QtnPropertyDelegateUInt64::onEditorDestroyed()
{
	editor = nullptr;
}

void QtnPropertyDelegateUInt64::updateEditor()
{
	QString str;
	propertyValueToStr(str);
	editor->setText(str);
}
