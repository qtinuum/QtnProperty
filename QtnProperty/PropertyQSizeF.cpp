#include "PropertyQSizeF.h"

#include "Delegates/PropertyDelegateFactory.h"
#include "Core/PropertyQSize.h"
#include "QObjectPropertySet.h"

#include <QLocale>

QtnPropertyQSizeFBase::QtnPropertyQSizeFBase(QObject *parent)
	: ParentClass(parent)
	, size_parser("\\s*\\[\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*x"
				   "\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*\\]\\s*")
{
	addState(QtnPropertyStateCollapsed);
}

QtnProperty *QtnPropertyQSizeFBase::createWidthProperty()
{
	return createFieldProperty(QtnPropertyQSizeF::tr("Width"),
							   QtnPropertyQSizeF::tr("Width of the %1"),
							   &QSizeF::width, &QSizeF::setWidth);
}

QtnProperty *QtnPropertyQSizeFBase::createHeightProperty()
{
	return createFieldProperty(QtnPropertyQSizeF::tr("Height"),
							   QtnPropertyQSizeF::tr("Height of the %1"),
							   &QSizeF::height, &QSizeF::setHeight);
}

bool QtnPropertyQSizeFBase::fromStrImpl(const QString &str, bool edit)
{
	if (!size_parser.exactMatch(str))
		return false;

	bool ok;
	qreal width = size_parser.cap(1).toDouble(&ok);
	if (!ok)
		return false;

	qreal height = size_parser.cap(6).toDouble(&ok);
	if (!ok)
		return false;

	return setValue(QSizeF(width, height), edit);
}

bool QtnPropertyQSizeFBase::toStrImpl(QString &str) const
{
	auto v = value();
	str = QString("[%1 x %2]").arg(v.width()).arg(v.height());
	return true;
}


QtnPropertyQSizeFCallback::QtnPropertyQSizeFCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQSizeFBase>(parent)
{

}


QtnPropertyQSizeF::QtnPropertyQSizeF(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQSizeFBase>(parent)
{

}

void QtnPropertyQSizeF::Register()
{
	qtnRegisterMetaPropertyFactory(QVariant::SizeF, qtnCreateFactory<QtnPropertyQSizeFCallback>());

	QtnPropertyDelegateFactory::staticInstance()
			.registerDelegateDefault(&QtnPropertyQSizeFBase::staticMetaObject
			, &qtnCreateDelegate<QtnPropertyDelegateQSizeF, QtnPropertyQSizeFBase>
			, "QSizeF");
}


QtnPropertyDelegateQSizeF::QtnPropertyDelegateQSizeF(QtnPropertyQSizeFBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQSizeFBase>(owner)
{
	addSubProperty(owner.createWidthProperty());
	addSubProperty(owner.createHeightProperty());
}

QWidget *QtnPropertyDelegateQSizeF::createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQSizeF::propertyValueToStr(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQSize::getToStringFormat().arg(locale.toString(value.width(), 'g', 6),
														 locale.toString(value.height(), 'g', 6));

	return true;
}
