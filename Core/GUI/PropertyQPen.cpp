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
    auto str2PenStyle = QMap<QString, Qt::PenStyle>({
        {"NoPen", Qt::NoPen},
        {"SolidLine", Qt::SolidLine},
        {"DashLine", Qt::DashLine},
        {"DotLine", Qt::DotLine},
        {"DashDotLine", Qt::DashDotLine},
        {"DashDotDotLine", Qt::DashDotDotLine},
        {"CustomDashLine", Qt::CustomDashLine}
    });
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

QtnEnumInfo* QtnPropertyQPenBase::penStyleEnum()
{
    static QtnEnumInfo* enumInfo = nullptr;
    if (!enumInfo)
    {
        QVector<QtnEnumValueInfo> items;
        items.append(QtnEnumValueInfo(Qt::NoPen, "NoPen"));
        items.append(QtnEnumValueInfo(Qt::SolidLine, "SolidLine"));
        items.append(QtnEnumValueInfo(Qt::DashLine, "DashLine"));
        items.append(QtnEnumValueInfo(Qt::DotLine, "DotLine"));
        items.append(QtnEnumValueInfo(Qt::DashDotLine, "DashDotLine"));
        items.append(QtnEnumValueInfo(Qt::DashDotDotLine, "DashDotDotLine"));
        enumInfo = new QtnEnumInfo("Qt", items);
    }

    return enumInfo;
}

QtnEnumInfo* QtnPropertyQPenBase::penCapStyleEnum()
{
    static QtnEnumInfo* enumInfo = nullptr;
    if (!enumInfo)
    {
        QVector<QtnEnumValueInfo> items;
        items.append(QtnEnumValueInfo(Qt::FlatCap, "FlatCap"));
        items.append(QtnEnumValueInfo(Qt::SquareCap, "SquareCap"));
        items.append(QtnEnumValueInfo(Qt::RoundCap, "RoundCap"));
        enumInfo = new QtnEnumInfo("Qt", items);
    }

    return enumInfo;
}

QtnEnumInfo* QtnPropertyQPenBase::penJoinStyleEnum()
{
    static QtnEnumInfo* enumInfo = nullptr;
    if (!enumInfo)
    {
        QVector<QtnEnumValueInfo> items;
        items.append(QtnEnumValueInfo(Qt::MiterJoin, "MiterJoin"));
        items.append(QtnEnumValueInfo(Qt::BevelJoin, "BevelJoin"));
        items.append(QtnEnumValueInfo(Qt::RoundJoin, "RoundJoin"));
        items.append(QtnEnumValueInfo(Qt::SvgMiterJoin, "SvgMiterJoin"));
        enumInfo = new QtnEnumInfo("Qt", items);
    }

    return enumInfo;
}

bool QtnPropertyQPenBase::penFromStr(const QString& str, QPen& pen)
{
    QStringList penParts = str.split(',');
    if (penParts.size() != 5)
        return false;

    QColor color;
    if (!QtnPropertyQColorBase::colorFromStr(penParts[0], color))
        return false;

    Qt::PenStyle style = Qt::NoPen;
    auto styleEnum = penStyleEnum()->fromStr(penParts[1]);
    if (!styleEnum)
        return false;
    style = (Qt::PenStyle)styleEnum->value();

    bool ok = false;
    int width = penParts[2].trimmed().toInt(&ok);
    if (!ok)
        return false;

    Qt::PenCapStyle capStyle = Qt::FlatCap;
    auto capStyleEnum = penCapStyleEnum()->fromStr(penParts[3]);
    if (!capStyleEnum)
        return false;
    capStyle = (Qt::PenCapStyle)capStyleEnum->value();

    Qt::PenJoinStyle joinStyle = Qt::MiterJoin;
    auto joinStyleEnum = penJoinStyleEnum()->fromStr(penParts[4]);
    if (!joinStyleEnum)
        return false;
    joinStyle = (Qt::PenJoinStyle)joinStyleEnum->value();

    pen.setColor(color);
    pen.setStyle(style);
    pen.setWidth(width);
    pen.setCapStyle(capStyle);
    pen.setJoinStyle(joinStyle);

    return true;
}

bool QtnPropertyQPenBase::strFromPen(const QPen& pen, QString& str)
{
    QString color;
    QtnPropertyQColorBase::strFromColor(pen.color(), color);
    QString style;
    penStyleEnum()->toStr(style, pen.style());
    QString width = QString("%1").arg(pen.width());
    QString capStyle;
    penCapStyleEnum()->toStr(capStyle, pen.capStyle());
    QString joinStyle;
    penJoinStyleEnum()->toStr(joinStyle, pen.joinStyle());
    str = QString("%1, %2, %3, %4, %5").arg(color, style, width, capStyle, joinStyle);
    return true;
}

bool QtnPropertyQPenBase::fromStrImpl(const QString& str)
{
    QPen pen;
    if (!penFromStr(str, pen))
        return false;

    setValue(pen);
    return true;
}

bool QtnPropertyQPenBase::toStrImpl(QString& str) const
{
    return strFromPen(value(), str);
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
