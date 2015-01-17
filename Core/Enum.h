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
