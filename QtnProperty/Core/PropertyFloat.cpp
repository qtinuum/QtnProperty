/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "PropertyFloat.h"

#include <QLocale>

QtnPropertyFloatBase::QtnPropertyFloatBase(QObject *parent)
	: QtnNumericPropertyBase<QtnSinglePropertyBase<float> >(parent)
{
}

bool QtnPropertyFloatBase::fromStrImpl(const QString& str, bool edit)
{
	bool ok = false;
	ValueType value = str.toFloat(&ok);
	if (!ok)
	{
		value = QLocale().toFloat(str, &ok);
		if (!ok)
			return false;
	}

	return setValue(value, edit);
}

bool QtnPropertyFloatBase::toStrImpl(QString& str) const
{
	str = QString::number(value(), 'f', 6);
	return true;
}

bool QtnPropertyFloatBase::fromVariantImpl(const QVariant& var, bool edit)
{
	bool ok = false;
	ValueType value = var.toFloat(&ok);
	if (!ok)
		return false;

	return setValue(value, edit);
}

QtnPropertyFloatCallback::QtnPropertyFloatCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyFloatBase>(parent)
{
}

QtnPropertyFloat::QtnPropertyFloat(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyFloatBase>(parent)
{
}
