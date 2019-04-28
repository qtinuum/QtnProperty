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

#include "PropertyQSize.h"
#include "PropertyInt.h"

QtnPropertyQSizeBase::QtnPropertyQSizeBase(QObject *parent)
    : QtnSinglePropertyBase<QSize>(parent)
{
}

bool QtnPropertyQSizeBase::fromStrImpl(const QString& str)
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

    setValue(QSize(width, height));
    return true;
}

bool QtnPropertyQSizeBase::toStrImpl(QString& str) const
{
    QSize v = value();
    str = QString("QSize(%1, %2)").arg(v.width()).arg(v.height());
    return true;
}

QtnProperty* qtnCreateWidthProperty(QObject *parent, QtnPropertyQSizeBase *propertySize)
{
    QtnPropertyIntCallback *widthProperty = new QtnPropertyIntCallback(parent);
    widthProperty->setName(QObject::tr("Width"));
    widthProperty->setDescription(QObject::tr("Width of the %1.").arg(propertySize->name()));
    widthProperty->setCallbackValueGet([propertySize]()->int { return propertySize->value().width(); });
    widthProperty->setCallbackValueSet([propertySize](int newWidth) {
        QSize size = propertySize->value();
        size.setWidth(newWidth);
        propertySize->setValue(size);
    });
    QtnPropertyBase::connectMasterSignals(*propertySize, *widthProperty);

    return widthProperty;
}

QtnProperty* qtnCreateHeightProperty(QObject *parent, QtnPropertyQSizeBase *propertySize)
{
    QtnPropertyIntCallback *heightProperty = new QtnPropertyIntCallback(parent);
    heightProperty->setName(QObject::tr("Height"));
    heightProperty->setDescription(QObject::tr("Height of the %1.").arg(propertySize->name()));
    heightProperty->setCallbackValueGet([propertySize]()->int { return propertySize->value().height(); });
    heightProperty->setCallbackValueSet([propertySize](int newHeight) {
        QSize size = propertySize->value();
        size.setHeight(newHeight);
        propertySize->setValue(size);
    });
    QtnPropertyBase::connectMasterSignals(*propertySize, *heightProperty);

    return heightProperty;
}

