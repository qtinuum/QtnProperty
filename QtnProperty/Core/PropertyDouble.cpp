/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

QtnPropertyDoubleBase::QtnPropertyDoubleBase(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<double>>(parent)
{
}

bool QtnPropertyDoubleBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	bool ok = false;
	ValueType value = str.toDouble(&ok);

	if (!ok)
		return false;

	return setValue(value, reason);
}

bool QtnPropertyDoubleBase::toStrImpl(QString &str) const
{
	str = QString::number(value(), 'f', std::numeric_limits<double>::digits10);
	return true;
}

QtnPropertyDoubleCallback::QtnPropertyDoubleCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyDoubleBase>(parent)
{
}

QtnPropertyDouble::QtnPropertyDouble(QObject *parent)
	: QtnNumericPropertyValue<QtnPropertyDoubleBase>(parent)
{
}
