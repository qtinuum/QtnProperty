#include "PropertyInt64.h"

#include "Extension.h"

#include "QtnProperty/Widget/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Widget/Delegates/PropertyEditorAux.h"

#include <QLocale>
#include <QKeyEvent>

QtnPropertyInt64Base::QtnPropertyInt64Base(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<qint64>>(parent)
{
}

bool QtnPropertyInt64Base::fromStrImpl(const QString &str)
{
	bool ok = false;
	ValueType value = str.toLongLong(&ok);
	if (!ok)
	{
		value = QLocale().toLongLong(str, &ok);
		if (!ok)
			return false;
	}

	return setValue(value);
}

bool QtnPropertyInt64Base::toStrImpl(QString &str) const
{
	str = QString::number(value());
	return true;
}

bool QtnPropertyInt64Base::fromVariantImpl(const QVariant &var)
{
	bool ok = false;
	ValueType value = var.toLongLong(&ok);
	if (!ok)
		return false;

	return setValue(value);
}


QtnPropertyInt64::QtnPropertyInt64(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyInt64Base>(parent)
{
}

void QtnPropertyInt64::Register()
{
	qtnRegisterMetaPropertyFactory(QVariant::LongLong, qtnCreateFactory<QtnPropertyInt64Callback>());

	QtnPropertyDelegateFactory::staticInstance()
		.registerDelegateDefault(&QtnPropertyInt64Base::staticMetaObject
		, &qtnCreateDelegate<QtnPropertyDelegateInt64, QtnPropertyInt64Base>
		, "LineEdit");
}

QtnPropertyDelegateInt64::QtnPropertyDelegateInt64(QtnPropertyInt64Base &owner)
	: QObject(nullptr)
	, QtnPropertyDelegateTyped<QtnPropertyInt64Base>(owner)
	, editor(nullptr)
{

}

bool QtnPropertyDelegateInt64::eventFilter(QObject *obj, QEvent *event)
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

bool QtnPropertyDelegateInt64::acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<QtnPropertyInt64Base>::acceptKeyPressedForInplaceEditImpl(keyEvent))
		return true;

	// accept any printable key
	return qtnAcceptForLineEdit(keyEvent);
}

QWidget *QtnPropertyDelegateInt64::createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	editor = static_cast<QLineEdit *>(createValueEditorLineEdit(parent, rect, !owner().isEditableByUser(), inplaceInfo));

	editor->installEventFilter(this);
	QObject::connect(editor, &QLineEdit::editingFinished,
					 this, &QtnPropertyDelegateInt64::onEditingFinished);

	QObject::connect(editor, &QObject::destroyed,
					 this, &QtnPropertyDelegateInt64::onEditorDestroyed);

	return editor;
}

bool QtnPropertyDelegateInt64::propertyValueToStr(QString &strValue) const
{
	auto value = owner().value();
	strValue = QLocale().toString(value);
	return true;
}

void QtnPropertyDelegateInt64::onEditingFinished()
{
	bool ok = false;
	auto value = QLocale().toLongLong(editor->text(), &ok);

	if (ok && value >= owner().minValue() && value <= owner().maxValue())
		owner().setValue(value);
	else
		updateEditor();
}

void QtnPropertyDelegateInt64::onEditorDestroyed()
{
	editor = nullptr;
}

void QtnPropertyDelegateInt64::updateEditor()
{
	QString str;
	propertyValueToStr(str);
	editor->setText(str);
}

QtnPropertyInt64Callback::QtnPropertyInt64Callback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyInt64Base>(parent)
{

}
