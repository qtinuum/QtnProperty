/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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

#include "QObjectPropertySet.h"
#include "PropertyCore.h"
#include "PropertyGUI.h"
#include "PropertyConnector.h"
#include "MultiProperty.h"
#include "IQtnPropertyStateProvider.h"

#include <QCoreApplication>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMap>

bool qtnRegisterDefaultMetaPropertyFactory()
{
	qtnRegisterMetaPropertyFactory(QVariant::Bool, qtnCreateFactory<QtnPropertyBoolCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::String, qtnCreateFactory<QtnPropertyQStringCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::Double, qtnCreateFactory<QtnPropertyDoubleCallback>());
//    qtnRegisterMetaPropertyFactory(QVariant::Float, qtnCreateFactory<QtnPropertyFloatCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::Int, qtnCreateFactory<QtnPropertyIntCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::UInt, qtnCreateFactory<QtnPropertyUIntCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::Point, qtnCreateFactory<QtnPropertyQPointCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::Rect, qtnCreateFactory<QtnPropertyQRectCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::Size, qtnCreateFactory<QtnPropertyQSizeCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::Color, qtnCreateFactory<QtnPropertyQColorCallback>());
	qtnRegisterMetaPropertyFactory(QVariant::Font, qtnCreateFactory<QtnPropertyQFontCallback>());
	return true;
}

static QMap<int, QtnMetaPropertyFactory_t> qtnFactoryMap;
static bool success = qtnRegisterDefaultMetaPropertyFactory();

bool qtnRegisterMetaPropertyFactory(int metaPropertyType, const QtnMetaPropertyFactory_t& factory, bool force)
{
	Q_ASSERT(factory);

	if (!force && qtnFactoryMap.contains(metaPropertyType))
		return false;

	qtnFactoryMap.insert(metaPropertyType, factory);
	return true;
}

void qtnUpdatePropertyState(QtnPropertyBase *property, const QMetaProperty &metaProperty)
{
	QtnPropertyState toAdd;

	if (!metaProperty.isDesignable())
		toAdd |= QtnPropertyStateInvisible;

	if (metaProperty.isConstant()
	||	(!metaProperty.isWritable() && !metaProperty.isResettable()))
	{
		toAdd |= QtnPropertyStateImmutable;
	}

	property->addState(toAdd);
}

QtnProperty* qtnCreateQObjectProperty(QObject* object, const QMetaProperty& metaProperty)
{
	if (!object)
		return nullptr;

	auto it = qtnFactoryMap.find(metaProperty.type());
	if (it == qtnFactoryMap.end())
		it = qtnFactoryMap.find(metaProperty.userType());
	if (it == qtnFactoryMap.end())
		return nullptr;

	if (!metaProperty.isDesignable(object) || !metaProperty.isReadable())
		return nullptr;

	QtnProperty* property = it.value()(object, metaProperty);
	if (!property)
		return property;

	property->setName(metaProperty.name());

	auto stateProvider = dynamic_cast<IQtnPropertyStateProvider *>(object);
	if (nullptr != stateProvider)
	{
		auto state = stateProvider->getPropertyState(metaProperty);
		property->setState(state);
	}

	qtnUpdatePropertyState(property, metaProperty);

	return property;
}

QtnProperty* qtnCreateQObjectProperty(QObject* object, const char* propertyName)
{
	if (!object)
		return nullptr;

	const QMetaObject* metaObject = object->metaObject();
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

	return qtnCreateQObjectProperty(object, metaObject->property(propertyIndex));
}

QtnProperty* CreateQObjectProperty(QObject* object, const char *className, const QMetaProperty& metaProperty)
{
	auto property =	qtnCreateQObjectProperty(object, metaProperty);

	if (nullptr != property)
	{
		property->setName(QCoreApplication::translate(className,
													  metaProperty.name()));

		if (metaProperty.hasNotifySignal())
		{
			auto connector = new QtnPropertyConnector(property);
			connector->connectProperty(object, metaProperty);
		}
	}

	return property;
}

QtnPropertySet *qtnCreateQObjectPropertySet(QObject *object)
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
			QList<QtnProperty*> properties;
			for (int propertyIndex = metaObject->propertyOffset(),
				 n = metaObject->propertyCount();
				 propertyIndex < n; ++propertyIndex)
			{
				auto metaProperty = metaObject->property(propertyIndex);
				auto property = CreateQObjectProperty(object, metaObject->className(), metaProperty);
				if (nullptr != property)
					properties.append(property);
			}

			if (!properties.isEmpty())
			{
				auto className = QCoreApplication::translate("ClassName",
															  metaObject->className());
				auto it = propertySetsByClass.find(className);

				QtnPropertySet *propertySetByClass;
				if (it != propertySetsByClass.end())
					propertySetByClass = it->second;
				else
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

	for (auto &class_name : classNames)
	{
		propertySet->addChildProperty(propertySetsByClass[class_name]);
	}

	return propertySet;
}

QtnPropertySet *qtnCreateQObjectMultiPropertySet(const std::set<QObject *> &objects)
{
	if (objects.empty())
		return nullptr;

	auto result = new QtnPropertySet(nullptr);
	auto &subSets = result->childProperties();

	for (auto object : objects)
	{
		auto propertySet = qtnCreateQObjectPropertySet(object);
		if (nullptr == propertySet)
			continue;

		for (int i = propertySet->childProperties().count() - 1;
			 i >= 0; i--)
		{
			auto property = propertySet->childProperties().at(i);

			auto subSet = dynamic_cast<QtnPropertySet *>(property);
			Q_ASSERT(nullptr != subSet);

			auto it = std::find_if(subSets.begin(), subSets.end(),
			[subSet](const QtnPropertyBase *set) -> bool
			{
				return (subSet->name() == set->name());
			});

			QtnPropertySet *multiSet;
			if (it == subSets.end())
			{
				multiSet = new QtnPropertySet(nullptr);
				multiSet->setName(subSet->name());
				multiSet->setDescription(subSet->description());
				multiSet->setId(subSet->id());
				multiSet->setState(subSet->stateLocal());

				result->addChildProperty(multiSet, true, 0);
			} else
			{
				multiSet = dynamic_cast<QtnPropertySet *>(*it);
			}

			auto &ncp = multiSet->childProperties();
			for (auto subProp : subSet->childProperties())
			{
				auto propIt = std::find_if(ncp.begin(), ncp.end(),
				[subProp](QtnPropertyBase *p) -> bool
				{
					return p->name() == subProp->name();
				});

				QtnMultiProperty *multiProperty;
				if (propIt == ncp.end())
				{
					multiProperty = new QtnMultiProperty(subProp->metaObject());
					multiProperty->setName(subProp->name());
					multiProperty->setDescription(subProp->description());
					multiProperty->setId(subProp->id());
					multiProperty->setState(subProp->stateLocal());

					multiSet->addChildProperty(multiProperty);
				} else
				{
					multiProperty = dynamic_cast<QtnMultiProperty *>(*propIt);
				}

				multiProperty->addProperty(dynamic_cast<QtnProperty *>(subProp));
			}
		}

		delete propertySet;
	}

	return result;
}
