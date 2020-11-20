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

template <typename FIELD_T, typename CLASS_T, typename SETTER_T>
void qtnSetFieldValue(CLASS_T &to, SETTER_T setter, FIELD_T value,
	typename std::enable_if<
		std::is_member_function_pointer<SETTER_T>::value>::type * = nullptr)
{
	(to.*setter)(value);
}

template <typename FIELD_T, typename CLASS_T, typename SETTER_T>
void qtnSetFieldValue(CLASS_T &to, SETTER_T field, FIELD_T value,
	typename std::enable_if<
		std::is_member_object_pointer<SETTER_T>::value>::type * = nullptr)
{
	to.*field = value;
}

template <typename FIELD_T, typename CLASS_T, typename GETTER_T>
FIELD_T qtnGetFieldValue(CLASS_T from, GETTER_T getter,
	typename std::enable_if<
		std::is_member_function_pointer<GETTER_T>::value>::type * = nullptr)
{
	return (from.*getter)();
}

template <typename FIELD_T, typename CLASS_T, typename GETTER_T>
FIELD_T qtnGetFieldValue(const CLASS_T &from, GETTER_T field,
	typename std::enable_if<
		std::is_member_object_pointer<GETTER_T>::value>::type * = nullptr)
{
	return from.*field;
}

template <typename FIELD_PROP_T, typename VALUE_T, typename GETTER_T,
	typename SETTER_T>
FIELD_PROP_T *qtnCreateFieldProperty(QtnSinglePropertyBase<VALUE_T> *property,
	GETTER_T getter, SETTER_T setter, const QString &name = QString(),
	const QString &displayName = QString(), const QString &desc_fmt = QString())
{
	using CallbackValueType = typename FIELD_PROP_T::ValueType;
	using CallbackValueTypeStore = typename FIELD_PROP_T::ValueTypeStore;
	using ValueTypeStore =
		typename QtnSinglePropertyBase<VALUE_T>::ValueTypeStore;

	Q_ASSERT(property);

	auto result = new FIELD_PROP_T(nullptr);

	if (!displayName.isEmpty())
		result->setDisplayName(displayName);
	if (!name.isEmpty())
		result->setName(name);
	if (!desc_fmt.isEmpty())
		result->setDescription(desc_fmt.arg(property->displayName()));

	result->setCallbackValueGet([property, getter]() -> CallbackValueTypeStore {
		return qtnGetFieldValue<CallbackValueTypeStore>(
			property->value(), getter);
	});
	result->setCallbackValueSet(
		[property, setter](CallbackValueType new_value, QtnPropertyChangeReason reason) {
			auto v = property->value();
			qtnSetFieldValue(v, setter, new_value);
			property->setValue(v, reason);
		});

	if (!property->isQObjectProperty() && property->isResettable())
	{
		result->addState(QtnPropertyStateResettable);
		result->setCallbackValueDefault(
			[property, getter]() -> CallbackValueTypeStore {
				ValueTypeStore value;
				if (!property->readDefaultValue(value))
				{
					value = property->value();
				}
				return qtnGetFieldValue<CallbackValueTypeStore>(value, getter);
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

	template <typename GETTER_T, typename SETTER_T>
	inline FIELD_PROP_T *createFieldProperty(GETTER_T getter, SETTER_T setter,
		const QString &name = QString(), const QString &displayName = QString(),
		const QString &desc_fmt = QString())
	{
		return qtnCreateFieldProperty<FIELD_PROP_T>(
			this, getter, setter, name, displayName, desc_fmt);
	}

protected:
	explicit QtnStructPropertyBase(QObject *parent)
		: Inherited(parent)
	{
	}
};
