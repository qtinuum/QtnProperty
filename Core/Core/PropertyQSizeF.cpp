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

#include "PropertyQSizeF.h"
#include "PropertyDouble.h"

QtnPropertyQSizeFBase::QtnPropertyQSizeFBase(QObject *parent)
    : QtnSinglePropertyBase<QSizeF>(parent)
{
}

bool QtnPropertyQSizeFBase::fromStrImpl(const QString& str)
{
    static QRegExp parserSize("^\\s*QSizeF\\s*\\(([^\\)]+)\\)\\s*$", Qt::CaseInsensitive);
    static QRegExp parserParams("^\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*,\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*$", Qt::CaseInsensitive);

    if (!parserSize.exactMatch(str))
        return false;

    QStringList params = parserSize.capturedTexts();
    if (params.size() != 2)
        return false;

    if (!parserParams.exactMatch(params[1]))
        return false;

    params = parserParams.capturedTexts();
    if (params.size() != 5)
        return false;

    bool ok = false;
    qreal width = params[1].toDouble(&ok);
    if (!ok)
        return false;

    qreal height = params[3].toDouble(&ok);
    if (!ok)
        return false;

    setValue(QSizeF(width, height));
    return true;
}

bool QtnPropertyQSizeFBase::toStrImpl(QString& str) const
{
    QSizeF v = value();
    str = QString("QSizeF(%1, %2)").arg(v.width()).arg(v.height());
    return true;
}

QtnProperty* qtnCreateWidthProperty(QObject *parent, QtnPropertyQSizeFBase *propertySize)
{
    QtnPropertyDoubleCallback *widthProperty = new QtnPropertyDoubleCallback(parent);
    widthProperty->setName(QObject::tr("Width"));
    widthProperty->setDescription(QObject::tr("Width of the %1.").arg(propertySize->name()));
    widthProperty->setCallbackValueGet([propertySize]()->qreal { return propertySize->value().width(); });
    widthProperty->setCallbackValueSet([propertySize](qreal newWidth) {
        QSizeF size = propertySize->value();
        size.setWidth(newWidth);
        propertySize->setValue(size);
    });
    QtnPropertyBase::connectMasterSignals(*propertySize, *widthProperty);

    return widthProperty;
}

QtnProperty* qtnCreateHeightProperty(QObject *parent, QtnPropertyQSizeFBase *propertySize)
{
    QtnPropertyDoubleCallback *heightProperty = new QtnPropertyDoubleCallback(parent);
    heightProperty->setName(QObject::tr("Height"));
    heightProperty->setDescription(QObject::tr("Height of the %1.").arg(propertySize->name()));
    heightProperty->setCallbackValueGet([propertySize]()->qreal { return propertySize->value().height(); });
    heightProperty->setCallbackValueSet([propertySize](qreal newHeight) {
        QSizeF size = propertySize->value();
        size.setHeight(newHeight);
        propertySize->setValue(size);
    });
    QtnPropertyBase::connectMasterSignals(*propertySize, *heightProperty);

    return heightProperty;
}

