#pragma once

#include "QtnProperty/Core/QObjectPropertySet.h"
#include "QtnProperty/Core/Auxiliary/PropertyTemplates.h"

#include <QMetaProperty>

#include <functional>

template <typename PropertyCallbackType, typename ValueType = typename PropertyCallbackType::ValueTypeStore>
QtnMetaPropertyFactory_t qtnCreateFactory()
{
	typedef typename PropertyCallbackType::ValueType CallbackValueType;
	typedef typename PropertyCallbackType::ValueTypeStore CallbackValueTypeStore;

	return [](QObject *object, const QMetaProperty &metaProperty) -> QtnProperty *
	{
		auto property = new PropertyCallbackType(nullptr);

		property->setCallbackValueGet([object, metaProperty]() -> CallbackValueType
		{
			auto variantValue = metaProperty.read(object);
			return CallbackValueTypeStore(variantValue.value<ValueType>());
		});

		property->setCallbackValueSet([object, metaProperty](CallbackValueType value)
		{
			auto variantValue = QVariant::fromValue<ValueType>(ValueType(value));
			metaProperty.write(object, variantValue);
		});

		property->setCallbackValueAccepted([property](CallbackValueType) -> bool
		{
			return property->isEditableByUser();
		});

		return property;
	};
}

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
		QtnPropertyBase::connectMasterState(*this, *result);

		return result;
	}
};

namespace QtnPropertyExtension
{
	class PropertyConnector : public QObject
	{
		Q_OBJECT

	public:
		explicit PropertyConnector(QtnProperty *parent);

		void connectProperty(QObject *object, const QMetaProperty &meta_property);

		bool isResettablePropertyValue() const;
		void resetPropertyValue();

	private slots:
		void onValueChanged();

	private:
		QObject *object;
		QMetaProperty meta_property;
	};


	void InstallTranslations(const QLocale &locale);
	void Register();
	QtnPropertySet *CreateQObjectPropertySet(QObject *object);
}

