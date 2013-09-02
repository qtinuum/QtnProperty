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

namespace Qtinuum
{

EnumInfo::EnumInfo(QVector<EnumValueInfo> &staticValues)
{
    m_staticValues.swap(staticValues);
}

const EnumValueInfo *EnumInfo::findByValue(EnumValueType value) const
{
    const EnumValueInfo *result = 0;

    forEachEnumValue([&result, value](const EnumValueInfo &enumValue)->bool {
        if (enumValue.value() == value)
        {
            result = &enumValue;
            return false;
        }

        return true;
    });

    return result;
}

const EnumValueInfo *EnumInfo::findByName(const QString &name) const
{
    const EnumValueInfo *result = 0;

    forEachEnumValue([&result, name](const EnumValueInfo &enumValue)->bool {
        if (enumValue.name() == name)
        {
            result = &enumValue;
            return false;
        }

        return true;
    });

    return result;
}

} // end namespace Qtinuum
