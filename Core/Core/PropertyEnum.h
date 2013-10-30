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

#ifndef PROPERTYENUM_H
#define PROPERTYENUM_H

#include "../Aux/PropertyTemplates.h"
#include "../Enum.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyEnumBase: public SinglePropertyBase<EnumValueType>
{
    Q_OBJECT
    PropertyEnumBase(const PropertyEnumBase& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyEnumBase(QObject *parent)
        : SinglePropertyBase<EnumValueType>(parent),
          m_enumInfo(0)
    {
    }

    const EnumInfo *enumInfo() const { return m_enumInfo; }
    EnumInfo *enumInfo() { return m_enumInfo; }
    void setEnumInfo(EnumInfo *enumInfo) { m_enumInfo = enumInfo; }

protected:
    bool isValueAcceptedImpl(ValueType valueToAccept) override;

private:
    EnumInfo *m_enumInfo;

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyEnumBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyEnumBase, EnumValueType)

class QTN_PE_CORE_EXPORT PropertyEnumCallback: public SinglePropertyCallback<PropertyEnumBase>
{
    Q_OBJECT
    PropertyEnumCallback(const PropertyEnumCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyEnumCallback(QObject *parent)
        : SinglePropertyCallback<PropertyEnumBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyEnumCallback, PropertyEnumBase)
};

class QTN_PE_CORE_EXPORT PropertyEnum: public SinglePropertyValue<PropertyEnumBase>
{
    Q_OBJECT
    PropertyEnum(const PropertyEnum& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyEnum(QObject *parent)
        : SinglePropertyValue<PropertyEnumBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyEnum, PropertyEnumBase)
};

} // end namespace Qtinuum

#endif // PROPERTYENUM_H
