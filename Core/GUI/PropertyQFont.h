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

#ifndef PROPERTYFONT_H
#define PROPERTYFONT_H

#include "../PropertyBasis.h"
#include <QtGui/QFont>

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyQFontBase: public SinglePropertyBase<QFont>
{
    Q_OBJECT

public:
    explicit PropertyQFontBase(QObject *parent);

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyQFontBase)
    P_PROPERTY_DECL_COPY_CONSTRUCTORS(PropertyQFontBase, SinglePropertyBase<QFont>)
};

P_PROPERTY_DECL_EQ_OPERATORS(PropertyQFontBase, QFont)

class QTN_PE_CORE_EXPORT PropertyQFontCallback: public SinglePropertyCallback<PropertyQFontBase>
{
    Q_OBJECT

public:
    explicit PropertyQFontCallback(QObject *parent)
        : SinglePropertyCallback<PropertyQFontBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQFontCallback, PropertyQFontBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyQFontCallback, SinglePropertyCallback<PropertyQFontBase>)
};

class QTN_PE_CORE_EXPORT PropertyQFont: public SinglePropertyValue<PropertyQFontBase>
{
    Q_OBJECT

public:
    explicit PropertyQFont(QObject *parent)
        : SinglePropertyValue<PropertyQFontBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQFont, PropertyQFontBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyQFont, SinglePropertyValue<PropertyQFontBase>)
};

} // end namespace Qtinuum

#endif // PROPERTYFONT_H
