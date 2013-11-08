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

#include "../Auxiliary/PropertyTemplates.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyDoubleBase: public NumericPropertyBase<SinglePropertyBase<double>>
{
    Q_OBJECT
    PropertyDoubleBase(const PropertyDoubleBase& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyDoubleBase(QObject *parent)
        : NumericPropertyBase<SinglePropertyBase<double>>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyDoubleBase)

protected:
    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override
    {
        bool ok = false;
        ValueType value = var.toDouble(&ok);
        if (!ok)
            return false;

        return setValue(value);
    }
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyDoubleBase, double)

class QTN_PE_CORE_EXPORT PropertyDoubleCallback: public SinglePropertyCallback<PropertyDoubleBase>
{
    Q_OBJECT
    PropertyDoubleCallback(const PropertyDoubleCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyDoubleCallback(QObject *parent)
        : SinglePropertyCallback<PropertyDoubleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyDoubleCallback, PropertyDoubleBase)
};

class QTN_PE_CORE_EXPORT PropertyDouble: public SinglePropertyValue<PropertyDoubleBase>
{
    Q_OBJECT
    PropertyDouble(const PropertyDouble& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyDouble(QObject *parent)
        : SinglePropertyValue<PropertyDoubleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyDouble, PropertyDoubleBase)
};

} // end namespace Qtinuum

#endif // PROPERTYDOUBLE_H
