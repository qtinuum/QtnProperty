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

#include "PropertyQSize.h"
#include "PropertyInt.h"

namespace Qtinuum
{

PropertyQSizeBase::PropertyQSizeBase(QObject *parent)
    : SinglePropertyBase<QSize>(parent)
{
    addState(PropertyStateCollapsed);
}

bool PropertyQSizeBase::fromStrImpl(const QString& str)
{
    static QRegExp parserSize("^\\s*QSize\\s*\\(([^\\)]+)\\)\\s*$", Qt::CaseInsensitive);
    static QRegExp parserParams("^\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*$", Qt::CaseInsensitive);

    if (!parserSize.exactMatch(str))
        return false;

    QStringList params = parserSize.capturedTexts();
    if (params.size() != 2)
        return false;

    if (!parserParams.exactMatch(params[1]))
        return false;

    params = parserParams.capturedTexts();
    if (params.size() != 3)
        return false;

    bool ok = false;
    int width = params[1].toInt(&ok);
    if (!ok)
        return false;

    int height = params[2].toInt(&ok);
    if (!ok)
        return false;

    return setValue(QSize(width, height));
}

bool PropertyQSizeBase::toStrImpl(QString& str) const
{
    QSize v = value();
    str = QString("QSize(%1, %2)").arg(v.width()).arg(v.height());
    return true;
}

Property* createWidthProperty(QObject *parent, PropertyQSizeBase *propertySize)
{
    PropertyIntCallback *widthProperty = new PropertyIntCallback(parent);
    widthProperty->setName(QObject::tr("Width"));
    widthProperty->setDescription(QObject::tr("Width of the %1.").arg(propertySize->name()));
    widthProperty->setCallbackValueGet([propertySize]()->int { return propertySize->value().width(); });
    widthProperty->setCallbackValueSet([propertySize](int newWidth) {
        QSize size = propertySize->value();
        size.setWidth(newWidth);
        propertySize->setValue(size);
    });
    PropertyBase::connectMasterState(*propertySize, *widthProperty);

    return widthProperty;
}

Property* createHeightProperty(QObject *parent, PropertyQSizeBase *propertySize)
{
    PropertyIntCallback *heightProperty = new PropertyIntCallback(parent);
    heightProperty->setName(QObject::tr("Height"));
    heightProperty->setDescription(QObject::tr("Height of the %1.").arg(propertySize->name()));
    heightProperty->setCallbackValueGet([propertySize]()->int { return propertySize->value().height(); });
    heightProperty->setCallbackValueSet([propertySize](int newHeight) {
        QSize size = propertySize->value();
        size.setHeight(newHeight);
        propertySize->setValue(size);
    });
    PropertyBase::connectMasterState(*propertySize, *heightProperty);

    return heightProperty;
}

} // end namespace Qtinuum
