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

#include "PropertyDelegateQPoint.h"
#include "../../../Core/Core/PropertyQPoint.h"
#include "../PropertyDelegateFactory.h"


namespace Qtinuum
{

static bool regQPointDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyQPointBase::staticMetaObject
                                , &createDelegate<PropertyDelegateQPoint, PropertyQPointBase>
                                , "XY");

PropertyDelegateQPoint::PropertyDelegateQPoint(PropertyQPointBase& owner)
    : PropertyDelegateTypedEx<PropertyQPointBase>(owner)
{
    addSubProperty(createXProperty(0, &owner));
    addSubProperty(createYProperty(0, &owner));
}

QWidget* PropertyDelegateQPoint::createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo)
{
    return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool PropertyDelegateQPoint::propertyValueToStr(QString& strValue) const
{
    QPoint value = owner().value();
    strValue = QString("%1 x %2").arg(value.x()).arg(value.y());
    return true;
}

} // end namespace Qtinuum

