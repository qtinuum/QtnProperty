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

#ifndef PROPERTYENUMFLAGS_H
#define PROPERTYENUMFLAGS_H

#include "../Auxiliary/PropertyTemplates.h"
#include "../Enum.h"

// not unsigned to disable warnings
typedef /*unsigned*/ qint32 QtnEnumFlagsValueType;

class QTN_PE_CORE_EXPORT QtnPropertyEnumFlagsBase: public QtnSinglePropertyBase<QtnEnumFlagsValueType>
{
    Q_OBJECT
    QtnPropertyEnumFlagsBase(const QtnPropertyEnumFlagsBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumFlagsBase(QObject* parent);

    const QtnEnumInfo* enumInfo() const { return m_enumInfo; }
    void setEnumInfo(const QtnEnumInfo* enumInfo) { m_enumInfo = enumInfo; }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

private:
    const QtnEnumInfo* m_enumInfo;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyEnumFlagsBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyEnumFlagsBase, QtnEnumFlagsValueType)

class QTN_PE_CORE_EXPORT QtnPropertyEnumFlagsCallback: public QtnSinglePropertyCallback<QtnPropertyEnumFlagsBase>
{
    Q_OBJECT
    QtnPropertyEnumFlagsCallback(const QtnPropertyEnumFlagsCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumFlagsCallback(QObject* parent)
        : QtnSinglePropertyCallback<QtnPropertyEnumFlagsBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyEnumFlagsCallback, QtnPropertyEnumFlagsBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyEnumFlags: public QtnSinglePropertyValue<QtnPropertyEnumFlagsBase>
{
    Q_OBJECT
    QtnPropertyEnumFlags(const QtnPropertyEnumFlags& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumFlags(QObject* parent)
        : QtnSinglePropertyValue<QtnPropertyEnumFlagsBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyEnumFlags, QtnPropertyEnumFlagsBase)
};

#endif // PROPERTYENUMFLAGS_H
