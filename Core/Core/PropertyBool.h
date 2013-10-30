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

#include "../Aux/PropertyTemplates.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyBoolBase: public SinglePropertyBase<bool>
{
    Q_OBJECT
    PropertyBoolBase(const PropertyBoolBase& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyBoolBase(QObject *parent)
        : SinglePropertyBase<bool>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyBoolBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyBoolBase, bool)

class QTN_PE_CORE_EXPORT PropertyBoolCallback: public SinglePropertyCallback<PropertyBoolBase>
{
    Q_OBJECT
    PropertyBoolCallback(const PropertyBoolCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyBoolCallback(QObject *parent)
        : SinglePropertyCallback<PropertyBoolBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyBoolCallback, PropertyBoolBase)
};

class QTN_PE_CORE_EXPORT PropertyBool: public SinglePropertyValue<PropertyBoolBase>
{
    Q_OBJECT
    PropertyBool(const PropertyBool& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyBool(QObject *parent)
        : SinglePropertyValue<PropertyBoolBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyBool, PropertyBoolBase)
};

} // end namespace Qtinuum

#endif // PROPERTYBOOL_H
