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

#include "PropertyQPen.h"
#include <QMap>

static QMap<QString, Qt::PenStyle> CreateStr2PenStyle()
{
    QMap<QString, Qt::PenStyle> str2PenStyle = {
        {"NoPen", Qt::NoPen},
        {"SolidLine", Qt::SolidLine},
        {"DashLine", Qt::DashLine},
        {"DotLine", Qt::DotLine},
        {"DashDotLine", Qt::DashDotLine},
        {"DashDotDotLine", Qt::DashDotDotLine},
        {"CustomDashLine", Qt::CustomDashLine}
    };
    return str2PenStyle;
}

static QMap<Qt::PenStyle, QString> CreatePenStyle2Str()
{
    QMap<Qt::PenStyle, QString> penStyle2Str;
    for (const auto& val : CreateStr2PenStyle().toStdMap())
        penStyle2Str[val.second] = val.first;

    return penStyle2Str;
}

QtnPropertyQPenStyleBase::QtnPropertyQPenStyleBase(QObject *parent)
    : QtnSinglePropertyBase<Qt::PenStyle>(parent)
{
}

bool QtnPropertyQPenStyleBase::fromStrImpl(const QString& str)
{
    static auto str2PenStyle = CreateStr2PenStyle();
    auto it = str2PenStyle.find(str.toLower());
    if (it != str2PenStyle.end())
    {
        setValue(it.value());
        return true;
    }

    return false;
}

bool QtnPropertyQPenStyleBase::toStrImpl(QString& str) const
{
    static auto penStyle2Str = CreatePenStyle2Str();
    auto it = penStyle2Str.find(value());
    if (it != penStyle2Str.end())
    {
        str = it.value();
        return true;
    }

    return false;
}

QDataStream& operator<< (QDataStream& stream, Qt::PenStyle penStyle)
{
    stream << (qint32)penStyle;
    return stream;
}

QDataStream& operator>> (QDataStream& stream, Qt::PenStyle& penStyle)
{
    qint32 value = 0;
    stream >> value;
    penStyle = (Qt::PenStyle)value;
    return stream;
}

QtnPropertyQPenBase::QtnPropertyQPenBase(QObject *parent)
    : QtnSinglePropertyBase<QPen>(parent)
{
}

bool QtnPropertyQPenBase::fromStrImpl(const QString& str)
{
/*
    QPen pen;
    if (!pen.fromString(str.trimmed()))
        return false;

    setValue(pen);
    return true;
    */
    return false;
}

bool QtnPropertyQPenBase::toStrImpl(QString& str) const
{
    /*
    QPen v = value();
    str = v.toString();
    return true;
    */
    return false;
}

bool QtnPropertyQPenBase::fromVariantImpl(const QVariant& var)
{
    if (!var.canConvert<QPen>())
        return false;

    setValue(var.value<QPen>());
    return true;
}

bool QtnPropertyQPenBase::toVariantImpl(QVariant& var) const
{
    QPen pen = value();
    var = pen;
    return true;
}
