#include "PropertyPercent.h"

#include "QtnProperty/Widget/Delegates/PropertyDelegateFactory.h"

#include <QLocale>

QtnPropertyPercentBase::QtnPropertyPercentBase(QObject *parent)
	: QtnPropertyDoubleBase(parent)
{

}

bool QtnPropertyPercentBase::fromStrImpl(const QString &str)
{
	auto value = str;
	int new_len = value.length() - 1;
	if (new_len > 0)
	{
		auto c = value.at(new_len);
		if (c == '%' || c == QLocale().percent())
			value.resize(new_len);
	}
	return QtnPropertyDoubleBase::fromStrImpl(value);
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

void QtnPropertyPercent::Register()
{
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
