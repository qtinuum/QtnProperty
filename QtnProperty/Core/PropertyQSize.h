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

#ifndef PROPERTYQSIZE_H
#define PROPERTYQSIZE_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QSize>

class QTN_PE_CORE_EXPORT QtnPropertyQSizeBase: public QtnSinglePropertyBase<QSize>
{
    Q_OBJECT
    QtnPropertyQSizeBase(const QtnPropertyQSizeBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQSizeBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQSizeBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQSizeBase, QSize)

QTN_PE_CORE_EXPORT QtnProperty* qtnCreateWidthProperty(QObject* parent, QtnPropertyQSizeBase* propertySize);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateHeightProperty(QObject* parent, QtnPropertyQSizeBase* propertySize);

class QTN_PE_CORE_EXPORT QtnPropertyQSizeCallback: public QtnSinglePropertyCallback<QtnPropertyQSizeBase>
{
    Q_OBJECT
    QtnPropertyQSizeCallback(const QtnPropertyQSizeCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQSizeCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQSizeBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSizeCallback, QtnPropertyQSizeBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQSize: public QtnSinglePropertyValue<QtnPropertyQSizeBase>
{
    Q_OBJECT
    QtnPropertyQSize(const QtnPropertyQSize& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQSize(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQSizeBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSize, QtnPropertyQSizeBase)
};

#endif // PROPERTYQSIZE_H
