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

#ifndef PROPERTYINT_H
#define PROPERTYINT_H

#include "../PropertyBasis.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyIntBase: public NumericPropertyBase<SinglePropertyBase<qint32> >
{
    Q_OBJECT

public:
    explicit PropertyIntBase(QObject *parent)
        : NumericPropertyBase<SinglePropertyBase<qint32> >(parent)
    {
    }

    operator qint32() const { return value(); }

    PropertyIntBase &operator=(const PropertyIntBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyIntBase &operator=(qint32 newValue)
    {
        setValue(newValue);
        return *this;
    }

private:
    PropertyIntBase(const PropertyIntBase &) Q_DECL_EQ_DELETE;
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyIntBase, qint32)

class QTN_PE_CORE_EXPORT PropertyIntCallback: public SinglePropertyCallback<PropertyIntBase>
{
    Q_OBJECT

public:
    explicit PropertyIntCallback(QObject *parent)
        : SinglePropertyCallback<PropertyIntBase>(parent)
    {
    }

    operator qint32() const { return value(); }

    PropertyIntCallback &operator=(const PropertyIntCallback &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyIntCallback &operator=(const PropertyIntBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyIntCallback &operator=(qint32 newValue)
    {
        setValue(newValue);
        return *this;
    }
};

class QTN_PE_CORE_EXPORT PropertyInt: public SinglePropertyValue<PropertyIntBase>
{
    Q_OBJECT

public:
    explicit PropertyInt(QObject *parent)
        : SinglePropertyValue<PropertyIntBase>(parent)
    {
    }

    operator qint32() const { return value(); }

    PropertyInt &operator=(const PropertyInt &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyInt &operator=(const PropertyIntBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyInt &operator=(qint32 newValue)
    {
        setValue(newValue);
        return *this;
    }
};

} // end namespace Qtinuum

#endif // PROPERTYINT_H
