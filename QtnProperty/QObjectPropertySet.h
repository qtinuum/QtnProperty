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

#ifndef QTN_QOBJECT_PROPERTY_SET_H
#define QTN_QOBJECT_PROPERTY_SET_H

#include "Property.h"
#include "PropertySet.h"

typedef std::function<QtnProperty*(QObject*, const QMetaProperty&)> QtnMetaPropertyFactory_t;
QTN_PE_CORE_EXPORT bool qtnRegisterMetaPropertyFactory(int metaPropertyType, const QtnMetaPropertyFactory_t& factory);

QTN_PE_CORE_EXPORT QtnProperty* qtnCreateQObjectProperty(QObject* object, const QMetaProperty& metaProperty);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateQObjectProperty(QObject* object, const char* propertyName);

QTN_PE_CORE_EXPORT QtnPropertySet* qtnCreateQObjectPropertySet(QObject* object);

#endif // QTN_QOBJECT_PROPERTY_SET_H
