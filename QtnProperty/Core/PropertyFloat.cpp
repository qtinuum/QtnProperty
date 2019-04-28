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

#include "PropertyFloat.h"

bool QtnPropertyFloatBase::fromStrImpl(const QString& str)
{
    bool ok = false;
    ValueType value = str.toFloat(&ok);
    if (!ok)
        return false;

    setValue(value);
    return true;
}

bool QtnPropertyFloatBase::toStrImpl(QString& str) const
{
    str = QString::number(value());
    return true;
}

bool QtnPropertyFloatBase::fromVariantImpl(const QVariant& var)
{
    bool ok = false;
    ValueType value = var.toFloat(&ok);
    if (!ok)
        return false;

    return setValue(value);
}
