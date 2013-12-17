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

#include "../Auxiliary/PropertyTemplates.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyFloatBase: public NumericPropertyBase<SinglePropertyBase<float> >
{
    Q_OBJECT
    PropertyFloatBase(const PropertyFloatBase& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyFloatBase(QObject *parent)
        : NumericPropertyBase<SinglePropertyBase<float> >(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyFloatBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyFloatBase, float)

class QTN_PE_CORE_EXPORT PropertyFloatCallback: public SinglePropertyCallback<PropertyFloatBase>
{
    Q_OBJECT
    PropertyFloatCallback(const PropertyFloatCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyFloatCallback(QObject *parent)
        : SinglePropertyCallback<PropertyFloatBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyFloatCallback, PropertyFloatBase)
};

class QTN_PE_CORE_EXPORT PropertyFloat: public SinglePropertyValue<PropertyFloatBase>
{
    Q_OBJECT
    PropertyFloat(const PropertyFloat& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyFloat(QObject *parent)
        : SinglePropertyValue<PropertyFloatBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyFloat, PropertyFloatBase)
};

} // end namespace Qtinuum

#endif // PROPERTYFLOAT_H
