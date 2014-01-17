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

typedef qint32 QtnEnumValueType;

enum QtnEnumValueStateFlag
{
    QtnEnumValueStateNone = 0x0000,
    QtnEnumValueStateHidden = 0x0001,
    QtnEnumValueStateObsolete = 0x0002,
    QtnEnumValueStateInvalid = 0x0004
};
Q_DECLARE_FLAGS(QtnEnumValueState, QtnEnumValueStateFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtnEnumValueState)

class QTN_PE_CORE_EXPORT QtnEnumValueInfo
{
public:
    QtnEnumValueInfo()
        : m_value(0),
          m_state(QtnEnumValueStateInvalid)
    {
    }

    QtnEnumValueInfo(QtnEnumValueType value, const QString& name, QtnEnumValueState state = QtnEnumValueStateNone)
        : m_value(value),
          m_name(name),
          m_state(state)
    {
    }

    ~QtnEnumValueInfo() {}

    QtnEnumValueType value() const { return m_value; }
    const QString& name() const { return m_name; }
    QtnEnumValueState state() const { return m_state; }

private:
    QtnEnumValueType m_value;
    QString m_name;
    QtnEnumValueState m_state;
};

class QTN_PE_CORE_EXPORT QtnEnumInfo
{
public:
    QtnEnumInfo(const QString& name, QVector<QtnEnumValueInfo>& staticValues);

    const QString& name() const { return m_name; }

    template <typename Pred>
    bool forEachEnumValue(Pred pred) const
    {
        foreach(const QtnEnumValueInfo& value, m_staticValues)
        {
            if (!pred(value))
                return false;
        }

        foreach(const QtnEnumValueInfo& value, m_dynamicValues)
        {
            if (!pred(value))
                return false;
        }
        return true;
    }

    const QtnEnumValueInfo* findByValue(QtnEnumValueType value) const;
    const QtnEnumValueInfo* findByName(const QString& name, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    const QtnEnumValueInfo* fromStr(const QString& str) const;
    bool toStr(QString& str, const QtnEnumValueInfo* value) const;

private:
    QString m_name;
    QVector<QtnEnumValueInfo> m_staticValues;
    QVector<QtnEnumValueInfo> m_dynamicValues;
};

#endif // QTN_ENUM_H
