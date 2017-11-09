/*******************************************************************************
Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyDouble.h"

#include <QLocale>

QtnPropertyDoubleBase::QtnPropertyDoubleBase(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<double>>(parent)
{
}

bool QtnPropertyDoubleBase::fromStrImpl(const QString &str, bool edit)
{
	bool ok = false;
	ValueType value = str.toDouble(&ok);

	if (!ok)
	{
		value = QLocale().toDouble(str, &ok);

		if (!ok)
			return false;
	}

	return setValue(value, edit);
}

bool QtnPropertyDoubleBase::toStrImpl(QString &str) const
{
	str = QString::number(value(), 'f', 10);
	return true;
}

bool QtnPropertyDoubleBase::fromVariantImpl(const QVariant &var, bool edit)
{
	bool ok = false;
	ValueType value = var.toDouble(&ok);

	if (!ok)
		return false;

	return setValue(value, edit);
}

QtnPropertyDoubleCallback::QtnPropertyDoubleCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyDoubleBase>(parent)
{
}

QtnPropertyDouble::QtnPropertyDouble(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyDoubleBase>(parent)
{
}
