/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#ifndef QTN_PROPERTY_AUX_H
#define QTN_PROPERTY_AUX_H

#include "QtnProperty/CoreAPI.h"
#include <QObject>

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
	QtnPropertyStateNonSerialized = 0x0010,
	QtnPropertyStateMultiValue = 0x0020,
	QtnPropertyStateModifiedValue = 0x0040,
	QtnPropertyStateIgnoreDirectParentState = 0x80,
	QtnPropertyStateResettable = 0x100
};

Q_DECLARE_FLAGS(QtnPropertyState, QtnPropertyStateFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtnPropertyState)

enum QtnPropertyChangeReasonFlag
{
	QtnPropertyChangeReasonNewValue = 0x0001,
	QtnPropertyChangeReasonLoadedValue = 0x0002,
	QtnPropertyChangeReasonName = 0x0004,
	QtnPropertyChangeReasonDisplayName = 0x0008,
	QtnPropertyChangeReasonDescription = 0x0010,
	QtnPropertyChangeReasonId = 0x0020,
	QtnPropertyChangeReasonStateLocal = 0x0040,
	QtnPropertyChangeReasonStateInherited = 0x0080,
	QtnPropertyChangeReasonChildPropertyAdd = 0x0100,
	QtnPropertyChangeReasonChildPropertyRemove = 0x0200,
	QtnPropertyChangeReasonEditValue = 0x0400,
	QtnPropertyChangeReasonResetValue = 0x0800,
	QtnPropertyChangeReasonEditMultiValue = 0x1000,
	QtnPropertyChangeReasonState = QtnPropertyChangeReasonStateLocal |
		QtnPropertyChangeReasonStateInherited,
	QtnPropertyChangeReasonChildren = QtnPropertyChangeReasonChildPropertyAdd |
		QtnPropertyChangeReasonChildPropertyRemove,
	QtnPropertyChangeReasonValue = QtnPropertyChangeReasonNewValue |
		QtnPropertyChangeReasonLoadedValue | QtnPropertyChangeReasonResetValue
};

Q_DECLARE_FLAGS(QtnPropertyChangeReason, QtnPropertyChangeReasonFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtnPropertyChangeReason)

typedef qint32 QtnPropertyID;
enum
{
	QtnPropertyIDInvalid = -1
};

typedef void *QtnPropertyValuePtr;

template <typename T>
inline T *qtnCastPropertyValue(QtnPropertyValuePtr value)
{
	return reinterpret_cast<T *>(value);
}

template <typename T>
inline const T *qtnConstCastPropertyValue(QtnPropertyValuePtr value)
{
	return reinterpret_cast<const T *>(value);
}

Q_DECLARE_METATYPE(QtnPropertyID)
Q_DECLARE_METATYPE(QtnPropertyState)
Q_DECLARE_METATYPE(QtnPropertyChangeReason)
Q_DECLARE_METATYPE(QtnPropertyValuePtr)

#endif // QTN_PROPERTY_AUX_H
