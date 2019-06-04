/*******************************************************************************
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

#pragma once

#include "Auxiliary/PropertyTemplates.h"

template <typename FIELD_PROP_T, typename VALUE_T,
	typename FIELD_RET = typename FIELD_PROP_T::ValueType,
	typename FIELD_ARG = FIELD_RET>
FIELD_PROP_T *qtnCreateFieldProperty(QtnSinglePropertyBase<VALUE_T> *property,
	const QString &name, const QString &displayName, const QString &desc_fmt,
	FIELD_RET (VALUE_T::*get)() const, void (VALUE_T::*set)(FIELD_ARG))
{
	using CallbackValueType = typename FIELD_PROP_T::ValueType;
	using CallbackValueTypeStore = typename FIELD_PROP_T::ValueTypeStore;
	using ValueTypeStore =
		typename QtnSinglePropertyBase<VALUE_T>::ValueTypeStore;

	Q_ASSERT(property);

	auto result = new FIELD_PROP_T(nullptr);

	result->setDisplayName(displayName);
	result->setName(name);
	result->setDescription(desc_fmt.arg(property->displayName()));
	result->setCallbackValueGet([property, get]() -> CallbackValueTypeStore {
		return (property->value().*get)();
	});
	result->setCallbackValueSet([property, set](CallbackValueType new_value) {
		auto v = property->value();
		(v.*set)(new_value);
		property->setValue(v);
	});

	if (!property->isQObjectProperty() && property->isResettable())
	{
		result->addState(QtnPropertyStateResettable);
		result->setCallbackValueDefault([property, get]() -> CallbackValueType {
			ValueTypeStore value;
			if (!property->readDefaultValue(value))
			{
				value = property->value();
			}
			return (value.*get)();
		});
	}

	auto delegateInfoCallback = [property]() -> QtnPropertyDelegateInfo {
		auto baseDelegate = property->delegateInfo();
		QtnPropertyDelegateInfo result;
		if (baseDelegate)
		{
			result.attributes = baseDelegate->attributes;
			auto it = result.attributes.find(qtnFieldDelegateNameAttr());
			if (it != result.attributes.end())
			{
				result.name = it.value().toByteArray();
			}
		}
		return result;
	};
	result->setDelegateInfoCallback(delegateInfoCallback);
	return result;
}

template <typename VALUE_T, typename FIELD_PROP_T>
class QtnStructPropertyBase : public QtnSinglePropertyBase<VALUE_T>
{
	typedef QtnSinglePropertyBase<VALUE_T> Inherited;

public:
	typedef QtnStructPropertyBase ParentClass;

	template <typename FIELD_RET = typename FIELD_PROP_T::ValueType,
		typename FIELD_ARG = FIELD_RET>
	inline FIELD_PROP_T *createFieldProperty(const QString &name,
		const QString &displayName, const QString &desc_fmt,
		FIELD_RET (VALUE_T::*get)() const, void (VALUE_T::*set)(FIELD_ARG))
	{
		return qtnCreateFieldProperty<FIELD_PROP_T, VALUE_T>(
			this, name, displayName, desc_fmt, get, set);
	}

protected:
	explicit QtnStructPropertyBase(QObject *parent)
		: Inherited(parent)
	{
	}
};
