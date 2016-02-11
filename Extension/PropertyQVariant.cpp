#include "PropertyQVariant.h"


QtnPropertyQVariantBase::QtnPropertyQVariantBase(QObject *parent)
	: QtnSinglePropertyBase<QVariant>(parent)
{
}

bool QtnPropertyQVariantBase::fromStrImpl(const QString &str)
{
	return setValue(str);
}

bool QtnPropertyQVariantBase::toStrImpl(QString &str) const
{
	str = value().toString();
	return true;
}

QtnPropertyQVariantCallback::QtnPropertyQVariantCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQVariantBase>(parent)
{
}


QtnPropertyQVariant::QtnPropertyQVariant(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQVariantBase>(parent)
{
}

QtnPropertyDelegateQVariant::QtnPropertyDelegateQVariant(QtnPropertyQVariantBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyQVariantBase>(owner)
	, check_is_object(false)
{

}

bool QtnPropertyDelegateQVariant::variantIsObject(const QVariant &variant)
{
	switch (variant.type())
	{
		case QVariant::Map:
		case QVariant::List:
			return true;

		default:
			break;
	}

	return false;
}

bool QtnPropertyDelegateQVariant::acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const
{

}

QWidget *QtnPropertyDelegateQVariant::createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{

}

bool QtnPropertyDelegateQVariant::propertyValueToStr(QString &strValue) const
{

}

void QtnPropertyDelegateQVariant::drawValueImpl(QStylePainter &painter, const QRect &rect, const QStyle::State &state, bool *needTooltip) const
{

}
