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

#include "PropertyFloat.h"

#include <QLocale>

QtnPropertyFloatBase::QtnPropertyFloatBase(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<float>>(parent)
{
}

bool QtnPropertyFloatBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	bool ok = false;
	ValueType value = str.toFloat(&ok);

	if (!ok)
		return false;

	return setValue(value, reason);
}

bool QtnPropertyFloatBase::toStrImpl(QString &str) const
{
	str = QString::number(value(), 'g', 5);
	return true;
}

QtnPropertyFloatCallback::QtnPropertyFloatCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyFloatBase>(parent)
{
}

QtnPropertyFloat::QtnPropertyFloat(QObject *parent)
	: QtnNumericPropertyValue<QtnPropertyFloatBase>(parent)
{
}
