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

#include "../CoreAPI.h"
#include <QtCore/QObject>

// forward declarations
class QtnPropertyBase;
class QtnProperty;
class QtnPropertySet;

enum QtnPropertyStateFlag
{
    QtnPropertyStateNone = 0x0000,
    QtnPropertyStateNonSimple = 0x0001,
    QtnPropertyStateInvisible = 0x0002,
    QtnPropertyStateImmutable = 0x0004,
    QtnPropertyStateCollapsed = 0x0008,
    QtnPropertyStateNonSerialized = 0x0010
};
Q_DECLARE_FLAGS(QtnPropertyState, QtnPropertyStateFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtnPropertyState)

enum QtnPropertyChangeReasonFlag
{
    QtnPropertyChangeReasonNewValue = 0x0001,
    QtnPropertyChangeReasonLoadedValue = 0x0002,
    QtnPropertyChangeReasonValue = QtnPropertyChangeReasonNewValue|QtnPropertyChangeReasonLoadedValue,
    QtnPropertyChangeReasonName = 0x0004,
    QtnPropertyChangeReasonDescription = 0x0008,
    QtnPropertyChangeReasonId = 0x0010,
    QtnPropertyChangeReasonStateLocal = 0x0020,
    QtnPropertyChangeReasonStateInherited = 0x0040,
    QtnPropertyChangeReasonState = QtnPropertyChangeReasonStateLocal|QtnPropertyChangeReasonStateInherited,
    QtnPropertyChangeReasonChildPropertyAdd = 0x0080,
    QtnPropertyChangeReasonChildPropertyRemove = 0x0100,
    QtnPropertyChangeReasonChildren = QtnPropertyChangeReasonChildPropertyAdd|QtnPropertyChangeReasonChildPropertyRemove
};
Q_DECLARE_FLAGS(QtnPropertyChangeReason, QtnPropertyChangeReasonFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtnPropertyChangeReason)

typedef qint32 QtnPropertyID;
const QtnPropertyID QtnPropertyIDInvalid = -1;
typedef void* QtnPropertyValuePtr;

template<typename T>
inline T* qtnCastPropertyValue(QtnPropertyValuePtr value) { return reinterpret_cast<T*>(value); }
template<typename T>
inline const T* qtnConstCastPropertyValue(QtnPropertyValuePtr value) { return reinterpret_cast<const T*>(value); }

Q_DECLARE_METATYPE(QtnPropertyID)
Q_DECLARE_METATYPE(QtnPropertyState)
Q_DECLARE_METATYPE(QtnPropertyChangeReason)
Q_DECLARE_METATYPE(QtnPropertyValuePtr)

#endif // QTN_PROPERTY_AUX_H
