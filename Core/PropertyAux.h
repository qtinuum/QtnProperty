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

#ifndef QTN_PROPERTY_AUX_H
#define QTN_PROPERTY_AUX_H

#include "CoreAPI.h"
#include <QtCore/QObject>
#include <QMap>
#include <QVariant>

namespace Qtinuum
{

enum PropertyStateFlag
{
    PropertyStateNone = 0x0000,
    PropertyStateNonSimple = 0x0001,
    PropertyStateInvisible = 0x0002,
    PropertyStateImmutable = 0x0004,
    PropertyStateCollapsed = 0x0008,
    PropertyStateNonSerialized = 0x0010
};
Q_DECLARE_FLAGS(PropertyState, PropertyStateFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(PropertyState)

enum PropertyChangeReasonFlag
{
    PropertyChangeReasonNewValue = 0x0001,
    PropertyChangeReasonLoadedValue = 0x0002,
    PropertyChangeReasonValue = PropertyChangeReasonNewValue|PropertyChangeReasonLoadedValue,
    PropertyChangeReasonName = 0x0004,
    PropertyChangeReasonDescription = 0x0008,
    PropertyChangeReasonId = 0x0010,
    PropertyChangeReasonStateLocal = 0x0020,
    PropertyChangeReasonStateInherited = 0x0040,
    PropertyChangeReasonState = PropertyChangeReasonStateLocal|PropertyChangeReasonStateInherited,
    PropertyChangeReasonChildPropertyAdd = 0x0080,
    PropertyChangeReasonChildPropertyRemove = 0x0100,
    PropertyChangeReasonChildren = PropertyChangeReasonChildPropertyAdd|PropertyChangeReasonChildPropertyRemove
};
Q_DECLARE_FLAGS(PropertyChangeReason, PropertyChangeReasonFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(PropertyChangeReason)

typedef qint32 PropertyID;
const PropertyID PropertyIDInvalid = -1;
typedef void * PropertyValuePtr;

template<typename T>
static T *CastPropertyValue(PropertyValuePtr value) { return reinterpret_cast<T *>(value); }
template<typename T>
static const T *ConstCastPropertyValue(PropertyValuePtr value) { return reinterpret_cast<const T *>(value); }

typedef QMap<QByteArray, QVariant> PropertyDelegateAttributes;

struct QTN_PE_CORE_EXPORT PropertyDelegateInfo
{
    QByteArray name;
    PropertyDelegateAttributes attributes;

    PropertyDelegateInfo()
    {
    }

    PropertyDelegateInfo(const PropertyDelegateInfo &other)
        : name(other.name),
          attributes(other.attributes)
    {
    }
};

template <typename T>
bool getAttribute(const PropertyDelegateAttributes &attributes, const QByteArray &attributeName, T &attributeValue)
{
    auto it = attributes.find(attributeName);
    if (it == attributes.end())
        return false;

    attributeValue = it.value().value<T>();
    return true;
}

} // end namespace Qtinuum

#endif // QTN_PROPERTY_AUX_H
