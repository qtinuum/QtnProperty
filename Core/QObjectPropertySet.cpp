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

template <typename PropertyCallbackType>
QtnMetaPropertyFactory_t qtnCreateFactory()
{
    typedef typename PropertyCallbackType::ValueType ValueType;

    QtnMetaPropertyFactory_t factory = [] (QObject* object, const QMetaProperty& metaProperty)->QtnProperty* {

        typedef ValueType ValueTypeLocal;

        QScopedPointer<PropertyCallbackType> property(new PropertyCallbackType(object));

        property->setCallbackValueGet([object, metaProperty] ()->ValueType {
            QVariant variantValue = metaProperty.read(object);
            return variantValue.value<ValueTypeLocal>();
        });

        property->setCallbackValueSet([object, metaProperty] (ValueType value)->void {
            QVariant variantValue = QVariant::fromValue<ValueTypeLocal>(value);
            metaProperty.write(object, variantValue);
        });

        PropertyCallbackType* propertyPtr = property.data();
        property->setCallbackValueAccepted([propertyPtr] (ValueType value)->bool {
            return propertyPtr->isEditableByUser();
        });

        return property.take();
    };

    return factory;
}

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

bool qtnRegisterMetaPropertyFactory(int metaPropertyType, const QtnMetaPropertyFactory_t& factory)
{
    Q_ASSERT(factory);

    if (qtnFactoryMap.contains(metaPropertyType))
        return false;

    qtnFactoryMap.insert(metaPropertyType, factory);
    return true;
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

    QtnProperty* property = it.value()(object, metaProperty);
    if (!property)
        return property;

    property->setName(metaProperty.name());
    if (!metaProperty.isDesignable(object))
        property->addState(QtnPropertyStateImmutable);

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


QtnPropertySet* qtnCreateQObjectPropertySet(QObject* object)
{
    if (!object)
        return nullptr;

    // collect property sets by object's classes
    QList<QtnPropertySet*> propertySetsByClass;

    const QMetaObject* metaObject = object->metaObject();
    while (metaObject)
    {
        if (metaObject->propertyCount() > 0)
        {
            QList<QtnProperty*> properties;
            for (int propertyIndex = metaObject->propertyOffset(), n = metaObject->propertyCount(); propertyIndex < n; ++propertyIndex)
            {
                QMetaProperty metaProperty = metaObject->property(propertyIndex);
                QtnProperty* property = qtnCreateQObjectProperty(object, metaProperty);
                if (property)
                    properties.append(property);
            }

            if (!properties.isEmpty())
            {
                QScopedPointer<QtnPropertySet> propertySetByClass(new QtnPropertySet(nullptr));

                propertySetByClass->setName(metaObject->className());
                foreach (QtnProperty* property, properties)
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

    // move collected property sets to object's property set
    QScopedPointer<QtnPropertySet> propertySet(new QtnPropertySet(object));
    propertySet->setName(object->objectName());

    foreach (QtnPropertySet* propertySetByClass, propertySetsByClass)
    {
        propertySet->addChildProperty(propertySetByClass);
    }

    return propertySet.take();
}
