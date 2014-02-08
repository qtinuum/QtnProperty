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

#include "PropertyQRect.h"
#include "PropertyInt.h"

QtnPropertyQRectBase::QtnPropertyQRectBase(QObject *parent)
    : QtnSinglePropertyBase<QRect>(parent)
{
    addState(QtnPropertyStateCollapsed);
}

bool QtnPropertyQRectBase::fromStrImpl(const QString& str)
{
    static QRegExp parserRect("^\\s*QRect\\s*\\(([^\\)]+)\\)\\s*$", Qt::CaseInsensitive);
    static QRegExp parserParams("^\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*$", Qt::CaseInsensitive);

    if (!parserRect.exactMatch(str))
        return false;

    QStringList params = parserRect.capturedTexts();
    if (params.size() != 2)
        return false;

    if (!parserParams.exactMatch(params[1]))
        return false;

    params = parserParams.capturedTexts();
    if (params.size() != 5)
        return false;

    bool ok = false;
    int left = params[1].toInt(&ok);
    if (!ok)
        return false;

    int top = params[2].toInt(&ok);
    if (!ok)
        return false;

    int width = params[3].toInt(&ok);
    if (!ok)
        return false;

    int height = params[4].toInt(&ok);
    if (!ok)
        return false;

    return setValue(QRect(left, top, width, height));
}

bool QtnPropertyQRectBase::toStrImpl(QString& str) const
{
    QRect v = value();
    str = QString("QRect(%1, %2, %3, %4)").arg(v.left()).arg(v.top()).arg(v.width()).arg(v.height());
    return true;
}

QtnProperty* qtnCreateLeftProperty(QObject *parent, QtnPropertyQRectBase *propertyRect)
{
    QtnPropertyIntCallback *leftProperty = new QtnPropertyIntCallback(parent);
    leftProperty->setName(QObject::tr("Left"));
    leftProperty->setDescription(QObject::tr("Left side of the %1.").arg(propertyRect->name()));
    leftProperty->setCallbackValueGet([propertyRect]()->int { return propertyRect->value().left(); });
    leftProperty->setCallbackValueSet([propertyRect](int newLeft) {
        QRect rect = propertyRect->value();
        rect.setLeft(newLeft);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterState(*propertyRect, *leftProperty);

    return leftProperty;
}

QtnProperty* qtnCreateRightProperty(QObject *parent, QtnPropertyQRectBase *propertyRect)
{
    QtnPropertyIntCallback *rightProperty = new QtnPropertyIntCallback(parent);
    rightProperty->setName(QObject::tr("Right"));
    rightProperty->setDescription(QObject::tr("Right side of the %1.").arg(propertyRect->name()));
    rightProperty->setCallbackValueGet([propertyRect]()->int { return propertyRect->value().right(); });
    rightProperty->setCallbackValueSet([propertyRect](int newRight) {
        QRect rect = propertyRect->value();
        rect.setRight(newRight);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterState(*propertyRect, *rightProperty);

    return rightProperty;
}

QtnProperty* qtnCreateTopProperty(QObject *parent, QtnPropertyQRectBase *propertyRect)
{
    QtnPropertyIntCallback *topProperty = new QtnPropertyIntCallback(parent);
    topProperty->setName(QObject::tr("Top"));
    topProperty->setDescription(QObject::tr("Top of the %1.").arg(propertyRect->name()));
    topProperty->setCallbackValueGet([propertyRect]()->int { return propertyRect->value().top(); });
    topProperty->setCallbackValueSet([propertyRect](int newTop) {
        QRect rect = propertyRect->value();
        rect.setTop(newTop);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterState(*propertyRect, *topProperty);

    return topProperty;
}

QtnProperty* qtnCreateBottomProperty(QObject *parent, QtnPropertyQRectBase *propertyRect)
{
    QtnPropertyIntCallback *bottomProperty = new QtnPropertyIntCallback(parent);
    bottomProperty->setName(QObject::tr("Bottom"));
    bottomProperty->setDescription(QObject::tr("Bottom of the %1.").arg(propertyRect->name()));
    bottomProperty->setCallbackValueGet([propertyRect]()->int { return propertyRect->value().bottom(); });
    bottomProperty->setCallbackValueSet([propertyRect](int newBottom) {
        QRect rect = propertyRect->value();
        rect.setBottom(newBottom);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterState(*propertyRect, *bottomProperty);

    return bottomProperty;
}

QtnProperty* qtnCreateWidthProperty(QObject *parent, QtnPropertyQRectBase *propertyRect)
{
    QtnPropertyIntCallback *widthProperty = new QtnPropertyIntCallback(parent);
    widthProperty->setName(QObject::tr("Width"));
    widthProperty->setDescription(QObject::tr("Width of the %1.").arg(propertyRect->name()));
    widthProperty->setCallbackValueGet([propertyRect]()->int { return propertyRect->value().width(); });
    widthProperty->setCallbackValueSet([propertyRect](int newWidth) {
        QRect rect = propertyRect->value();
        rect.setWidth(newWidth);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterState(*propertyRect, *widthProperty);

    return widthProperty;
}

QtnProperty* qtnCreateHeightProperty(QObject *parent, QtnPropertyQRectBase *propertyRect)
{
    QtnPropertyIntCallback *heightProperty = new QtnPropertyIntCallback(parent);
    heightProperty->setName(QObject::tr("Height"));
    heightProperty->setDescription(QObject::tr("Height of the %1.").arg(propertyRect->name()));
    heightProperty->setCallbackValueGet([propertyRect]()->int { return propertyRect->value().height(); });
    heightProperty->setCallbackValueSet([propertyRect](int newHeight) {
        QRect rect = propertyRect->value();
        rect.setHeight(newHeight);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterState(*propertyRect, *heightProperty);

    return heightProperty;
}
