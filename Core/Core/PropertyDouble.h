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

class QTN_PE_CORE_EXPORT PropertyDoubleBase: public NumericPropertyBase<SinglePropertyBase<double>>
{
    Q_OBJECT

public:
    explicit PropertyDoubleBase(QObject *parent)
        : NumericPropertyBase<SinglePropertyBase<double>>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyDoubleBase)
    P_PROPERTY_DECL_COPY_CONSTRUCTORS(PropertyDoubleBase, NumericPropertyBase<SinglePropertyBase<double>>)
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

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyDoubleCallback, PropertyDoubleBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyDoubleCallback, SinglePropertyCallback<PropertyDoubleBase>)
};

class QTN_PE_CORE_EXPORT PropertyDouble: public SinglePropertyValue<PropertyDoubleBase>
{
    Q_OBJECT

public:
    explicit PropertyDouble(QObject *parent)
        : SinglePropertyValue<PropertyDoubleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyDouble, PropertyDoubleBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyDouble, SinglePropertyValue<PropertyDoubleBase>)
};

} // end namespace Qtinuum

#endif // PROPERTYDOUBLE_H
