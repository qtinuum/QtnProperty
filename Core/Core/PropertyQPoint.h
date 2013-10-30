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

#ifndef PROPERTYQPOINT_H
#define PROPERTYQPOINT_H

#include "../Aux/PropertyTemplates.h"
#include <QPoint>

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyQPointBase: public SinglePropertyBase<QPoint>
{
    Q_OBJECT
    PropertyQPointBase(const PropertyQPointBase& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyQPointBase(QObject *parent);

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyQPointBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(PropertyQPointBase, QPoint)

QTN_PE_CORE_EXPORT Property* createXProperty(QObject *parent, PropertyQPointBase *propertyPoint);
QTN_PE_CORE_EXPORT Property* createYProperty(QObject *parent, PropertyQPointBase *propertyPoint);

class QTN_PE_CORE_EXPORT PropertyQPointCallback: public SinglePropertyCallback<PropertyQPointBase>
{
    Q_OBJECT
    PropertyQPointCallback(const PropertyQPointCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyQPointCallback(QObject *parent)
        : SinglePropertyCallback<PropertyQPointBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQPointCallback, PropertyQPointBase)
};

class QTN_PE_CORE_EXPORT PropertyQPoint: public SinglePropertyValue<PropertyQPointBase>
{
    Q_OBJECT
    PropertyQPoint(const PropertyQPoint& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyQPoint(QObject *parent)
        : SinglePropertyValue<PropertyQPointBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQPoint, PropertyQPointBase)
};

} // end namespace Qtinuum

#endif // PROPERTYQPOINT_H
