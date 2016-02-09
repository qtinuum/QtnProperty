#include "PropertyQRectF.h"

#include "QtnProperty/Widget/Delegates/PropertyDelegateFactory.h"

#include "QtnProperty/Core/Core/PropertyQRect.h"

#include "Extension.h"

#include <QLocale>

QtnPropertyQRectFBase::QtnPropertyQRectFBase(QObject *parent)
	: ParentClass(parent)
	, rect_parser("\\s*\\[\\s*\\(\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*,"
				  "\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\)\\s*,"
				  "\\s*((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+))\\s*x"
				  "\\s*((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+))\\s*\\]\\s*")
{
	addState(QtnPropertyStateCollapsed);
}

QtnProperty *QtnPropertyQRectFBase::createLeftProperty()
{
	return createFieldProperty(QtnPropertyQRectF::tr("Left"),
							   QtnPropertyQRectF::tr("Left position of the %1"),
							   &QRectF::left, &QRectF::moveLeft);
}

QtnProperty *QtnPropertyQRectFBase::createTopProperty()
{
	return createFieldProperty(QtnPropertyQRectF::tr("Top"),
							   QtnPropertyQRectF::tr("Top position of the %1"),
							   &QRectF::top, &QRectF::moveTop);
}

QtnProperty *QtnPropertyQRectFBase::createWidthProperty()
{
	return createFieldProperty(QtnPropertyQRectF::tr("Width"),
							   QtnPropertyQRectF::tr("Width of the %1"),
							   &QRectF::width, &QRectF::setWidth);
}

QtnProperty *QtnPropertyQRectFBase::createHeightProperty()
{
	return createFieldProperty(QtnPropertyQRectF::tr("Height"),
							   QtnPropertyQRectF::tr("Height of the %1"),
							   &QRectF::height, &QRectF::setHeight);
}

bool QtnPropertyQRectFBase::fromStrImpl(const QString &str)
{
	if (!rect_parser.exactMatch(str))
		return false;

	bool ok;
	qreal x = rect_parser.cap(1).toDouble(&ok);
	if (!ok)
		return false;

	qreal y = rect_parser.cap(6).toDouble(&ok);
	if (!ok)
		return false;

	qreal width = rect_parser.cap(11).toDouble(&ok);
	if (!ok)
		return false;

	qreal height = rect_parser.cap(15).toDouble(&ok);
	if (!ok)
		return false;

	return setValue(QRectF(x, y, width, height));
}

bool QtnPropertyQRectFBase::toStrImpl(QString &str) const
{
	auto v = value();
	str = QString("[(%1, %2), %3 x %4]").arg(v.left()).arg(v.top()).arg(v.width()).arg(v.height());
	return true;
}

QtnPropertyQRectFCallback::QtnPropertyQRectFCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQRectFBase>(parent)
{
}

QtnPropertyQRectF::QtnPropertyQRectF(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQRectFBase>(parent)
{
}

void QtnPropertyQRectF::Register()
{
	qtnRegisterMetaPropertyFactory(QVariant::RectF, qtnCreateFactory<QtnPropertyQRectFCallback>());

	QtnPropertyDelegateFactory::staticInstance()
			.registerDelegateDefault(&QtnPropertyQRectFBase::staticMetaObject
			, &qtnCreateDelegate<QtnPropertyDelegateQRectF, QtnPropertyQRectFBase>
			, "QRectF");
}

QtnPropertyDelegateQRectF::QtnPropertyDelegateQRectF(QtnPropertyQRectFBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQRectFBase>(owner)
{
	addSubProperty(owner.createLeftProperty());
	addSubProperty(owner.createTopProperty());
	addSubProperty(owner.createWidthProperty());
	addSubProperty(owner.createHeightProperty());
}

QWidget *QtnPropertyDelegateQRectF::createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQRectF::propertyValueToStr(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQRect::getToStringFormat().arg(locale.toString(value.left(), 'g', 6),
														 locale.toString(value.top(), 'g', 6),
														 locale.toString(value.width(), 'g', 6),
														 locale.toString(value.height(), 'g', 6));

	return true;
}
