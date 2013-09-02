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

#ifndef PROPERTYCOLOR_H
#define PROPERTYCOLOR_H

#include "../PropertyBasis.h"
#include <QtGui/QColor>

namespace Qtinuum
{

enum QColorDelegateShape
{
    QColorDelegateShapeNone = 0x1,
    QColorDelegateShapeSquare = 0x2,
    QColorDelegateShapeCircle = 0x3
};

class QTN_PE_CORE_EXPORT PropertyQColorBase: public SinglePropertyBase<QColor>
{
    Q_OBJECT

public:
    explicit PropertyQColorBase(QObject *parent)
        : SinglePropertyBase<QColor>(parent)
    {
    }

    operator QColor() const { return value(); }

    PropertyQColorBase &operator=(const PropertyQColorBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQColorBase &operator=(const QColor &newValue)
    {
        setValue(newValue);
        return *this;
    }

private:
    PropertyQColorBase(const PropertyQColorBase &) Q_DECL_EQ_DELETE;
};
P_PROPERTY_DECL_EQ_OPERATORS(PropertyQColorBase, QColor)

class QTN_PE_CORE_EXPORT PropertyQColorCallback: public SinglePropertyCallback<PropertyQColorBase>
{
    Q_OBJECT

public:
    explicit PropertyQColorCallback(QObject *parent)
        : SinglePropertyCallback<PropertyQColorBase>(parent)
    {
    }

    operator QColor() const { return value(); }

    PropertyQColorCallback &operator=(const PropertyQColorCallback &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQColorCallback &operator=(const PropertyQColorBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQColorCallback &operator=(const QColor &newValue)
    {
        setValue(newValue);
        return *this;
    }
};

class QTN_PE_CORE_EXPORT PropertyQColor: public SinglePropertyValue<PropertyQColorBase>
{
    Q_OBJECT

public:
    explicit PropertyQColor(QObject *parent)
        : SinglePropertyValue<PropertyQColorBase>(parent)
    {
    }

    operator QColor() const { return value(); }

    PropertyQColor &operator=(const PropertyQColor &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQColor &operator=(const PropertyQColorBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQColor &operator=(const QColor &newValue)
    {
        setValue(newValue);
        return *this;
    }
};

} // end namespace Qtinuum

#endif // PROPERTYCOLOR_H
