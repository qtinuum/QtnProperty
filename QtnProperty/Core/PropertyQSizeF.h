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

#ifndef PROPERTYQSIZEF_H
#define PROPERTYQSIZEF_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QSizeF>

class QTN_PE_CORE_EXPORT QtnPropertyQSizeFBase: public QtnSinglePropertyBase<QSizeF>
{
    Q_OBJECT
    QtnPropertyQSizeFBase(const QtnPropertyQSizeFBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQSizeFBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQSizeFBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQSizeFBase, QSizeF)

QTN_PE_CORE_EXPORT QtnProperty* qtnCreateWidthProperty(QObject* parent, QtnPropertyQSizeFBase* propertySize);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateHeightProperty(QObject* parent, QtnPropertyQSizeFBase* propertySize);

class QTN_PE_CORE_EXPORT QtnPropertyQSizeFCallback: public QtnSinglePropertyCallback<QtnPropertyQSizeFBase>
{
    Q_OBJECT
    QtnPropertyQSizeFCallback(const QtnPropertyQSizeFCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQSizeFCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQSizeFBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSizeFCallback, QtnPropertyQSizeFBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQSizeF: public QtnSinglePropertyValue<QtnPropertyQSizeFBase>
{
    Q_OBJECT
    QtnPropertyQSizeF(const QtnPropertyQSizeF& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQSizeF(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQSizeFBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSizeF, QtnPropertyQSizeFBase)
};

#endif // PROPERTYQSIZEF_H
