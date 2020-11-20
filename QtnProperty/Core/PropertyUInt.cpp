/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyUInt.h"

#include <QLocale>

QtnPropertyUIntBase::QtnPropertyUIntBase(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<quint32>>(parent)
{
}

bool QtnPropertyUIntBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	bool ok = false;
	ValueType value = str.toUInt(&ok);

	if (!ok)
		return false;

	return setValue(value, reason);
}

bool QtnPropertyUIntBase::toStrImpl(QString &str) const
{
	str = QString::number(value());
	return true;
}

QtnPropertyUIntCallback::QtnPropertyUIntCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyUIntBase>(parent)
{
}

QtnPropertyUInt::QtnPropertyUInt(QObject *parent)
	: QtnNumericPropertyValue<QtnPropertyUIntBase>(parent)
{
}
