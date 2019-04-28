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

#ifndef PROPERTYENUM_H
#define PROPERTYENUM_H

#include "../Auxiliary/PropertyTemplates.h"
#include "../Enum.h"

class QTN_PE_CORE_EXPORT QtnPropertyEnumBase: public QtnSinglePropertyBase<QtnEnumValueType>
{
    Q_OBJECT
    QtnPropertyEnumBase(const QtnPropertyEnumBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumBase(QObject* parent)
        : QtnSinglePropertyBase<QtnEnumValueType>(parent),
          m_enumInfo(nullptr)
    {
    }

    const QtnEnumInfo* enumInfo() const { return m_enumInfo; }
    void setEnumInfo(const QtnEnumInfo* enumInfo) { m_enumInfo = enumInfo; }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    bool isValueAcceptedImpl(ValueType valueToAccept) override;

private:
    const QtnEnumInfo* m_enumInfo;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyEnumBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyEnumBase, QtnEnumValueType)

class QTN_PE_CORE_EXPORT QtnPropertyEnumCallback: public QtnSinglePropertyCallback<QtnPropertyEnumBase>
{
    Q_OBJECT
    QtnPropertyEnumCallback(const QtnPropertyEnumCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumCallback(QObject* parent)
        : QtnSinglePropertyCallback<QtnPropertyEnumBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyEnumCallback, QtnPropertyEnumBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyEnum: public QtnSinglePropertyValue<QtnPropertyEnumBase>
{
    Q_OBJECT
    QtnPropertyEnum(const QtnPropertyEnum& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnum(QObject* parent)
        : QtnSinglePropertyValue<QtnPropertyEnumBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyEnum, QtnPropertyEnumBase)
};

#endif // PROPERTYENUM_H
