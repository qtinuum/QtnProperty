/*******************************************************************************
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

template <typename VALUE_T, typename FIELD_PROP_T,
	typename FIELD_T = typename FIELD_PROP_T::ValueType>
class QtnStructPropertyBase : public QtnSinglePropertyBase<VALUE_T>
{
public:
	typedef FIELD_PROP_T FieldProperty;
	typedef FIELD_T FieldValueType;
	typedef typename FIELD_PROP_T::ValueType CallbackValueType;
	typedef typename FIELD_PROP_T::ValueTypeStore CallbackValueTypeStore;
	typedef QtnStructPropertyBase ParentClass;

private:
	typedef QtnSinglePropertyBase<VALUE_T> Inherited;

protected:
	explicit QtnStructPropertyBase(QObject *parent)
		: Inherited(parent)
	{
	}

	QtnProperty *createFieldProperty(const QString &name,
		const QString &desc_fmt, FieldValueType (VALUE_T::*get)() const,
		void (VALUE_T::*set)(FieldValueType))
	{
		auto result = new FieldProperty(nullptr);

		result->setName(name);
		result->setDescription(desc_fmt.arg(this->name()));
		result->setCallbackValueGet([this, get]() -> CallbackValueType {
			return (Inherited::value().*get)();
		});
		result->setCallbackValueSet([this, set](CallbackValueType new_value) {
			auto v = Inherited::value();
			(v.*set)(new_value);
			Inherited::setValue(v);
		});

		if (Inherited::isResettable())
		{
			result->addState(QtnPropertyStateResettable);
			result->setCallbackValueDefault([this, get]() -> CallbackValueType {
				typename Inherited::ValueTypeStore value;
				this->defaultValueImpl(value);
				return (value.*get)();
			});
		}

		auto delegateInfoCallback = [this]() -> QtnPropertyDelegateInfo {
			auto baseDelegate = Inherited::delegateInfo();
			QtnPropertyDelegateInfo result;
			if (baseDelegate)
			{
				result.attributes = baseDelegate->attributes;
			}
			auto it = result.attributes.find(qtnFieldDelegateName());
			if (it != result.attributes.end())
			{
				result.name = it.value().toByteArray();
			}
			return result;
		};
		result->setDelegateInfoCallback(delegateInfoCallback);
		return result;
	}
};
