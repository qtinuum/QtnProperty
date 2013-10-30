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

#include "PropertyQPoint.h"
#include "PropertyInt.h"

namespace Qtinuum
{

PropertyQPointBase::PropertyQPointBase(QObject *parent)
    : SinglePropertyBase<QPoint>(parent)
{
    addState(PropertyStateCollapsed);
}

Property* createXProperty(QObject *parent, PropertyQPointBase *propertyPoint)
{
    PropertyIntCallback *xProperty = new PropertyIntCallback(parent);
    xProperty->setName(QObject::tr("X"));
    xProperty->setDescription(QObject::tr("X coordinate of the %1.").arg(propertyPoint->name()));
    xProperty->setCallbackValueGet([propertyPoint]()->int { return propertyPoint->value().x(); });
    xProperty->setCallbackValueSet([propertyPoint](int newX) {
        QPoint point = propertyPoint->value();
        point.setX(newX);
        propertyPoint->setValue(point);
    });
    PropertyBase::connectMasterState(*propertyPoint, *xProperty);

    return xProperty;
}

Property* createYProperty(QObject *parent, PropertyQPointBase *propertyPoint)
{
    PropertyIntCallback *yProperty = new PropertyIntCallback(parent);
    yProperty->setName(QObject::tr("Y"));
    yProperty->setDescription(QObject::tr("Y coordinate of the %1.").arg(propertyPoint->name()));
    yProperty->setCallbackValueGet([propertyPoint]()->int { return propertyPoint->value().y(); });
    yProperty->setCallbackValueSet([propertyPoint](int newY) {
        QPoint point = propertyPoint->value();
        point.setY(newY);
        propertyPoint->setValue(point);
    });
    PropertyBase::connectMasterState(*propertyPoint, *yProperty);

    return yProperty;
}

} // end namespace Qtinuum
