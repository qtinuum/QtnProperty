/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
 */

#include "Enum.h"
#include <QRegExp>
#include <QStringList>

namespace Qtinuum
{

EnumInfo::EnumInfo(const QString &name, QVector<EnumValueInfo>& staticValues)
    : m_name(name)
{
    m_staticValues.swap(staticValues);
}

const EnumValueInfo* EnumInfo::findByValue(EnumValueType value) const
{
    const EnumValueInfo* result = nullptr;

    forEachEnumValue([&result, value](const EnumValueInfo& enumValue)->bool {
        if (enumValue.value() == value)
        {
            result = &enumValue;
            return false;
        }

        return true;
    });

    return result;
}

const EnumValueInfo* EnumInfo::findByName(const QString& name, Qt::CaseSensitivity cs) const
{
    const EnumValueInfo* result = nullptr;

    forEachEnumValue([&result, &name, cs](const EnumValueInfo& enumValue)->bool {
        if (QString::compare(enumValue.name(), name, cs) == 0)
        {
            result = &enumValue;
            return false;
        }

        return true;
    });

    return result;
}

const EnumValueInfo* EnumInfo::fromStr(const QString& str) const
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

    const EnumValueInfo* enumValue = findByName(enumStr, Qt::CaseInsensitive);
    return enumValue;
}

bool EnumInfo::toStr(QString& str, const EnumValueInfo* value) const
{
    if (!value)
        return false;

    str = QString("%1::%2").arg(name(), value->name());
    return true;
}


} // end namespace Qtinuum
