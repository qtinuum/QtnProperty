/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

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

QtnEnumInfo::QtnEnumInfo(const QString &name, QVector<QtnEnumValueInfo>& staticValues)
    : m_name(name)
{
    m_staticValues.swap(staticValues);
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

const QtnEnumValueInfo* QtnEnumInfo::findByName(const QString& name, Qt::CaseSensitivity cs) const
{
    const QtnEnumValueInfo* result = nullptr;

    forEachEnumValue([&result, &name, cs](const QtnEnumValueInfo& enumValue)->bool {
        if (QString::compare(enumValue.name(), name, cs) == 0)
        {
            result = &enumValue;
            return false;
        }

        return true;
    });

    return result;
}

const QtnEnumValueInfo* QtnEnumInfo::findByDisplayName(const QString& displayName, Qt::CaseSensitivity cs) const
{
    const QtnEnumValueInfo* result = nullptr;

    forEachEnumValue([&result, &displayName, cs](const QtnEnumValueInfo& enumValue)->bool {
        if (QString::compare(enumValue.displayName(), displayName, cs) == 0)
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
    static QRegExp parserEnum("^\\s*([^:\\s]+)::([^:\\s]+)\\s*$", Qt::CaseInsensitive);

    QString enumStr = str.trimmed();

    if (parserEnum.exactMatch(str))
    {
        QStringList params = parserEnum.capturedTexts();
        if (params.size() != 3)
            return nullptr;

        if (QString::compare(params[1], name(), Qt::CaseInsensitive) != 0)
            return nullptr;

        enumStr = params[2];
    }

    const QtnEnumValueInfo* enumValue = findByName(enumStr, Qt::CaseInsensitive);
    return enumValue;
}

bool QtnEnumInfo::toStr(QString& str, const QtnEnumValueInfo* value) const
{
    if (!value)
        return false;

    str = QString("%1::%2").arg(name(), value->name());
    return true;
}

bool QtnEnumInfo::toStr(QString& str, QtnEnumValueType value) const
{
    return toStr(str, findByValue(value));
}

void QtnEnumInfo::setDynamicValues(const QVector<QtnEnumValueInfo>& dynamicValues)
{
    m_dynamicValues = dynamicValues;
}
