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

#ifndef PROPERTYDOUBLE_H
#define PROPERTYDOUBLE_H

#include "../PropertyBasis.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyDoubleBase: public NumericPropertyBase<SinglePropertyBase<double> >
{
    Q_OBJECT

public:
    explicit PropertyDoubleBase(QObject *parent)
        : NumericPropertyBase<SinglePropertyBase<double> >(parent)
    {
    }

    operator double() const { return value(); }

    PropertyDoubleBase &operator=(const PropertyDoubleBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyDoubleBase &operator=(double newValue)
    {
        setValue(newValue);
        return *this;
    }

private:
    PropertyDoubleBase(const PropertyDoubleBase &) Q_DECL_EQ_DELETE;
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyDoubleBase, double)

class QTN_PE_CORE_EXPORT PropertyDoubleCallback: public SinglePropertyCallback<PropertyDoubleBase>
{
    Q_OBJECT

public:
    explicit PropertyDoubleCallback(QObject *parent)
        : SinglePropertyCallback<PropertyDoubleBase>(parent)
    {
    }

    operator int() const { return value(); }

    PropertyDoubleCallback &operator=(const PropertyDoubleCallback &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyDoubleCallback &operator=(const PropertyDoubleBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyDoubleCallback &operator=(double newValue)
    {
        setValue(newValue);
        return *this;
    }
};

class QTN_PE_CORE_EXPORT PropertyDouble: public SinglePropertyValue<PropertyDoubleBase>
{
    Q_OBJECT

public:
    explicit PropertyDouble(QObject *parent)
        : SinglePropertyValue<PropertyDoubleBase>(parent)
    {
    }

    operator double() const { return value(); }

    PropertyDouble &operator=(const PropertyDouble &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyDouble &operator=(const PropertyDoubleBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyDouble &operator=(double newValue)
    {
        setValue(newValue);
        return *this;
    }
};

} // end namespace Qtinuum

#endif // PROPERTYDOUBLE_H
