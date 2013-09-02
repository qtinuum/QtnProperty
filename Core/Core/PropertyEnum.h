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

#include "../PropertyBasis.h"
#include "../Enum.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyEnumBase: public SinglePropertyBase<EnumValueType>
{
    Q_OBJECT

public:
    explicit PropertyEnumBase(QObject *parent)
        : SinglePropertyBase<EnumValueType>(parent),
          m_enumInfo(0)
    {
    }

    operator EnumValueType() const { return value(); }

    PropertyEnumBase &operator=(const PropertyEnumBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyEnumBase &operator=(EnumValueType newValue)
    {
        setValue(newValue);
        return *this;
    }

    const EnumInfo *enumInfo() const { return m_enumInfo; }
    EnumInfo *enumInfo() { return m_enumInfo; }
    void setEnumInfo(EnumInfo *enumInfo) { m_enumInfo = enumInfo; }

protected:
    bool isValueAcceptedImpl(ValueType valueToAccept) override;

private:
    EnumInfo *m_enumInfo;

    PropertyEnumBase(const PropertyEnumBase &) Q_DECL_EQ_DELETE;
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyEnumBase, EnumValueType);

class QTN_PE_CORE_EXPORT PropertyEnumCallback: public SinglePropertyCallback<PropertyEnumBase>
{
    Q_OBJECT

public:
    explicit PropertyEnumCallback(QObject *parent)
        : SinglePropertyCallback<PropertyEnumBase>(parent)
    {
    }

    operator EnumValueType() const { return value(); }

    PropertyEnumCallback &operator=(const PropertyEnumCallback &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyEnumCallback &operator=(const PropertyEnumBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyEnumCallback &operator=(EnumValueType newValue)
    {
        setValue(newValue);
        return *this;
    }
};

class QTN_PE_CORE_EXPORT PropertyEnum: public SinglePropertyValue<PropertyEnumBase>
{
    Q_OBJECT

public:
    explicit PropertyEnum(QObject *parent)
        : SinglePropertyValue<PropertyEnumBase>(parent)
    {
    }

    operator EnumValueType() const { return value(); }

    PropertyEnum &operator=(const PropertyEnum &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyEnum &operator=(const PropertyEnumBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyEnum &operator=(EnumValueType newValue)
    {
        setValue(newValue);
        return *this;
    }
};

} // end namespace Qtinuum

#endif // PROPERTYENUM_H
