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

#include "PropertyQString.h"

QtnPropertyQStringBase::QtnPropertyQStringBase(QObject *parent)
	: QtnSinglePropertyBase<QString>(parent)
	, multiline(false)
{
}

bool QtnPropertyQStringBase::isMultilineEnabled() const
{
	return multiline;
}

void QtnPropertyQStringBase::setMultilineEnabled(bool enabled)
{
	multiline = enabled;
}

QString QtnPropertyQStringBase::getPlaceholderText() const
{
	return QtnPropertyQString::getPlaceholderStr(value(), multiline);
}

QtnPropertyQStringBase &QtnPropertyQStringBase::operator=(const char *newValue)
{
	setValue(QString(newValue));
	return *this;
}

bool QtnPropertyQStringBase::fromStrImpl(const QString &str, bool edit)
{
	if (!multiline)
	{
		int n = str.indexOf('\n');
		int r = str.indexOf('\r');
		int len = n < 0 ? r : (r < 0 ? n : qMin(n, r));
		return setValue(QString(str.data(), len), edit);
	}

	return setValue(str, edit);
}

bool QtnPropertyQStringBase::toStrImpl(QString &str) const
{
	str = value();
	return true;
}

QtnPropertyQString::QtnPropertyQString(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQStringBase>(parent)
{
}

QtnPropertyQString &QtnPropertyQString::operator=(const char *newValue)
{
	setValue(QString(newValue));
	return *this;
}

bool QtnPropertyQString::isMultilineText(const QString &text)
{
	return text.contains('\n') || text.contains('\r');
}

QString QtnPropertyQString::getPlaceholderStr(
	const QString &text, bool checkMultiline)
{
	if (checkMultiline && isMultilineText(text))
		return tr("(Multiline Text)");

	if (text.isEmpty())
		return tr("(Empty)");

	return QString();
}

QString QtnPropertyQString::getReadOnlyPropertyTitleFormat()
{
	return tr("%1 (Read only)");
}

QtnPropertyQStringCallback::QtnPropertyQStringCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQStringBase>(parent)
{
}

QtnPropertyQStringCallback &QtnPropertyQStringCallback::operator=(
	const char *newValue)
{
	setValue(QString(newValue));
	return *this;
}
