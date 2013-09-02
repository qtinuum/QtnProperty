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

#ifndef PROPERTYQRECT_H
#define PROPERTYQRECT_H

#include "../PropertyBasis.h"
#include <QRect>

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyQRectBase: public SinglePropertyBase<QRect>
{
    Q_OBJECT

public:
    explicit PropertyQRectBase(QObject *parent);

    operator QRect() const { return value(); }

    PropertyQRectBase &operator=(const PropertyQRectBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQRectBase &operator=(const QRect &newValue)
    {
        setValue(newValue);
        return *this;
    }

private:
    PropertyQRectBase(const PropertyQRectBase &) Q_DECL_EQ_DELETE;
};
P_PROPERTY_DECL_EQ_OPERATORS(PropertyQRectBase, QRect);

QTN_PE_CORE_EXPORT Property* createLeftProperty(QObject *parent, PropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT Property* createRightProperty(QObject *parent, PropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT Property* createTopProperty(QObject *parent, PropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT Property* createBottomProperty(QObject *parent, PropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT Property* createWidthProperty(QObject *parent, PropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT Property* createHeightProperty(QObject *parent, PropertyQRectBase *propertyRect);

class QTN_PE_CORE_EXPORT PropertyQRectCallback: public SinglePropertyCallback<PropertyQRectBase>
{
    Q_OBJECT

public:
    explicit PropertyQRectCallback(QObject *parent)
        : SinglePropertyCallback<PropertyQRectBase>(parent)
    {
    }

    operator QRect() const { return value(); }

    PropertyQRectCallback &operator=(const PropertyQRectCallback &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQRectCallback &operator=(const PropertyQRectBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQRectCallback &operator=(const QRect &newValue)
    {
        setValue(newValue);
        return *this;
    }
};

class QTN_PE_CORE_EXPORT PropertyQRect: public SinglePropertyValue<PropertyQRectBase>
{
    Q_OBJECT

public:
    explicit PropertyQRect(QObject *parent)
        : SinglePropertyValue<PropertyQRectBase>(parent)
    {
    }

    operator QRect() const { return value(); }

    PropertyQRect &operator=(const PropertyQRect &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQRect &operator=(const PropertyQRectBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQRect &operator=(const QRect &newValue)
    {
        setValue(newValue);
        return *this;
    }
};

} // end namespace Qtinuum

#endif // PROPERTYQRECT_H
