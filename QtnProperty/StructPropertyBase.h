/*
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

#pragma once

#include "Auxiliary/PropertyTemplates.h"

template <typename VALUE_T, typename FIELD_PROP_T, typename FIELD_T = typename FIELD_PROP_T::ValueType>
class QtnStructPropertyBase : public QtnSinglePropertyBase<VALUE_T>
{
public:
	typedef FIELD_PROP_T FieldProperty;
	typedef FIELD_T FieldValueType;
	typedef typename FIELD_PROP_T::ValueType CallbackValueType;
	typedef QtnStructPropertyBase ParentClass;

private:
	typedef QtnSinglePropertyBase<VALUE_T> Inherited;

protected:
	explicit QtnStructPropertyBase(QObject *parent)
		: Inherited(parent)
	{
	}

	QtnProperty *createFieldProperty(const QString &name, const QString &desc_fmt,
									 FieldValueType (VALUE_T::*get)() const,
									 void (VALUE_T::*set)(FieldValueType))
	{
		auto result = new FieldProperty(nullptr);

		result->setName(name);
		result->setDescription(desc_fmt.arg(this->name()));
		result->setCallbackValueGet([this, get]()->CallbackValueType
		{
			return (Inherited::value().*get)();
		});
		result->setCallbackValueSet([this, set](CallbackValueType new_value)
		{
			auto rect = Inherited::value();
			(rect.*set)(new_value);
			Inherited::setValue(rect);
		});

		return result;
	}
};
