/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

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
#include <QDebug>
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
            Q_UNUSED(value);
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
    // qtnRegisterMetaPropertyFactory(QVariant::Float, qtnCreateFactory<QtnPropertyFloatCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::Int, qtnCreateFactory<QtnPropertyIntCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::UInt, qtnCreateFactory<QtnPropertyUIntCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::Point, qtnCreateFactory<QtnPropertyQPointCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::PointF, qtnCreateFactory<QtnPropertyQPointFCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::Rect, qtnCreateFactory<QtnPropertyQRectCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::RectF, qtnCreateFactory<QtnPropertyQRectFCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::Size, qtnCreateFactory<QtnPropertyQSizeCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::SizeF, qtnCreateFactory<QtnPropertyQSizeFCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::Color, qtnCreateFactory<QtnPropertyQColorCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::Font, qtnCreateFactory<QtnPropertyQFontCallback>());
    qtnRegisterMetaPropertyFactory(QVariant::Pen, qtnCreateFactory<QtnPropertyQPenCallback>());
    return true;
}

static QMap<int, QtnMetaPropertyFactory_t> qtnFactoryMap;

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
    static bool registerDefaultMetaPropertyFactory = qtnRegisterDefaultMetaPropertyFactory();

    if (!registerDefaultMetaPropertyFactory)
        return nullptr;

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
