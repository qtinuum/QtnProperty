/*
 * Copyright (c) 2012 - 2014, the Qtinuum project.
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

static bool regQPointDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyQPointBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateQPoint, QtnPropertyQPointBase>
                                , "XY");

QtnPropertyDelegateQPoint::QtnPropertyDelegateQPoint(QtnPropertyQPointBase& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyQPointBase>(owner)
{
    addSubProperty(qtnCreateXProperty(0, &owner));
    addSubProperty(qtnCreateYProperty(0, &owner));
}

QWidget* QtnPropertyDelegateQPoint::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQPoint::propertyValueToStr(QString& strValue) const
{
    QPoint value = owner().value();
    strValue = QString("%1 x %2").arg(value.x()).arg(value.y());
    return true;
}
