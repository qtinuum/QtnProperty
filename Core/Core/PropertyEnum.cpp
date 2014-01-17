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

#include "PropertyEnum.h"

bool QtnPropertyEnumBase::fromStrImpl(const QString& str)
{
    if (!m_enumInfo)
        return false;

    const QtnEnumValueInfo* enumValue = m_enumInfo->fromStr(str);
    if (!enumValue)
        return false;

    return setValue(enumValue->value());
}

bool QtnPropertyEnumBase::toStrImpl(QString& str) const
{
    if (!m_enumInfo)
        return false;

    QtnEnumValueType v = value();
    const QtnEnumValueInfo* enumValue = m_enumInfo->findByValue(v);
    return m_enumInfo->toStr(str, enumValue);
}

bool QtnPropertyEnumBase::isValueAcceptedImpl(ValueType valueToAccept)
{
    if (!m_enumInfo)
        return false;

    if (!m_enumInfo->findByValue(valueToAccept))
        return false;

    return true;
}
