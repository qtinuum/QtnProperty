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

#include "Property.h"
#include "Delegates/Utils/PropertyDelegateMisc.h"

#include <QMetaProperty>

#include <set>
#include <memory>

class QtnMultiPropertyDelegate;
class QtnConnections;

class QTN_IMPORT_EXPORT QtnMultiProperty : public QtnProperty
{
	Q_OBJECT

public:
	explicit QtnMultiProperty(
		const QMetaObject *propertyMetaObject, QObject *parent = nullptr);
	virtual ~QtnMultiProperty() override;

	virtual const QMetaObject *propertyMetaObject() const override;

	void addProperty(QtnProperty *property, bool own = true);

	bool hasMultipleValues() const;

	static QString getMultiValuePlaceholder();

	inline const std::vector<QtnProperty *> &getProperties() const;

	QMetaProperty getMetaProperty() const;

private:
	void onPropertyValueAccept(QtnPropertyValuePtr valueToAccept, bool *accept);

	void onPropertyWillChange(QtnPropertyChangeReason reason,
		QtnPropertyValuePtr newValue, int typeId);

	void onPropertyDidChange(QtnPropertyChangeReason reason);

protected:
	virtual void doReset(QtnPropertyChangeReason reason) override;
	virtual bool loadImpl(QDataStream &stream) override;
	virtual bool saveImpl(QDataStream &stream) const override;
	virtual void masterPropertyWillChange(
		QtnPropertyChangeReason reason) override;
	virtual void masterPropertyDidChange(
		QtnPropertyChangeReason reason) override;

	virtual bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	virtual bool toStrImpl(QString &str) const override;

	virtual bool fromVariantImpl(
		const QVariant &var, QtnPropertyChangeReason reason) override;
	virtual bool toVariantImpl(QVariant &var) const override;

private:
	void updateStateFrom(QtnProperty *source);
	void updateMultipleState(bool force);

private:
	std::vector<QtnProperty *> properties;
	const QMetaObject *mPropertyMetaObject;
	unsigned m_subPropertyUpdates;

	bool edited;
	bool calculateMultipleValues;
	bool multipleValues;

	friend class QtnMultiPropertyDelegate;
};

const std::vector<QtnProperty *> &QtnMultiProperty::getProperties() const
{
	return properties;
}

class QtnMultiPropertyDelegate
	: public QtnPropertyDelegateTypedEx<QtnMultiProperty, QtnPropertyDelegate>
{
	Q_DISABLE_COPY(QtnMultiPropertyDelegate)

	typedef QtnPropertyDelegateTypedEx Inherited;

public:
	QtnMultiPropertyDelegate(QtnMultiProperty &owner);
	virtual ~QtnMultiPropertyDelegate() override;

	static void Register(QtnPropertyDelegateFactory &factory);

private:
	virtual void init() override;

	struct PropertyToEdit;

	static void onEditedPropertyDestroyed(PropertyToEdit *data);
	static void onEditorDestroyed(PropertyToEdit *data);

protected:
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;

	void createSubItemsImpl(
		QtnDrawContext &context, QList<QtnSubItem> &subItems) override;

private:
	typedef std::unique_ptr<QtnPropertyDelegate> DelegatePtr;
	std::vector<DelegatePtr> superDelegates;
};

QTN_IMPORT_EXPORT void qtnPropertiesToMultiSet(
	QtnPropertySet *target, QtnPropertySet *source, bool takeOwnership);

struct QtnMultiVariant
{
	QVariantList values;
};

Q_DECLARE_METATYPE(QtnMultiVariant)
