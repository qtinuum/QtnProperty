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

#ifndef PROPERTY_PEN_H
#define PROPERTY_PEN_H

#include "../Auxiliary/PropertyTemplates.h"
#include "../Enum.h"
#include <QtGui/QPen>

Q_DECLARE_METATYPE(Qt::PenStyle)

class QTN_PE_CORE_EXPORT QtnPropertyQPenStyleBase: public QtnSinglePropertyBase<Qt::PenStyle>
{
    Q_OBJECT
    QtnPropertyQPenStyleBase(const QtnPropertyQPenStyleBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPenStyleBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPenStyleBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPenStyleBase, Qt::PenStyle)
QTN_PE_CORE_EXPORT QDataStream& operator<< (QDataStream& stream, Qt::PenStyle penStyle);
QTN_PE_CORE_EXPORT QDataStream& operator>> (QDataStream& stream, Qt::PenStyle& penStyle);


class QTN_PE_CORE_EXPORT QtnPropertyQPenStyleCallback: public QtnSinglePropertyCallback<QtnPropertyQPenStyleBase>
{
    Q_OBJECT
    QtnPropertyQPenStyleCallback(const QtnPropertyQPenStyleCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPenStyleCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQPenStyleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPenStyleCallback, QtnPropertyQPenStyleBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQPenStyle: public QtnSinglePropertyValue<QtnPropertyQPenStyleBase>
{
    Q_OBJECT
    QtnPropertyQPenStyle(const QtnPropertyQPenStyle& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPenStyle(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQPenStyleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPenStyle, QtnPropertyQPenStyleBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQPenBase: public QtnSinglePropertyBase<QPen>
{
    Q_OBJECT
    QtnPropertyQPenBase(const QtnPropertyQPenBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPenBase(QObject *parent);

    static QtnEnumInfo* penStyleEnum();
    static QtnEnumInfo* penCapStyleEnum();
    static QtnEnumInfo* penJoinStyleEnum();
    static bool penFromStr(const QString& str, QPen& pen);
    static bool strFromPen(const QPen& pen, QString& str);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override;
    bool toVariantImpl(QVariant& var) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPenBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPenBase, QPen)

class QTN_PE_CORE_EXPORT QtnPropertyQPenCallback: public QtnSinglePropertyCallback<QtnPropertyQPenBase>
{
    Q_OBJECT
    QtnPropertyQPenCallback(const QtnPropertyQPenCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPenCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQPenBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPenCallback, QtnPropertyQPenBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQPen: public QtnSinglePropertyValue<QtnPropertyQPenBase>
{
    Q_OBJECT
    QtnPropertyQPen(const QtnPropertyQPen& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPen(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQPenBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPen, QtnPropertyQPenBase)
};

#endif // PROPERTY_PEN_H
