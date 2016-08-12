#pragma once

#include "QtnProperty/Core/QObjectPropertySet.h"
#include "QtnProperty/Core/Auxiliary/PropertyTemplates.h"

#include <QMetaProperty>

#include <functional>

class QtnMultiObject : public QObject, public QObjectList
{
	Q_OBJECT

public:
	QtnMultiObject(QObject *parent = nullptr);

	QtnPropertySet *createPropertySet() const;
};

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

		void connectProperty(QObject *object, const QMetaProperty &metaProperty);

		bool isResettablePropertyValue() const;
		void resetPropertyValue();

	private slots:
		void onValueChanged();

	private:
		QObject *object;
		QMetaProperty metaProperty;
	};


	void InstallTranslations(const QLocale &locale);
	void Register();
	QtnPropertySet *CreateQObjectPropertySet(QObject *object);
}

