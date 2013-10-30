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

#ifndef PROPERTYENUMFLAGS_H
#define PROPERTYENUMFLAGS_H

#include "../Aux/PropertyTemplates.h"
#include "../Enum.h"

namespace Qtinuum
{

// not unsigned to disable warnings
typedef /*unsigned*/ int EnumFlagsValueType;

class QTN_PE_CORE_EXPORT PropertyEnumFlagsBase: public SinglePropertyBase<EnumFlagsValueType>
{
    Q_OBJECT
    PropertyEnumFlagsBase(const PropertyEnumFlagsBase& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyEnumFlagsBase(QObject *parent);

    const EnumInfo *enumInfo() const { return m_enumInfo; }
    EnumInfo *enumInfo() { return m_enumInfo; }
    void setEnumInfo(EnumInfo *enumInfo) { m_enumInfo = enumInfo; }

private:
    EnumInfo *m_enumInfo;

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyEnumFlagsBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyEnumFlagsBase, EnumFlagsValueType)

class QTN_PE_CORE_EXPORT PropertyEnumFlagsCallback: public SinglePropertyCallback<PropertyEnumFlagsBase>
{
    Q_OBJECT
    PropertyEnumFlagsCallback(const PropertyEnumFlagsCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyEnumFlagsCallback(QObject *parent)
        : SinglePropertyCallback<PropertyEnumFlagsBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyEnumFlagsCallback, PropertyEnumFlagsBase)
};

class QTN_PE_CORE_EXPORT PropertyEnumFlags: public SinglePropertyValue<PropertyEnumFlagsBase>
{
    Q_OBJECT
    PropertyEnumFlags(const PropertyEnumFlags& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyEnumFlags(QObject *parent)
        : SinglePropertyValue<PropertyEnumFlagsBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyEnumFlags, PropertyEnumFlagsBase)
};

} // end namespace Qtinuum

#endif // PROPERTYENUMFLAGS_H
