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

#include "PropertyBool.h"

static QString getBoolText(bool value)
{
    static QString boolTexts[2] = {QObject::tr("False"), QObject::tr("True")};
    return boolTexts[value];
}

static bool getBoolValue(QString boolText, bool& success)
{
    success = true;
    if (QString::compare(boolText, getBoolText(false), Qt::CaseInsensitive) == 0)
        return false;

    if (QString::compare(boolText, getBoolText(true), Qt::CaseInsensitive) == 0)
        return true;

    success = false;
    return false;
}

bool QtnPropertyBoolBase::fromStrImpl(const QString& str)
{
    bool success = false;
    bool value = getBoolValue(str.trimmed(), success);

    if (!success)
        return false;

    setValue(value);
    return true;
}

bool QtnPropertyBoolBase::toStrImpl(QString& str) const
{
    bool boolValue = value();
    str = getBoolText(boolValue);
    return true;
}

