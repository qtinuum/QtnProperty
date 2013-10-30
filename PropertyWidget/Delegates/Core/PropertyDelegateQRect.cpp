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

#include "PropertyDelegateQRect.h"
#include "../../../Core/Core/PropertyQRect.h"
#include "../PropertyDelegateFactory.h"

#include <QLineEdit>

namespace Qtinuum
{

static bool regQRectDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyQRectBase::staticMetaObject
                                , &createDelegate<PropertyDelegateQRect, PropertyQRectBase>
                                , "LTWH");

PropertyDelegateQRect::PropertyDelegateQRect(PropertyQRectBase& owner)
    : PropertyDelegateTypedEx<PropertyQRectBase>(owner)
{
    addSubProperty(createLeftProperty(0, &owner));
    addSubProperty(createTopProperty(0, &owner));
    addSubProperty(createWidthProperty(0, &owner));
    addSubProperty(createHeightProperty(0, &owner));
}

QWidget* PropertyDelegateQRect::createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo)
{
    return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool PropertyDelegateQRect::propertyValueToStr(QString& strValue) const
{
    QRect value = owner().value();
    strValue = QString("[(%1, %2), %3 x %4]").arg(value.left()).arg(value.top()).arg(value.width()).arg(value.height());
    return true;
}

} // end namespace Qtinuum

