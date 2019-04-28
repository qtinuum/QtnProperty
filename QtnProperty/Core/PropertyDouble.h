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

#ifndef PROPERTYDOUBLE_H
#define PROPERTYDOUBLE_H

#include "../Auxiliary/PropertyTemplates.h"

class QTN_PE_CORE_EXPORT QtnPropertyDoubleBase: public QtnNumericPropertyBase<QtnSinglePropertyBase<double>>
{
    Q_OBJECT
    QtnPropertyDoubleBase(const QtnPropertyDoubleBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyDoubleBase(QObject *parent)
        : QtnNumericPropertyBase<QtnSinglePropertyBase<double>>(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyDoubleBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyDoubleBase, double)

class QTN_PE_CORE_EXPORT QtnPropertyDoubleCallback: public QtnSinglePropertyCallback<QtnPropertyDoubleBase>
{
    Q_OBJECT
    QtnPropertyDoubleCallback(const QtnPropertyDoubleCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyDoubleCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyDoubleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyDoubleCallback, QtnPropertyDoubleBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyDouble: public QtnSinglePropertyValue<QtnPropertyDoubleBase>
{
    Q_OBJECT
    QtnPropertyDouble(const QtnPropertyDouble& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyDouble(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyDoubleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyDouble, QtnPropertyDoubleBase)
};

#endif // PROPERTYDOUBLE_H
