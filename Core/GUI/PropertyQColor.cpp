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

#include "PropertyQColor.h"

bool QtnPropertyQColorBase::fromStrImpl(const QString& str)
{
    QColor color(str.trimmed());
    if (!color.isValid())
        return false;

    return setValue(color);
}

bool QtnPropertyQColorBase::toStrImpl(QString& str) const
{
    QColor v = value();

    if (v.alpha() < 255)
        str = v.name(QColor::HexArgb);
    else
        str = v.name();

    return true;
}
