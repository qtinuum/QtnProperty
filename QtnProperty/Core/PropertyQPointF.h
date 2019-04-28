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

#ifndef PROPERTYQPOINTF_H
#define PROPERTYQPOINTF_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QPointF>

class QTN_PE_CORE_EXPORT QtnPropertyQPointFBase: public QtnSinglePropertyBase<QPointF>
{
    Q_OBJECT
    QtnPropertyQPointFBase(const QtnPropertyQPointFBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPointFBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPointFBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPointFBase, QPointF)

QTN_PE_CORE_EXPORT QtnProperty* qtnCreateXProperty(QObject *parent, QtnPropertyQPointFBase *propertyPoint);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateYProperty(QObject *parent, QtnPropertyQPointFBase *propertyPoint);

class QTN_PE_CORE_EXPORT QtnPropertyQPointFCallback: public QtnSinglePropertyCallback<QtnPropertyQPointFBase>
{
    Q_OBJECT
    QtnPropertyQPointFCallback(const QtnPropertyQPointFCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPointFCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQPointFBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPointFCallback, QtnPropertyQPointFBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQPointF: public QtnSinglePropertyValue<QtnPropertyQPointFBase>
{
    Q_OBJECT
    QtnPropertyQPointF(const QtnPropertyQPointF& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPointF(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQPointFBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPointF, QtnPropertyQPointFBase)
};

#endif // PROPERTYQPOINTF_H
