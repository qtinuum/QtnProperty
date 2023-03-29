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

#include "PropertyEnumFlags.h"
#include <QRegularExpression>

QtnPropertyEnumFlagsBase::QtnPropertyEnumFlagsBase(QObject *parent)
	: QtnSinglePropertyBase<QtnEnumFlagsValueType>(parent)
	, m_enumInfo(0)
{
}

bool QtnPropertyEnumFlagsBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	if (!m_enumInfo)
		return false;

	QtnEnumFlagsValueType val = 0;
	QString enumStr = str.trimmed();

	if (!enumStr.isEmpty() && enumStr != "0")
	{
        static QRegularExpression parserEnumFlags(
            QStringLiteral("^\\s*([^|\\s]+)\\s*\\|(.+)$"), QRegularExpression::CaseInsensitiveOption);

        QRegularExpressionMatch match = parserEnumFlags.match(enumStr);
        while (match.hasMatch())
		{
            QStringList params = match.capturedTexts();

			if (params.size() != 3)
				return false;

			const QtnEnumValueInfo *enumValue = m_enumInfo->fromStr(params[1]);

			if (!enumValue)
				return false;

			val = val | enumValue->value();

			enumStr = params[2];
            match = parserEnumFlags.match(enumStr);
		}

		const QtnEnumValueInfo *enumValue = m_enumInfo->fromStr(enumStr);

		if (!enumValue)
			return false;

		val = val | enumValue->value();
	}

	return setValue(val, reason);
}

bool QtnPropertyEnumFlagsBase::toStrImpl(QString &str) const
{
	if (!m_enumInfo)
		return false;

	QtnEnumFlagsValueType v = value();

	if (v == 0)
	{
		str = "0";
		return true;
	}

	QString strValues;
	m_enumInfo->forEachEnumValue(
		[&strValues, v, this](const QtnEnumValueInfo &value) -> bool //
		{
			if (v & value.value())
			{
				if (!strValues.isEmpty())
					strValues += "|";

				QString enumStr;
				m_enumInfo->toStr(enumStr, &value);
				strValues += enumStr;
			}

			return true;
		});

	Q_ASSERT(!strValues.isEmpty());

	str = strValues;
	return true;
}

QtnPropertyEnumFlags::QtnPropertyEnumFlags(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyEnumFlagsBase>(parent)
{
}

QString QtnPropertyEnumFlags::getFlagLabelDescription(
	const QString &flagName, const QString &ownerName)
{
	return tr("%1 flag for %2").arg(flagName, ownerName);
}

QtnPropertyEnumFlagsCallback::QtnPropertyEnumFlagsCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyEnumFlagsBase>(parent)
{
}
