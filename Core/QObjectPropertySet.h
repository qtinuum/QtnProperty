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
