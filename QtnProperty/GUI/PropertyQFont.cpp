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

#include "PropertyQFont.h"

QtnPropertyQFontBase::QtnPropertyQFontBase(QObject *parent)
    : QtnSinglePropertyBase<QFont>(parent)
{
}

bool QtnPropertyQFontBase::fromStrImpl(const QString& str)
{
    QFont font;
    if (!font.fromString(str.trimmed()))
        return false;

    setValue(font);
    return true;
}

bool QtnPropertyQFontBase::toStrImpl(QString& str) const
{
    QFont v = value();
    str = v.toString();
    return true;
}
