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

#ifndef PROPERTYFLOAT_H
#define PROPERTYFLOAT_H

#include "../PropertyBasis.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyFloatBase: public NumericPropertyBase<SinglePropertyBase<float> >
{
    Q_OBJECT

public:
    explicit PropertyFloatBase(QObject *parent)
        : NumericPropertyBase<SinglePropertyBase<float> >(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyFloatBase)
    P_PROPERTY_DECL_COPY_CONSTRUCTORS(PropertyFloatBase, NumericPropertyBase<SinglePropertyBase<float>>)
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyFloatBase, float)

class QTN_PE_CORE_EXPORT PropertyFloatCallback: public SinglePropertyCallback<PropertyFloatBase>
{
    Q_OBJECT

public:
    explicit PropertyFloatCallback(QObject *parent)
        : SinglePropertyCallback<PropertyFloatBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyFloatCallback, PropertyFloatBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyFloatCallback, SinglePropertyCallback<PropertyFloatBase>)
};

class QTN_PE_CORE_EXPORT PropertyFloat: public SinglePropertyValue<PropertyFloatBase>
{
    Q_OBJECT

public:
    explicit PropertyFloat(QObject *parent)
        : SinglePropertyValue<PropertyFloatBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyFloat, PropertyFloatBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyFloat, SinglePropertyValue<PropertyFloatBase>)
};

} // end namespace Qtinuum

#endif // PROPERTYFLOAT_H
