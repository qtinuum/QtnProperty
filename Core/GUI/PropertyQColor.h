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

#include "../Auxiliary/PropertyTemplates.h"
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
    PropertyQColorBase(const PropertyQColorBase& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyQColorBase(QObject *parent)
        : SinglePropertyBase<QColor>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyQColorBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(PropertyQColorBase, QColor)

class QTN_PE_CORE_EXPORT PropertyQColorCallback: public SinglePropertyCallback<PropertyQColorBase>
{
    Q_OBJECT
    PropertyQColorCallback(const PropertyQColorCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyQColorCallback(QObject *parent)
        : SinglePropertyCallback<PropertyQColorBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQColorCallback, PropertyQColorBase)
};

class QTN_PE_CORE_EXPORT PropertyQColor: public SinglePropertyValue<PropertyQColorBase>
{
    Q_OBJECT
    PropertyQColor(const PropertyQColor& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyQColor(QObject *parent)
        : SinglePropertyValue<PropertyQColorBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQColor, PropertyQColorBase)
};

} // end namespace Qtinuum

#endif // PROPERTYCOLOR_H
