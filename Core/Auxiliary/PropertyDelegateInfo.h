/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
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

    QtnPropertyDelegateInfo()
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
