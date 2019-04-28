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

#ifndef PROPERTYUINT_H
#define PROPERTYUINT_H

#include "../Auxiliary/PropertyTemplates.h"

class QTN_PE_CORE_EXPORT QtnPropertyUIntBase: public QtnNumericPropertyBase<QtnSinglePropertyBase<quint32> >
{
    Q_OBJECT
    QtnPropertyUIntBase(const QtnPropertyUIntBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyUIntBase(QObject *parent)
        : QtnNumericPropertyBase<QtnSinglePropertyBase<quint32> >(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyUIntBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyUIntBase, quint32)

class QTN_PE_CORE_EXPORT QtnPropertyUIntCallback: public QtnSinglePropertyCallback<QtnPropertyUIntBase>
{
    Q_OBJECT
    QtnPropertyUIntCallback(const QtnPropertyUIntCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyUIntCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyUIntBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyUIntCallback, QtnPropertyUIntBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyUInt: public QtnSinglePropertyValue<QtnPropertyUIntBase>
{
    Q_OBJECT
    QtnPropertyUInt(const QtnPropertyUInt& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyUInt(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyUIntBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyUInt, QtnPropertyUIntBase)
};

#endif // PROPERTYUINT_H
