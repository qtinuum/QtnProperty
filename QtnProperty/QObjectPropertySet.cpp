/*******************************************************************************
Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
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

#include "QObjectPropertySet.h"
#include "PropertyCore.h"
#include "PropertyGUI.h"
#include "PropertyConnector.h"
#include "MultiProperty.h"
#include "IQtnPropertyStateProvider.h"
#include "Config.h"
#include "PropertyDelegateAttrs.h"

#include <QCoreApplication>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMap>
#include <QLocale>

static QtnProperty *createRealNumberProperty(
	QObject *object, const QMetaProperty &metaProperty)
{
	auto property = new QtnPropertyDoubleCallback(nullptr);
	property->setCallbackValueAccepted(
		[property](double) -> bool { return property->isEditableByUser(); });

	switch (metaProperty.revision())
	{
		case PERCENT_SUFFIX:
		{
			QtnPropertyDelegateInfo delegate;
			qtnInitPercentSpinBoxDelegate(delegate);
			property->setDelegate(delegate);

			property->setCallbackValueGet([object, metaProperty]() -> double {
				return metaProperty.read(object).toDouble() * 100.0;
			});

			property->setCallbackValueSet([object, metaProperty](double value) {
				metaProperty.write(object, value / 100.0);
			});

			return property;
		}

		case DEGREE_SUFFIX:
		{
			QtnPropertyDelegateInfo delegate;
			qtnInitDegreeSpinBoxDelegate(delegate);
			property->setDelegate(delegate);
			break;
		}
	}

	property->setCallbackValueGet([object, metaProperty]() -> double {
		return metaProperty.read(object).toDouble();
	});

	property->setCallbackValueSet([object, metaProperty](double value) {
		metaProperty.write(object, value);
	});

	return property;
}

bool qtnRegisterDefaultMetaPropertyFactory()
{
	qtnRegisterMetaPropertyFactory(
		QVariant::Bool, qtnCreateFactory<QtnPropertyBoolCallback>());
	qtnRegisterMetaPropertyFactory(
		QVariant::String, qtnCreateFactory<QtnPropertyQStringCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::Double, createRealNumberProperty);
	qtnRegisterMetaPropertyFactory(
		QVariant::Int, qtnCreateFactory<QtnPropertyIntCallback>());
	qtnRegisterMetaPropertyFactory(
		QVariant::UInt, qtnCreateFactory<QtnPropertyUIntCallback>());
	qtnRegisterMetaPropertyFactory(
		QVariant::Point, qtnCreateFactory<QtnPropertyQPointCallback>());
	qtnRegisterMetaPropertyFactory(
		QVariant::Rect, qtnCreateFactory<QtnPropertyQRectCallback>());
	qtnRegisterMetaPropertyFactory(
		QVariant::Size, qtnCreateFactory<QtnPropertyQSizeCallback>());
	qtnRegisterMetaPropertyFactory(
		QVariant::Color, qtnCreateFactory<QtnPropertyQColorCallback>());
	qtnRegisterMetaPropertyFactory(
		QVariant::Font, qtnCreateFactory<QtnPropertyQFontCallback>());
	return true;
}

static QMap<int, QtnMetaPropertyFactory_t> &qtnFactoryMap()
{
	static QMap<int, QtnMetaPropertyFactory_t> result;
	return result;
};

bool qtnRegisterMetaPropertyFactory(
	int metaPropertyType, const QtnMetaPropertyFactory_t &factory, bool force)
{
	Q_ASSERT(factory);

	auto &map = qtnFactoryMap();
	if (!force && map.contains(metaPropertyType))
		return false;

	map.insert(metaPropertyType, factory);
	return true;
}

QtnPropertyState qtnPropertyStateToAdd(const QMetaProperty &metaProperty)
{
	QtnPropertyState toAdd;

	if (!metaProperty.isDesignable())
		toAdd |= QtnPropertyStateInvisible;

	if (metaProperty.isConstant() ||
		(!metaProperty.isWritable() && !metaProperty.isResettable()))
	{
		toAdd |= QtnPropertyStateImmutable;
	}

	return toAdd;
}

void qtnUpdatePropertyState(
	QtnPropertyBase *property, const QMetaProperty &metaProperty)
{
	property->addState(qtnPropertyStateToAdd(metaProperty));
}

QtnProperty *qtnCreateQObjectProperty(QObject *object,
	const QMetaProperty &metaProperty, bool connect, const char *className)
{
	if (!object)
		return nullptr;

	auto &map = qtnFactoryMap();

	auto it = map.find(metaProperty.type());

	if (it == map.end())
		it = map.find(metaProperty.userType());

	if (it == map.end())
		return nullptr;

	if (!metaProperty.isDesignable(object) || !metaProperty.isReadable())
		return nullptr;

	QtnProperty *property = it.value()(object, metaProperty);

	if (!property)
		return property;

	property->setName((nullptr != className)
			? QCoreApplication::translate(className, metaProperty.name())
			: metaProperty.name());

	auto stateProvider = dynamic_cast<IQtnPropertyStateProvider *>(object);

	if (nullptr != stateProvider)
	{
		auto state = stateProvider->getPropertyState(metaProperty);
		property->setState(state);
	}

	qtnUpdatePropertyState(property, metaProperty);

	if (connect && metaProperty.hasNotifySignal())
	{
		auto connector = new QtnPropertyConnector(property);
		connector->connectProperty(object, metaProperty);
	}

	return property;
}

QtnProperty *qtnCreateQObjectProperty(
	QObject *object, const char *propertyName, bool connect)
{
	if (!object)
		return nullptr;

	const QMetaObject *metaObject = object->metaObject();
	int propertyIndex = -1;

	while (metaObject)
	{
		propertyIndex = object->metaObject()->indexOfProperty(propertyName);

		if (propertyIndex != -1)
			break;

		metaObject = metaObject->superClass();
	}

	if (!metaObject)
		return nullptr;

	if (propertyIndex == -1)
		return nullptr;

	Q_ASSERT(propertyIndex >= 0 && propertyIndex < metaObject->propertyCount());

	return qtnCreateQObjectProperty(object, metaObject->property(propertyIndex),
		connect, metaObject->className());
}

QtnPropertySet *qtnCreateQObjectPropertySet(QObject *object, bool backwards)
{
	if (!object)
		return nullptr;

	// collect property sets by object's classes
	QStringList classNames;
	std::map<QString, QtnPropertySet *> propertySetsByClass;

	auto metaObject = object->metaObject();

	while (nullptr != metaObject)
	{
		if (metaObject->propertyCount() > 0)
		{
			QList<QtnProperty *> properties;

			for (int propertyIndex = metaObject->propertyOffset(),
					 n = metaObject->propertyCount();
				 propertyIndex < n; ++propertyIndex)
			{
				auto metaProperty = metaObject->property(propertyIndex);
				auto property = qtnCreateQObjectProperty(
					object, metaProperty, true, metaObject->className());

				if (nullptr != property)
					properties.append(property);
			}

			if (!properties.isEmpty())
			{
				auto className = QCoreApplication::translate(
					"ClassName", metaObject->className());
				auto it = propertySetsByClass.find(className);

				QtnPropertySet *propertySetByClass;

				if (it != propertySetsByClass.end())
				{
					propertySetByClass = it->second;
				} else
				{
					propertySetByClass = new QtnPropertySet(nullptr);
					propertySetByClass->setName(className);
					propertySetsByClass[className] = propertySetByClass;

					classNames.push_back(className);
				}

				for (auto property : properties)
				{
					propertySetByClass->addChildProperty(property);
				}
			}
		}

		// move up in class hierarchy
		metaObject = metaObject->superClass();
	}

	if (propertySetsByClass.empty())
		return nullptr;

	// move collected property sets to object's property set
	auto propertySet = new QtnPropertySet(nullptr);
	propertySet->setName(object->objectName());

	int addIndex = backwards ? 0 : -1;

	for (auto &class_name : classNames)
	{
		propertySet->addChildProperty(
			propertySetsByClass[class_name], true, addIndex);
	}

	return propertySet;
}

QtnPropertySet *qtnCreateQObjectMultiPropertySet(
	const std::set<QObject *> &objects, bool backwards)
{
	if (objects.empty())
		return nullptr;

	auto result = new QtnPropertySet(nullptr);

	for (auto object : objects)
	{
		auto propertySet = qtnCreateQObjectPropertySet(object, backwards);

		if (nullptr == propertySet)
			continue;

		qtnPropertiesToMultiSet(result, propertySet, false);

		delete propertySet;
	}

	return result;
}

void qtnInitPercentSpinBoxDelegate(QtnPropertyDelegateInfo &delegate)
{
	delegate.name = qtnSpinBoxDelegate();
	delegate.attributes[qtnSuffixAttr()] = QLocale().percent();
}

void qtnInitDegreeSpinBoxDelegate(QtnPropertyDelegateInfo &delegate)
{
	delegate.name = qtnSpinBoxDelegate();
	delegate.attributes[qtnSuffixAttr()] = QString::fromUtf8("º");
}

void qtnPropertiesToMultiSet(
	QtnPropertySet *target, QtnPropertySet *source, bool sorted)
{
	Q_ASSERT(target);
	Q_ASSERT(source);

	auto &targetProperties = target->childProperties();
	for (auto property : source->childProperties())
	{
		auto it = std::find_if(targetProperties.begin(), targetProperties.end(),
			[property](const QtnPropertyBase *targetProperty) -> bool {
				auto multiProperty =
					qobject_cast<const QtnMultiProperty *>(targetProperty);

				auto targetMetaObject = multiProperty
					? multiProperty->propertyMetaObject()
					: targetProperty->metaObject();

				return property->metaObject() == targetMetaObject &&
					property->name() == targetProperty->name();
			});

		auto subSet = qobject_cast<QtnPropertySet *>(property);
		if (subSet)
		{
			QtnPropertySet *multiSet;

			if (it == targetProperties.end())
			{
				multiSet = new QtnPropertySet(nullptr);
				multiSet->setName(subSet->name());
				multiSet->setDescription(subSet->description());
				multiSet->setId(subSet->id());
				multiSet->setState(subSet->stateLocal());

				target->addChildProperty(
					multiSet, true, -1, QtnPropertySet::Ascend);
			} else
			{
				multiSet = qobject_cast<QtnPropertySet *>(*it);
			}

			qtnPropertiesToMultiSet(multiSet, subSet, sorted);
		} else
		{
			QtnMultiProperty *multiProperty;

			if (it == targetProperties.end())
			{
				multiProperty = new QtnMultiProperty(property->metaObject());
				multiProperty->setName(property->name());
				multiProperty->setDescription(property->description());
				multiProperty->setId(property->id());

				target->addChildProperty(
					multiProperty, true, -1, QtnPropertySet::Ascend);
			} else
			{
				multiProperty = qobject_cast<QtnMultiProperty *>(*it);
			}

			multiProperty->addProperty(property->asProperty());
		}
	}
}
