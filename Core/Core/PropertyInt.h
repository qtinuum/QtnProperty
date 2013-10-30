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

#include "../Aux/PropertyTemplates.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyIntBase: public NumericPropertyBase<SinglePropertyBase<qint32>>
{
    Q_OBJECT
    PropertyIntBase(const PropertyIntBase& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyIntBase(QObject *parent)
        : NumericPropertyBase<SinglePropertyBase<qint32>>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyIntBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyIntBase, qint32)

class QTN_PE_CORE_EXPORT PropertyIntCallback: public SinglePropertyCallback<PropertyIntBase>
{
    Q_OBJECT
    PropertyIntCallback(const PropertyIntCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyIntCallback(QObject *parent)
        : SinglePropertyCallback<PropertyIntBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyIntCallback, PropertyIntBase)
};

class QTN_PE_CORE_EXPORT PropertyInt: public SinglePropertyValue<PropertyIntBase>
{
    Q_OBJECT
    PropertyInt(const PropertyInt& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyInt(QObject *parent)
        : SinglePropertyValue<PropertyIntBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyInt, PropertyIntBase)
};

} // end namespace Qtinuum

#endif // PROPERTYINT_H
