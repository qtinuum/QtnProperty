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

#ifndef PROPERTYCOLOR_H
#define PROPERTYCOLOR_H

#include "../PropertyBasis.h"
#include <QtGui/QColor>

namespace Qtinuum
{

enum QColorDelegateShape
{
    QColorDelegateShapeNone = 0x1,
    QColorDelegateShapeSquare = 0x2,
    QColorDelegateShapeCircle = 0x3
};

class QTN_PE_CORE_EXPORT PropertyQColorBase: public SinglePropertyBase<QColor>
{
    Q_OBJECT

public:
    explicit PropertyQColorBase(QObject *parent)
        : SinglePropertyBase<QColor>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyQColorBase)
    P_PROPERTY_DECL_COPY_CONSTRUCTORS(PropertyQColorBase, SinglePropertyBase<QColor>)
};

P_PROPERTY_DECL_EQ_OPERATORS(PropertyQColorBase, QColor)

class QTN_PE_CORE_EXPORT PropertyQColorCallback: public SinglePropertyCallback<PropertyQColorBase>
{
    Q_OBJECT

public:
    explicit PropertyQColorCallback(QObject *parent)
        : SinglePropertyCallback<PropertyQColorBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQColorCallback, PropertyQColorBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyQColorCallback, SinglePropertyCallback<PropertyQColorBase>)
};

class QTN_PE_CORE_EXPORT PropertyQColor: public SinglePropertyValue<PropertyQColorBase>
{
    Q_OBJECT

public:
    explicit PropertyQColor(QObject *parent)
        : SinglePropertyValue<PropertyQColorBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQColor, PropertyQColorBase)
    P_PROPERTY_DECL_MEMBER_CLONING(PropertyQColor, SinglePropertyValue<PropertyQColorBase>)
};

} // end namespace Qtinuum

#endif // PROPERTYCOLOR_H
