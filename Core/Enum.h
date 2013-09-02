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

#ifndef QTN_ENUM_H
#define QTN_ENUM_H

#include "CoreAPI.h"
#include <QVector>
#include <QSharedPointer>

namespace Qtinuum
{

typedef int EnumValueType;

enum EnumValueStateFlag
{
    EnumValueStateNone = 0x0000,
    EnumValueStateHidden = 0x0001,
    EnumValueStateObsolete = 0x0002,
    EnumValueStateInvalid = 0x0004
};
Q_DECLARE_FLAGS(EnumValueState, EnumValueStateFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(EnumValueState)

class QTN_PE_CORE_EXPORT EnumValueInfo
{
public:
    EnumValueInfo()
        : m_value(0),
          m_state(EnumValueStateInvalid)
    {
    }

    EnumValueInfo(EnumValueType value, const QString &name, EnumValueState state = EnumValueStateNone)
        : m_value(value),
          m_name(name),
          m_state(state)
    {
    }

    ~EnumValueInfo() {}

    EnumValueType value() const { return m_value; }
    const QString &name() const { return m_name; }
    EnumValueState state() const { return m_state; }

private:
    EnumValueType m_value;
    QString m_name;
    EnumValueState m_state;
};

class QTN_PE_CORE_EXPORT EnumInfo
{
public:
    EnumInfo(QVector<EnumValueInfo> &staticValues);

    template <typename Pred>
    bool forEachEnumValue(Pred pred) const
    {
        foreach(const EnumValueInfo &value, m_staticValues)
        {
            if (!pred(value))
                return false;
        }

        foreach(const EnumValueInfo &value, m_dynamicValues)
        {
            if (!pred(value))
                return false;
        }
        return true;
    }

    const EnumValueInfo *findByValue(EnumValueType value) const;
    const EnumValueInfo *findByName(const QString &name) const;

private:
    QVector<EnumValueInfo> m_staticValues;
    QVector<EnumValueInfo> m_dynamicValues;
};

} // end namespace Qtinuum

#endif // QTN_ENUM_H
