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

#ifndef PROPERTYCOLOR_H
#define PROPERTYCOLOR_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QtGui/QColor>

enum QtnColorDelegateShape
{
    QtnColorDelegateShapeNone = 0x1,
    QtnColorDelegateShapeSquare = 0x2,
    QtnColorDelegateShapeCircle = 0x3
};

class QTN_PE_CORE_EXPORT QtnPropertyQColorBase: public QtnSinglePropertyBase<QColor>
{
    Q_OBJECT
    QtnPropertyQColorBase(const QtnPropertyQColorBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQColorBase(QObject *parent)
        : QtnSinglePropertyBase<QColor>(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQColorBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQColorBase, QColor)

class QTN_PE_CORE_EXPORT QtnPropertyQColorCallback: public QtnSinglePropertyCallback<QtnPropertyQColorBase>
{
    Q_OBJECT
    QtnPropertyQColorCallback(const QtnPropertyQColorCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQColorCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQColorBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQColorCallback, QtnPropertyQColorBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQColor: public QtnSinglePropertyValue<QtnPropertyQColorBase>
{
    Q_OBJECT
    QtnPropertyQColor(const QtnPropertyQColor& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQColor(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQColorBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQColor, QtnPropertyQColorBase)
};

#endif // PROPERTYCOLOR_H
