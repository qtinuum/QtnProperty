#include "Extension.h"

#include "PropertyInt64.h"
#include "PropertyUInt64.h"
#include "PropertyPercent.h"
#include "PropertyQPointF.h"
#include "PropertyQSizeF.h"
#include "PropertyQRectF.h"
#include "PropertyQVariant.h"

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
	}

	QtnProperty* CreateQObjectProperty(QObject* object, const char *class_name, const QMetaProperty& meta_property)
	{
		auto property =	qtnCreateQObjectProperty(object, meta_property);

		if (nullptr != property)
		{
			if (!meta_property.isDesignable(object))
			{
				delete property;
				return nullptr;
			}

			if (!meta_property.isWritable()
			||	meta_property.isConstant())
			{
				property->addState(QtnPropertyStateImmutable);
			}

			property->setName(QCoreApplication::translate(class_name,
														  meta_property.name()));

			if (meta_property.hasNotifySignal())
			{
				auto connector = new PropertyConnector(property);
				connector->connectProperty(object, meta_property);
			}
		}

		return property;
	}

	QtnPropertySet *CreateQObjectPropertySet(QObject *object)
	{
		if (!object)
			return nullptr;

		// collect property sets by object's classes
		QStringList class_names;
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

						class_names.push_back(class_name);
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

		for (auto &class_name : class_names)
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

	void PropertyConnector::connectProperty(QObject *object, const QMetaProperty &meta_property)
	{
		this->object = object;
		this->meta_property = meta_property;
		auto meta_object = metaObject();
		auto slot = meta_object->method(meta_object->indexOfSlot("onValueChanged()"));
		QObject::connect(object, meta_property.notifySignal(), this, slot);
	}

	bool PropertyConnector::isResettablePropertyValue() const
	{
		return meta_property.isResettable();
	}

	void PropertyConnector::resetPropertyValue()
	{
		if (nullptr != object && meta_property.isResettable())
			meta_property.reset(object);
	}

	void PropertyConnector::onValueChanged()
	{
		auto property = dynamic_cast<QtnProperty *>(parent());
		if (nullptr != property)
			emit property->propertyDidChange(property, property, QtnPropertyChangeReasonNewValue);
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

}
