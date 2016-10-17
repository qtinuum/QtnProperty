/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyBool.h"

#include <QCoreApplication>

static bool getBoolValue(QString boolText, bool& success)
{
	success = true;
	if (0 == boolText.compare(QtnPropertyBool::getBoolText(false, true), Qt::CaseInsensitive))
		return false;

	if (0 == boolText.compare(QtnPropertyBool::getBoolText(true, true), Qt::CaseInsensitive))
		return true;

	if (0 == boolText.compare(QtnPropertyBool::getBoolText(false, false), Qt::CaseInsensitive))
		return false;

	if (0 == boolText.compare(QtnPropertyBool::getBoolText(true, false), Qt::CaseInsensitive))
		return true;

	if (boolText.toULongLong(&success) != 0)
		return true;

	success = false;
	return false;
}

QtnPropertyBoolBase::QtnPropertyBoolBase(QObject *parent)
	: QtnSinglePropertyBase<bool>(parent)
{
}

bool QtnPropertyBoolBase::fromStrImpl(const QString& str, bool edit)
{
	bool success = false;
	bool value = getBoolValue(str.trimmed(), success);

	if (!success)
		return false;

	return setValue(value, edit);
}

bool QtnPropertyBoolBase::toStrImpl(QString& str) const
{
	bool boolValue = value();
	str = QtnPropertyBool::getBoolText(boolValue, true);
	return true;
}

QtnPropertyBool::QtnPropertyBool(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyBoolBase>(parent)
{
}

QString QtnPropertyBool::getBoolText(bool value, bool internal)
{
	static const char pFalse[] = QT_TRANSLATE_NOOP("QtnPropertyBool", "False");
	static const char pTrue[] = QT_TRANSLATE_NOOP("QtnPropertyBool", "True");

	if (internal)
		return QString(value ? pTrue : pFalse);

	return QCoreApplication::translate("QtnPropertyBool", value ? pTrue : pFalse);
}

QtnPropertyBoolCallback::QtnPropertyBoolCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyBoolBase>(parent)
{
}
