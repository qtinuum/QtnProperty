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

#ifndef QTN_PROPERTY_DELEGATE_INFO_H
#define QTN_PROPERTY_DELEGATE_INFO_H

#include "../CoreAPI.h"
#include <QMap>
#include <QVariant>

typedef QMap<QByteArray, QVariant> QtnPropertyDelegateAttributes;

struct QTN_PE_CORE_EXPORT QtnPropertyDelegateInfo
{
    QByteArray name;
    QtnPropertyDelegateAttributes attributes;

    QtnPropertyDelegateInfo(const QByteArray& name = {})
        : name(name)
    {
    }

    QtnPropertyDelegateInfo(const QtnPropertyDelegateInfo& other)
        : name(other.name),
          attributes(other.attributes)
    {
    }
};

template <typename T>
bool qtnGetAttribute(const QtnPropertyDelegateAttributes& attributes, const QByteArray& attributeName, T& attributeValue)
{
    auto it = attributes.find(attributeName);
    if (it == attributes.end())
        return false;

    attributeValue = it.value().value<T>();
    return true;
}

#endif // QTN_PROPERTY_DELEGATE_INFO_H
