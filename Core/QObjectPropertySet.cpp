/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
 */

#include "QObjectPropertySet.h"
#include "PropertyCore.h"
#include "PropertyGUI.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <QMap>

namespace Qtinuum
{

template <typename PropertyCallbackType>
MetaPropertyFactory_t createFactory()
{
    typedef typename PropertyCallbackType::ValueType ValueType;

    MetaPropertyFactory_t factory = [] (QObject* object, const QMetaProperty& metaProperty)->Property* {

        QScopedPointer<PropertyCallbackType> property(new PropertyCallbackType(object));

        property->setCallbackValueGet([object, metaProperty] ()->ValueType {
            QVariant variantValue = metaProperty.read(object);
            return variantValue.value<ValueType>();
        });

        property->setCallbackValueSet([object, metaProperty] (ValueType value)->void {
            QVariant variantValue = QVariant::fromValue<ValueType>(value);
            metaProperty.write(object, variantValue);
        });

        return property.take();
    };

    return factory;
}

bool registerDefaultMetaPropertyFactory()
{
    registerMetaPropertyFactory(QVariant::Bool, createFactory<PropertyBoolCallback>());
    registerMetaPropertyFactory(QVariant::String, createFactory<PropertyQStringCallback>());
    registerMetaPropertyFactory(QVariant::Double, createFactory<PropertyDoubleCallback>());
//    registerMetaPropertyFactory(QVariant::Float, createFactory<PropertyFloatCallback>());
    registerMetaPropertyFactory(QVariant::Int, createFactory<PropertyIntCallback>());
    registerMetaPropertyFactory(QVariant::UInt, createFactory<PropertyUIntCallback>());
    registerMetaPropertyFactory(QVariant::Point, createFactory<PropertyQPointCallback>());
    registerMetaPropertyFactory(QVariant::Rect, createFactory<PropertyQRectCallback>());
    registerMetaPropertyFactory(QVariant::Size, createFactory<PropertyQSizeCallback>());
    registerMetaPropertyFactory(QVariant::Color, createFactory<PropertyQColorCallback>());
    registerMetaPropertyFactory(QVariant::Font, createFactory<PropertyQFontCallback>());
    return true;
}

static QMap<int, MetaPropertyFactory_t> factoryMap;
static bool success = registerDefaultMetaPropertyFactory();

bool registerMetaPropertyFactory(int metaPropertyType, const MetaPropertyFactory_t& factory)
{
    Q_ASSERT(factory);

    if (factoryMap.contains(metaPropertyType))
        return false;

    factoryMap.insert(metaPropertyType, factory);
    return true;
}

Property* createQObjectProperty(QObject* object, const QMetaProperty& metaProperty)
{
    if (!object)
        return nullptr;

    auto it = factoryMap.find(metaProperty.type());
    if (it == factoryMap.end())
        it = factoryMap.find(metaProperty.userType());
    if (it == factoryMap.end())
        return nullptr;

    Property* property = it.value()(object, metaProperty);
    if (!property)
        return property;

    property->setName(metaProperty.name());
    if (!metaProperty.isEditable(object))
        property->addState(PropertyStateImmutable);

    return property;
}

Property* createQObjectProperty(QObject* object, const char* propertyName)
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

    return createQObjectProperty(object, metaObject->property(propertyIndex));
}


PropertySet* createQObjectPropertySet(QObject* object)
{
    if (!object)
        return nullptr;

    QList<PropertySet*> propertySetsByClass;

    const QMetaObject* metaObject = object->metaObject();
    while (metaObject)
    {
        if (metaObject->propertyCount() > 0)
        {
            QList<Property*> properties;
            for (int propertyIndex = 0, n = metaObject->propertyCount(); propertyIndex < n; ++propertyIndex)
            {
                QMetaProperty metaProperty = metaObject->property(propertyIndex);
                Property* property = createQObjectProperty(object, metaProperty);
                if (property)
                    properties.append(property);
            }

            if (!properties.isEmpty())
            {
                QScopedPointer<PropertySet> propertySetByClass(new PropertySet(nullptr));

                propertySetByClass->setName(metaObject->className());
                foreach (Property* property, properties)
                {
                    propertySetByClass->addChildProperty(property);
                }

                propertySetsByClass.prepend(propertySetByClass.take());
            }
        }

        // move up in class hierarchy
        metaObject = metaObject->superClass();
    }

    if (propertySetsByClass.isEmpty())
        return nullptr;

    QScopedPointer<PropertySet> propertySet(new PropertySet(object));

    foreach (PropertySet* propertySetByClass, propertySetsByClass)
    {
        propertySet->addChildProperty(propertySetByClass);
    }

    return propertySet.take();
}

} // end namespace Qtinuum
