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

class QTN_PE_CORE_EXPORT PropertyIntBase: public NumericPropertyBase<SinglePropertyBase<qint32>>
{
    Q_OBJECT

public:
    explicit PropertyIntBase(QObject *parent)
        : NumericPropertyBase<SinglePropertyBase<qint32>>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyIntBase)
    P_PROPERTY_DECL_COPY_CONSTRUCTORS(PropertyIntBase, NumericPropertyBase<SinglePropertyBase<qint32>>)
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

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyIntCallback, PropertyIntBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyIntCallback, SinglePropertyCallback<PropertyIntBase>)
};

class QTN_PE_CORE_EXPORT PropertyInt: public SinglePropertyValue<PropertyIntBase>
{
    Q_OBJECT

public:
    explicit PropertyInt(QObject *parent)
        : SinglePropertyValue<PropertyIntBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyInt, PropertyIntBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyInt, SinglePropertyValue<PropertyIntBase>)
};

} // end namespace Qtinuum

#endif // PROPERTYINT_H
