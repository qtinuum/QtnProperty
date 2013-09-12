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

#ifndef PROPERTYBOOL_H
#define PROPERTYBOOL_H

#include "../PropertyBasis.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyBoolBase: public SinglePropertyBase<bool>
{
    Q_OBJECT

public:
    explicit PropertyBoolBase(QObject *parent)
        : SinglePropertyBase<bool>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyBoolBase)
    P_PROPERTY_DECL_COPY_CONSTRUCTORS(PropertyBoolBase, SinglePropertyBase<bool>)
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyBoolBase, bool)

class QTN_PE_CORE_EXPORT PropertyBoolCallback: public SinglePropertyCallback<PropertyBoolBase>
{
    Q_OBJECT

public:
    explicit PropertyBoolCallback(QObject *parent)
        : SinglePropertyCallback<PropertyBoolBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyBoolCallback, PropertyBoolBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyBoolCallback, SinglePropertyCallback<PropertyBoolBase>)
};

class QTN_PE_CORE_EXPORT PropertyBool: public SinglePropertyValue<PropertyBoolBase>
{
    Q_OBJECT

public:
    explicit PropertyBool(QObject *parent)
        : SinglePropertyValue<PropertyBoolBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyBool, PropertyBoolBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyBool, SinglePropertyValue<PropertyBoolBase>)
};

} // end namespace Qtinuum

#endif // PROPERTYBOOL_H
