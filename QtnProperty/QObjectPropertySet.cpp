/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
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

#include "QObjectPropertySet.h"
#include "PropertyCore.h"
#include "PropertyGUI.h"
#include "PropertyConnector.h"
#include "MultiProperty.h"
#include "IQtnPropertyStateProvider.h"
#include "Install.h"
#include "PropertyDelegateAttrs.h"

#include <QCoreApplication>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMap>
#include <QLocale>

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

    if (!metaProperty.isDesignable() || !metaProperty.isReadable())
		return nullptr;

	QtnProperty *property = it.value()(object, metaProperty);

	if (!property)
		return property;

	property->setName(metaProperty.name());
	if (className)
	{
		property->setDisplayName(
			QCoreApplication::translate(className, metaProperty.name()));
	}

	auto stateProvider = dynamic_cast<IQtnPropertyStateProvider *>(object);

	if (nullptr != stateProvider)
	{
		auto state = stateProvider->getPropertyState(metaProperty);
		property->setState(state);
	}

	qtnUpdatePropertyState(property, metaProperty);

	if (connect)
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
					propertySetByClass = new QtnPropertySet;
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
	auto propertySet = new QtnPropertySet;
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

		qtnPropertiesToMultiSet(result, propertySet, true);

		delete propertySet;
	}

	return result;
}
