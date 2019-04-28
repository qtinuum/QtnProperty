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

#ifndef PROPERTY_BRUSH_H
#define PROPERTY_BRUSH_H

#include "../Auxiliary/PropertyTemplates.h"
#include "../Enum.h"
#include <QtGui/QBrush>

Q_DECLARE_METATYPE(Qt::BrushStyle)

class QTN_PE_CORE_EXPORT QtnPropertyQBrushStyleBase: public QtnSinglePropertyBase<Qt::BrushStyle>
{
    Q_OBJECT
    QtnPropertyQBrushStyleBase(const QtnPropertyQBrushStyleBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQBrushStyleBase(QObject *parent);

    static QString brushStyleToStr(Qt::BrushStyle brushStyle, bool *ok = nullptr);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQBrushStyleBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQBrushStyleBase, Qt::BrushStyle)
QTN_PE_CORE_EXPORT QDataStream& operator<< (QDataStream& stream, Qt::BrushStyle brushStyle);
QTN_PE_CORE_EXPORT QDataStream& operator>> (QDataStream& stream, Qt::BrushStyle& brushStyle);


class QTN_PE_CORE_EXPORT QtnPropertyQBrushStyleCallback: public QtnSinglePropertyCallback<QtnPropertyQBrushStyleBase>
{
    Q_OBJECT
    QtnPropertyQBrushStyleCallback(const QtnPropertyQBrushStyleCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQBrushStyleCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQBrushStyleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQBrushStyleCallback, QtnPropertyQBrushStyleBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQBrushStyle: public QtnSinglePropertyValue<QtnPropertyQBrushStyleBase>
{
    Q_OBJECT
    QtnPropertyQBrushStyle(const QtnPropertyQBrushStyle& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQBrushStyle(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQBrushStyleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQBrushStyle, QtnPropertyQBrushStyleBase)
};


#endif // PROPERTY_BRUSH_H
