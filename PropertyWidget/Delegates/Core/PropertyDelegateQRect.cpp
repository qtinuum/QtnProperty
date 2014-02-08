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

#include "PropertyDelegateQRect.h"
#include "../../../Core/Core/PropertyQRect.h"
#include "../PropertyDelegateFactory.h"

#include <QLineEdit>

static bool regQRectDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyQRectBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateQRect, QtnPropertyQRectBase>
                                , "LTWH");

QtnPropertyDelegateQRect::QtnPropertyDelegateQRect(QtnPropertyQRectBase& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyQRectBase>(owner)
{
    addSubProperty(qtnCreateLeftProperty(0, &owner));
    addSubProperty(qtnCreateTopProperty(0, &owner));
    addSubProperty(qtnCreateWidthProperty(0, &owner));
    addSubProperty(qtnCreateHeightProperty(0, &owner));
}

QWidget* QtnPropertyDelegateQRect::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQRect::propertyValueToStr(QString& strValue) const
{
    QRect value = owner().value();
    strValue = QString("[(%1, %2), %3 x %4]").arg(value.left()).arg(value.top()).arg(value.width()).arg(value.height());
    return true;
}
