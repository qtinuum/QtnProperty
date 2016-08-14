#include "PropertyPercent.h"

#include "QtnProperty/Widget/Delegates/PropertyDelegateFactory.h"

#include "QtnProperty/Core/QObjectPropertySet.h"

#include <QLocale>

QtnPropertyPercentBase::QtnPropertyPercentBase(QObject *parent)
	: QtnPropertyDoubleBase(parent)
{

}

bool QtnPropertyPercentBase::fromStrImpl(const QString &str, bool edit)
{
	auto value = str;
	int new_len = value.length() - 1;
	if (new_len > 0)
	{
		auto c = value.at(new_len);
		if (c == '%' || c == QLocale().percent())
			value.resize(new_len);
	}
	return QtnPropertyDoubleBase::fromStrImpl(value, edit);
}

bool QtnPropertyPercentBase::toStrImpl(QString &str) const
{
	auto result = QtnPropertyDoubleBase::toStrImpl(str);

	str.append('%');

	return result;
}

QtnPropertyPercent::QtnPropertyPercent(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyPercentBase>(parent)
{

}

static QtnProperty *createRealNumberProperty(QObject *object, const QMetaProperty &metaProperty)
{
	if (metaProperty.revision() == 1)
	{
		auto property = new QtnPropertyPercentCallback(nullptr);

		property->setCallbackValueGet([object, metaProperty]() -> qreal
		{
			return metaProperty.read(object).toDouble() * 100.0;
		});

		property->setCallbackValueSet([object, metaProperty](qreal value)
		{
			metaProperty.write(object, value / 100.0);
		});

		property->setCallbackValueAccepted([property](qreal) -> bool
		{
			return property->isEditableByUser();
		});

		return property;
	}
	auto property = new QtnPropertyDoubleCallback(nullptr);

	property->setCallbackValueGet([object, metaProperty]() -> qreal
	{
		return metaProperty.read(object).toDouble();
	});

	property->setCallbackValueSet([object, metaProperty](qreal value)
	{
		metaProperty.write(object, value);
	});

	property->setCallbackValueAccepted([property](qreal) -> bool
	{
		return property->isEditableByUser();
	});

	return property;
}

void QtnPropertyPercent::Register()
{
	qtnRegisterMetaPropertyFactory(QVariant::Double, createRealNumberProperty, true);

	QtnPropertyDelegateFactory::staticInstance()
			.registerDelegateDefault(&QtnPropertyPercentBase::staticMetaObject
			, &qtnCreateDelegate<QtnPropertyDelegatePercent, QtnPropertyPercentBase>
			, "SpinBox");
}

QtnPropertyPercentCallback::QtnPropertyPercentCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyPercentBase>(parent)
{

}

QtnPropertyDelegatePercent::QtnPropertyDelegatePercent(QtnPropertyPercentBase &owner)
	: QtnPropertyDelegateDouble(owner)
{
	QtnPropertyDelegateDouble::percent_suffix = true;
}
