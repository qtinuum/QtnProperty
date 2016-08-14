#include "Extension.h"

#include "PropertyInt64.h"
#include "PropertyUInt64.h"
#include "PropertyPercent.h"
#include "PropertyQPointF.h"
#include "PropertyQSizeF.h"
#include "PropertyQRectF.h"
#include "PropertyQVariant.h"
#include "MultiProperty.h"

#include "IQtnPropertyStateProvider.h"

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>

#include <map>

static void QtnPropertyExtension_InitResources()
{
	Q_INIT_RESOURCE(QtnPropertyExtension);
}

namespace QtnPropertyExtension
{
	void Register()
	{
		QtnPropertyExtension_InitResources();

		QtnPropertyInt64::Register();
		QtnPropertyUInt64::Register();
		QtnPropertyPercent::Register();
		QtnPropertyQPointF::Register();
		QtnPropertyQSizeF::Register();
		QtnPropertyQRectF::Register();
		QtnPropertyQVariant::Register();
		QtnMultiProperty::Register();
	}

	QtnProperty* CreateQObjectProperty(QObject* object, const char *className, const QMetaProperty& metaProperty)
	{
		auto property =	qtnCreateQObjectProperty(object, metaProperty);

		if (nullptr != property)
		{
			if (!metaProperty.isDesignable(object))
			{
				delete property;
				return nullptr;
			}

			auto stateProvider = dynamic_cast<IQtnPropertyStateProvider *>(object);
			if (nullptr != stateProvider)
			{
				auto state = stateProvider->getPropertyState(metaProperty);
				property->setState(state);
			}

			if (metaProperty.isConstant()
			||	(!metaProperty.isWritable() && !metaProperty.isResettable()))
			{
				property->addState(QtnPropertyStateImmutable);
			}

			property->setName(QCoreApplication::translate(className,
														  metaProperty.name()));

			if (metaProperty.hasNotifySignal())
			{
				auto connector = new PropertyConnector(property);
				connector->connectProperty(object, metaProperty);
			}
		}

		return property;
	}

	QtnPropertySet *CreateQObjectPropertySet(QObject *object)
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
					auto class_name = QCoreApplication::translate("ClassName",
																  metaObject->className());
					auto it = propertySetsByClass.find(class_name);

					QtnPropertySet *propertySetByClass;
					if (it != propertySetsByClass.end())
						propertySetByClass = it->second;
					else
					{
						propertySetByClass = new QtnPropertySet(nullptr);
						propertySetByClass->setName(class_name);
						propertySetsByClass[class_name] = propertySetByClass;

						classNames.push_back(class_name);
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

	PropertyConnector::PropertyConnector(QtnProperty *parent)
		: QObject(parent)
		, object(nullptr)
	{
	}

	void PropertyConnector::connectProperty(QObject *object, const QMetaProperty &metaProperty)
	{
		this->object = object;
		this->metaProperty = metaProperty;
		auto meta_object = metaObject();
		auto slot = meta_object->method(meta_object->indexOfSlot("onValueChanged()"));
		QObject::connect(object, metaProperty.notifySignal(), this, slot);
	}

	bool PropertyConnector::isResettablePropertyValue() const
	{
		return metaProperty.isResettable();
	}

	void PropertyConnector::resetPropertyValue()
	{
		if (nullptr != object && metaProperty.isResettable())
			metaProperty.reset(object);
	}

	void PropertyConnector::onValueChanged()
	{
		auto property = dynamic_cast<QtnPropertyBase *>(parent());
		if (nullptr != property)
		{
			auto stateProvider = dynamic_cast<IQtnPropertyStateProvider *>(object);
			if (nullptr != stateProvider)
			{
				auto state = stateProvider->getPropertyState(metaProperty);
				bool collapsed = property->isCollapsed();
				if (collapsed)
					state |= QtnPropertyStateCollapsed;
				else
					state &= ~QtnPropertyStateCollapsed;
				property->setState(state);
			}
			property->postUpdateEvent(QtnPropertyChangeReasonNewValue);
		}
	}

	void InstallTranslations(const QLocale &locale)
	{
		static QTranslator base_translator;
		if (base_translator.load(locale, "QtnProperty.qm", "", ":/Translations"))
			QCoreApplication::installTranslator(&base_translator);

		static QTranslator extension_translator;
		if (extension_translator.load(locale, "QtnPropertyExtension.qm", "", ":/Translations"))
			QCoreApplication::installTranslator(&extension_translator);
	}

	QtnPropertySet *CreateQObjectMultiPropertySet(const std::set<QObject *> &objects)
	{
		if (objects.empty())
			return nullptr;

		auto result = new QtnPropertySet(nullptr);
		auto &subSets = result->childProperties();

		for (auto object : objects)
		{
			auto propertySet = CreateQObjectPropertySet(object);
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

}
