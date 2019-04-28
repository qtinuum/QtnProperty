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

#include "PropertyQBrush.h"
#include <QMap>

static QMap<QString, Qt::BrushStyle> CreateStr2BrushStyle()
{
    auto str2BrushStyle = QMap<QString, Qt::BrushStyle>({
        {"NoBrush", Qt::NoBrush},
        {"Solid", Qt::SolidPattern},
        {"Dense1Pattern", Qt::Dense1Pattern},
        {"Dense2Pattern", Qt::Dense2Pattern},
        {"Dense3Pattern", Qt::Dense3Pattern},
        {"Dense4Pattern", Qt::Dense4Pattern},
        {"Dense5Pattern", Qt::Dense5Pattern},
        {"Dense6Pattern", Qt::Dense6Pattern},
        {"Dense7Pattern", Qt::Dense7Pattern},
        {"HorPattern", Qt::HorPattern},
        {"VerPattern", Qt::VerPattern},
        {"CrossPattern", Qt::CrossPattern},
        {"BDiagPattern", Qt::BDiagPattern},
        {"FDiagPattern", Qt::FDiagPattern},
        {"DiagCrossPattern", Qt::DiagCrossPattern},
        {"LinearGradientPattern", Qt::LinearGradientPattern},
        {"RadialGradientPattern", Qt::RadialGradientPattern},
        {"ConicalGradientPattern", Qt::ConicalGradientPattern},
        {"TexturePattern", Qt::TexturePattern}
    });
    return str2BrushStyle;
}

static QMap<Qt::BrushStyle, QString> CreateBrushStyle2Str()
{
    QMap<Qt::BrushStyle, QString> brushStyle2Str;
    for (const auto& val : CreateStr2BrushStyle().toStdMap())
        brushStyle2Str[val.second] = val.first;

    return brushStyle2Str;
}

QString QtnPropertyQBrushStyleBase::brushStyleToStr(Qt::BrushStyle brushStyle, bool* ok)
{
    static auto brushStyle2Str = CreateBrushStyle2Str();
    auto it = brushStyle2Str.find(brushStyle);
    if (it != brushStyle2Str.end())
    {
        if (ok) *ok = true;
        return it.value();
    }

    if (ok) *ok = false;
    return "";
}

QtnPropertyQBrushStyleBase::QtnPropertyQBrushStyleBase(QObject *parent)
    : QtnSinglePropertyBase<Qt::BrushStyle>(parent)
{
}

bool QtnPropertyQBrushStyleBase::fromStrImpl(const QString& str)
{
    static auto str2BrushStyle = CreateStr2BrushStyle();
    auto it = str2BrushStyle.find(str.toLower());
    if (it != str2BrushStyle.end())
    {
        setValue(it.value());
        return true;
    }

    return false;
}

bool QtnPropertyQBrushStyleBase::toStrImpl(QString& str) const
{
    bool ok = false;
    auto res = brushStyleToStr(value(), &ok);
    if (ok)
        str = res;
    return ok;
}

QDataStream& operator<< (QDataStream& stream, Qt::BrushStyle brushStyle)
{
    stream << (qint32)brushStyle;
    return stream;
}

QDataStream& operator>> (QDataStream& stream, Qt::BrushStyle& brushStyle)
{
    qint32 value = 0;
    stream >> value;
    brushStyle = (Qt::BrushStyle)value;
    return stream;
}


