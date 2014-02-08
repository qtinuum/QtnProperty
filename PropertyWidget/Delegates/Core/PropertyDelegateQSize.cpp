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

#include "PropertyDelegateQSize.h"
#include "../../../Core/Core/PropertyQSize.h"
#include "../PropertyDelegateFactory.h"

#include <QLineEdit>

static bool regQSizeDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyQSizeBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateQSize, QtnPropertyQSizeBase>
                                , "WH");

QtnPropertyDelegateQSize::QtnPropertyDelegateQSize(QtnPropertyQSizeBase& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyQSizeBase>(owner)
{
    addSubProperty(qtnCreateWidthProperty(0, &owner));
    addSubProperty(qtnCreateHeightProperty(0, &owner));
}

QWidget* QtnPropertyDelegateQSize::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQSize::propertyValueToStr(QString& strValue) const
{
    QSize value = owner().value();
    strValue = QString("%1 x %2").arg(value.width()).arg(value.height());
    return true;
}
