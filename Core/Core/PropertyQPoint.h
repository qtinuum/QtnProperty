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

#include "../PropertyBasis.h"
#include <QPoint>

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyQPointBase: public SinglePropertyBase<QPoint>
{
    Q_OBJECT

public:
    explicit PropertyQPointBase(QObject *parent);

    operator QPoint() const { return value(); }

    PropertyQPointBase &operator=(const PropertyQPointBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQPointBase &operator=(const QPoint & newValue)
    {
        setValue(newValue);
        return *this;
    }

private:
    PropertyQPointBase(const PropertyQPointBase &) Q_DECL_EQ_DELETE;
};
P_PROPERTY_DECL_EQ_OPERATORS(PropertyQPointBase, QPoint);

QTN_PE_CORE_EXPORT Property* createXProperty(QObject *parent, PropertyQPointBase *propertyPoint);
QTN_PE_CORE_EXPORT Property* createYProperty(QObject *parent, PropertyQPointBase *propertyPoint);

class QTN_PE_CORE_EXPORT PropertyQPointCallback: public SinglePropertyCallback<PropertyQPointBase>
{
    Q_OBJECT

public:
    explicit PropertyQPointCallback(QObject *parent)
        : SinglePropertyCallback<PropertyQPointBase>(parent)
    {
    }

    operator QPoint() const { return value(); }

    PropertyQPointCallback &operator=(const PropertyQPointCallback &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQPointCallback &operator=(const PropertyQPointBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQPointCallback &operator=(const QPoint & newValue)
    {
        setValue(newValue);
        return *this;
    }
};

class QTN_PE_CORE_EXPORT PropertyQPoint: public SinglePropertyValue<PropertyQPointBase>
{
    Q_OBJECT

public:
    explicit PropertyQPoint(QObject *parent)
        : SinglePropertyValue<PropertyQPointBase>(parent)
    {
    }

    operator QPoint() const { return value(); }

    PropertyQPoint &operator=(const PropertyQPoint &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQPoint &operator=(const PropertyQPointBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQPoint &operator=(const QPoint & newValue)
    {
        setValue(newValue);
        return *this;
    }
};

} // end namespace Qtinuum

#endif // PROPERTYQPOINT_H
