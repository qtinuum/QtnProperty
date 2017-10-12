/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#ifndef QTN_QOBJECT_PROPERTY_SET_H
#define QTN_QOBJECT_PROPERTY_SET_H

#include "CoreAPI.h"
#include "Auxiliary/PropertyAux.h"

#include <QMetaProperty>
#include <QVariant>

#include <functional>
#include <set>

class QtnPropertyBase;
class QtnProperty;
class QtnPropertySet;
struct QtnPropertyDelegateInfo;

typedef std::function<QtnProperty * (QObject *, const QMetaProperty &)>
	QtnMetaPropertyFactory_t;
QTN_IMPORT_EXPORT bool qtnRegisterMetaPropertyFactory(
	int metaPropertyType,
	const QtnMetaPropertyFactory_t &factory, bool force = false);
QTN_IMPORT_EXPORT void qtnInitPercentSpinBoxDelegate(
	QtnPropertyDelegateInfo &delegate);
QTN_IMPORT_EXPORT void qtnInitDegreeSpinBoxDelegate(
	QtnPropertyDelegateInfo &delegate);

QTN_IMPORT_EXPORT QtnProperty *qtnCreateQObjectProperty(
	QObject *object, const QMetaProperty &metaProperty,
	bool connect = false, const char *className = nullptr);
QTN_IMPORT_EXPORT QtnProperty *qtnCreateQObjectProperty(
	QObject *object, const char *propertyName, bool connect = false);
QTN_IMPORT_EXPORT QtnPropertySet *qtnCreateQObjectPropertySet(QObject *object);
QTN_IMPORT_EXPORT QtnPropertySet *qtnCreateQObjectMultiPropertySet(
	const std::set<QObject *> &objects);

QTN_IMPORT_EXPORT QtnPropertyState qtnPropertyStateToAdd(
	const QMetaProperty &metaProperty);
QTN_IMPORT_EXPORT void qtnUpdatePropertyState(
	QtnPropertyBase *property, const QMetaProperty &metaProperty);

template <typename PropertyCallbackType,
		  typename ValueType = typename PropertyCallbackType::ValueTypeStore>
QtnMetaPropertyFactory_t qtnCreateFactory()
{
	typedef typename PropertyCallbackType::ValueType CallbackValueType;
	typedef typename PropertyCallbackType::ValueTypeStore
		CallbackValueTypeStore;

	auto result = [](QObject *object,
					 const QMetaProperty &metaProperty) -> QtnProperty *
		{
			auto property = new PropertyCallbackType(nullptr);

			property->setCallbackValueGet(
				[object, metaProperty]() -> CallbackValueType
		{
			auto variantValue = metaProperty.read(object);
			return CallbackValueTypeStore(variantValue.value<ValueType>());
		});

			property->setCallbackValueSet(
				[object, metaProperty](CallbackValueType value)
		{
			auto variantValue = QVariant::fromValue<ValueType>(
					ValueType(value));
			metaProperty.write(object, variantValue);
		});

			property->setCallbackValueAccepted(
				[property](CallbackValueType) -> bool
		{
			return property->isEditableByUser();
		});

			return property;
		};
	return result;
}

#endif	// QTN_QOBJECT_PROPERTY_SET_H
