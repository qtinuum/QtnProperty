/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "PropertyQRectF.h"
#include "PropertyDouble.h"

QtnPropertyQRectFBase::QtnPropertyQRectFBase(QObject *parent)
    : QtnSinglePropertyBase<QRectF>(parent)
{
}

bool QtnPropertyQRectFBase::fromStrImpl(const QString& str)
{
    static QRegExp parserRect("^\\s*QRectF\\s*\\(([^\\)]+)\\)\\s*$", Qt::CaseInsensitive);
    static QRegExp parserParams("^\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*,\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*,\\s*(\\d+(\\.\\d{0,})?)\\s*,\\s*(\\d+(\\.\\d{0,})?)\\s*$", Qt::CaseInsensitive);

    if (!parserRect.exactMatch(str))
        return false;

    QStringList params = parserRect.capturedTexts();
    if (params.size() != 2)
        return false;

    if (!parserParams.exactMatch(params[1]))
        return false;

    params = parserParams.capturedTexts();
    if (params.size() != 9)
        return false;

    bool ok = false;
    qreal left = params[1].toDouble(&ok);
    if (!ok)
        return false;

    qreal top = params[3].toDouble(&ok);
    if (!ok)
        return false;

    qreal width = params[5].toDouble(&ok);
    if (!ok)
        return false;

    qreal height = params[7].toDouble(&ok);
    if (!ok)
        return false;

    setValue(QRectF(left, top, width, height));
    return true;
}

bool QtnPropertyQRectFBase::toStrImpl(QString& str) const
{
    QRectF v = value();
    str = QString("QRectF(%1, %2, %3, %4)").arg(v.left()).arg(v.top()).arg(v.width()).arg(v.height());
    return true;
}

QtnProperty* qtnCreateLeftProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect)
{
    QtnPropertyDoubleCallback *leftProperty = new QtnPropertyDoubleCallback(parent);
    leftProperty->setName(QObject::tr("Left"));
    leftProperty->setDescription(QObject::tr("Left side of the %1.").arg(propertyRect->name()));
    leftProperty->setCallbackValueGet([propertyRect]()->qreal { return propertyRect->value().left(); });
    leftProperty->setCallbackValueSet([propertyRect](qreal newLeft) {
        QRectF rect = propertyRect->value();
        rect.setLeft(newLeft);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterSignals(*propertyRect, *leftProperty);

    return leftProperty;
}

QtnProperty* qtnCreateRightProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect)
{
    QtnPropertyDoubleCallback *rightProperty = new QtnPropertyDoubleCallback(parent);
    rightProperty->setName(QObject::tr("Right"));
    rightProperty->setDescription(QObject::tr("Right side of the %1.").arg(propertyRect->name()));
    rightProperty->setCallbackValueGet([propertyRect]()->qreal { return propertyRect->value().right(); });
    rightProperty->setCallbackValueSet([propertyRect](qreal newRight) {
        QRectF rect = propertyRect->value();
        rect.setRight(newRight);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterSignals(*propertyRect, *rightProperty);

    return rightProperty;
}

QtnProperty* qtnCreateTopProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect)
{
    QtnPropertyDoubleCallback *topProperty = new QtnPropertyDoubleCallback(parent);
    topProperty->setName(QObject::tr("Top"));
    topProperty->setDescription(QObject::tr("Top of the %1.").arg(propertyRect->name()));
    topProperty->setCallbackValueGet([propertyRect]()->qreal { return propertyRect->value().top(); });
    topProperty->setCallbackValueSet([propertyRect](qreal newTop) {
        QRectF rect = propertyRect->value();
        rect.setTop(newTop);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterSignals(*propertyRect, *topProperty);

    return topProperty;
}

QtnProperty* qtnCreateBottomProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect)
{
    QtnPropertyDoubleCallback *bottomProperty = new QtnPropertyDoubleCallback(parent);
    bottomProperty->setName(QObject::tr("Bottom"));
    bottomProperty->setDescription(QObject::tr("Bottom of the %1.").arg(propertyRect->name()));
    bottomProperty->setCallbackValueGet([propertyRect]()->qreal { return propertyRect->value().bottom(); });
    bottomProperty->setCallbackValueSet([propertyRect](qreal newBottom) {
        QRectF rect = propertyRect->value();
        rect.setBottom(newBottom);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterSignals(*propertyRect, *bottomProperty);

    return bottomProperty;
}

QtnProperty* qtnCreateWidthProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect)
{
    QtnPropertyDoubleCallback *widthProperty = new QtnPropertyDoubleCallback(parent);
    widthProperty->setName(QObject::tr("Width"));
    widthProperty->setDescription(QObject::tr("Width of the %1.").arg(propertyRect->name()));
    widthProperty->setCallbackValueGet([propertyRect]()->qreal { return propertyRect->value().width(); });
    widthProperty->setCallbackValueSet([propertyRect](qreal newWidth) {
        QRectF rect = propertyRect->value();
        rect.setWidth(newWidth);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterSignals(*propertyRect, *widthProperty);

    return widthProperty;
}

QtnProperty* qtnCreateHeightProperty(QObject *parent, QtnPropertyQRectFBase *propertyRect)
{
    QtnPropertyDoubleCallback *heightProperty = new QtnPropertyDoubleCallback(parent);
    heightProperty->setName(QObject::tr("Height"));
    heightProperty->setDescription(QObject::tr("Height of the %1.").arg(propertyRect->name()));
    heightProperty->setCallbackValueGet([propertyRect]()->qreal { return propertyRect->value().height(); });
    heightProperty->setCallbackValueSet([propertyRect](qreal newHeight) {
        QRectF rect = propertyRect->value();
        rect.setHeight(newHeight);
        propertyRect->setValue(rect);
    });
    QtnPropertyBase::connectMasterSignals(*propertyRect, *heightProperty);

    return heightProperty;
}
