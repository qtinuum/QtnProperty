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

#include "PropertyQString.h"

bool QtnPropertyQStringBase::fromStrImpl(const QString& str)
{
    QString strValue = str;

    QString quotedStr = str.trimmed();
    if (quotedStr.size() > 1)
    {
        if (quotedStr.startsWith('"') && quotedStr.endsWith('"'))
        {
            strValue = quotedStr.mid(1, quotedStr.size() - 2);
        }
    }

    setValue(strValue);
    return true;
}

bool QtnPropertyQStringBase::toStrImpl(QString& str) const
{
    str = value();
    return true;
}
