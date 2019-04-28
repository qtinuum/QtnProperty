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

#ifndef PROPERTYQRECTF_H
#define PROPERTYQRECTF_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QRectF>

class QTN_PE_CORE_EXPORT QtnPropertyQRectFBase: public QtnSinglePropertyBase<QRectF>
{
    Q_OBJECT
    QtnPropertyQRectFBase(const QtnPropertyQRectFBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQRectFBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQRectFBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQRectFBase, QRectF)

QTN_PE_CORE_EXPORT QtnProperty* qtnCreateLeftProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateRightProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateTopProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateBottomProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateWidthProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateHeightProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect);

class QTN_PE_CORE_EXPORT QtnPropertyQRectFCallback: public QtnSinglePropertyCallback<QtnPropertyQRectFBase>
{
    Q_OBJECT
    QtnPropertyQRectFCallback(const QtnPropertyQRectFCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQRectFCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQRectFBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRectFCallback, QtnPropertyQRectFBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQRectF: public QtnSinglePropertyValue<QtnPropertyQRectFBase>
{
    Q_OBJECT
    QtnPropertyQRectF(const QtnPropertyQRectF& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQRectF(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQRectFBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRectF, QtnPropertyQRectFBase)
};

#endif // PROPERTYQRECTF_H
