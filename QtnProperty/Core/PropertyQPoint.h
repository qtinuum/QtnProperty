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

#ifndef PROPERTYQPOINT_H
#define PROPERTYQPOINT_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QPoint>

class QTN_PE_CORE_EXPORT QtnPropertyQPointBase: public QtnSinglePropertyBase<QPoint>
{
    Q_OBJECT
    QtnPropertyQPointBase(const QtnPropertyQPointBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPointBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPointBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPointBase, QPoint)

QTN_PE_CORE_EXPORT QtnProperty* qtnCreateXProperty(QObject *parent, QtnPropertyQPointBase *propertyPoint);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateYProperty(QObject *parent, QtnPropertyQPointBase *propertyPoint);

class QTN_PE_CORE_EXPORT QtnPropertyQPointCallback: public QtnSinglePropertyCallback<QtnPropertyQPointBase>
{
    Q_OBJECT
    QtnPropertyQPointCallback(const QtnPropertyQPointCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPointCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQPointBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPointCallback, QtnPropertyQPointBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQPoint: public QtnSinglePropertyValue<QtnPropertyQPointBase>
{
    Q_OBJECT
    QtnPropertyQPoint(const QtnPropertyQPoint& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPoint(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQPointBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPoint, QtnPropertyQPointBase)
};

#endif // PROPERTYQPOINT_H
