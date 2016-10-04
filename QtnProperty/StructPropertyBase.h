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
