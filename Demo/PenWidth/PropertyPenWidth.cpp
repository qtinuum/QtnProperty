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

#include "PropertyPenWidth.h"

QDataStream& operator<< (QDataStream& stream, const PenWidth penWidth)
{
    stream << (qint64)penWidth;
    return stream;
}

QDataStream& operator>> (QDataStream& stream, PenWidth& penWidth)
{
    qint64 data = 0;
    stream >> data;

    penWidth = (PenWidth)data;

    return stream;
}

bool QtnPropertyPenWidthBase::fromStrImpl(const QString& str)
{
    static QMap<QString, PenWidth> str2PenWidth = {{"Default", PenWidth::Default},
                                                   {"Thin", PenWidth::Thin},
                                                   {"Middle", PenWidth::Middle},
                                                   {"Thick", PenWidth::Thick}};

    setValue(str2PenWidth.value(str, PenWidth::Default));
    return true;
}

bool QtnPropertyPenWidthBase::toStrImpl(QString& str) const
{
    switch (value()) {
    case PenWidth::Default:
        str = "Default";
        return true;
    case PenWidth::Thin:
        str = "Thin";
        return true;
    case PenWidth::Middle:
        str = "Middle";
        return true;
    case PenWidth::Thick:
        str = "Thick";
        return true;
    default:
        return false;
    }
}
