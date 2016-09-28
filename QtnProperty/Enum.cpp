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

#include "Enum.h"
#include <QRegExp>
#include <QStringList>

QtnEnumInfo::QtnEnumInfo()
	: m_case_sensitivity(Qt::CaseSensitive)
{

}

QtnEnumInfo::QtnEnumInfo(const QString &name, QVector<QtnEnumValueInfo>& staticValues)
	: m_case_sensitivity(Qt::CaseSensitive)
	, m_name(name)
{
	m_values.swap(staticValues);
}

const QtnEnumValueInfo* QtnEnumInfo::findByValue(QtnEnumValueType value) const
{
    const QtnEnumValueInfo* result = nullptr;

    forEachEnumValue([&result, value](const QtnEnumValueInfo& enumValue)->bool {
        if (enumValue.value() == value)
        {
            result = &enumValue;
            return false;
        }

        return true;
    });

    return result;
}

const QtnEnumValueInfo* QtnEnumInfo::findByName(const QString& name) const
{
    const QtnEnumValueInfo* result = nullptr;

	forEachEnumValue([&result, &name, this](const QtnEnumValueInfo& enumValue)->bool {
		if (QString::compare(enumValue.name(), name, m_case_sensitivity) == 0)
        {
            result = &enumValue;
            return false;
        }

        return true;
    });

    return result;
}

const QtnEnumValueInfo* QtnEnumInfo::fromStr(const QString& str) const
{
	static QRegExp parserEnum("^\\s*([^:\\s]+)::([^:\\s]+)\\s*$", m_case_sensitivity);

    QString enumStr = str.trimmed();

    if (parserEnum.exactMatch(str))
    {
        QStringList params = parserEnum.capturedTexts();
        if (params.size() != 3)
            return nullptr;

		if (QString::compare(params[1], name(), m_case_sensitivity) != 0)
            return nullptr;

        enumStr = params[2];
    }

	return findByName(enumStr);
}

bool QtnEnumInfo::toStr(QString& str, const QtnEnumValueInfo* value) const
{
    if (!value)
        return false;

    str = QString("%1::%2").arg(name(), value->name());
    return true;
}

QVector<QtnEnumValueInfo> &QtnEnumInfo::getVector()
{
	return m_values;
}
