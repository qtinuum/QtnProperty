/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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

#include "PropertyQColor.h"
#include "../Core/PropertyInt.h"

bool QtnPropertyQColorBase::fromStrImpl(const QString& str)
{
    QColor color(str.trimmed());
    if (!color.isValid())
        return false;

    return setValue(color);
}

bool QtnPropertyQColorBase::toStrImpl(QString& str) const
{
    QColor v = value();

    if (v.alpha() < 255)
        str = v.name(QColor::HexArgb);
    else
        str = v.name();

    return true;
}

QtnProperty* qtnCreateRedProperty(QObject *parent, QtnPropertyQColorBase *propertyColor)
{
    QtnPropertyIntCallback *redProperty = new QtnPropertyIntCallback(parent);
    redProperty->setName(QObject::tr("Red"));
    redProperty->setDescription(QObject::tr("Red component of the %1.").arg(propertyColor->name()));
    redProperty->setCallbackValueGet([propertyColor]()->int { return propertyColor->value().red(); });
    redProperty->setCallbackValueSet([propertyColor](int newRed) {
        QColor color = propertyColor->value();
        color.setRed(newRed);
        propertyColor->setValue(color);
    });
    redProperty->setMinValue(0);
    redProperty->setMaxValue(255);
    QtnPropertyBase::connectMasterState(*propertyColor, *redProperty);

    return redProperty;
}

QtnProperty* qtnCreateGreenProperty(QObject *parent, QtnPropertyQColorBase *propertyColor)
{
    QtnPropertyIntCallback *greenProperty = new QtnPropertyIntCallback(parent);
    greenProperty->setName(QObject::tr("Green"));
    greenProperty->setDescription(QObject::tr("Green component of the %1.").arg(propertyColor->name()));
    greenProperty->setCallbackValueGet([propertyColor]()->int { return propertyColor->value().green(); });
    greenProperty->setCallbackValueSet([propertyColor](int newGreen) {
        QColor color = propertyColor->value();
        color.setGreen(newGreen);
        propertyColor->setValue(color);
    });
    greenProperty->setMinValue(0);
    greenProperty->setMaxValue(255);
    QtnPropertyBase::connectMasterState(*propertyColor, *greenProperty);

    return greenProperty;
}

QtnProperty* qtnCreateBlueProperty(QObject *parent, QtnPropertyQColorBase *propertyColor)
{
    QtnPropertyIntCallback *blueProperty = new QtnPropertyIntCallback(parent);
    blueProperty->setName(QObject::tr("Blue"));
    blueProperty->setDescription(QObject::tr("Blue component of the %1.").arg(propertyColor->name()));
    blueProperty->setCallbackValueGet([propertyColor]()->int { return propertyColor->value().blue(); });
    blueProperty->setCallbackValueSet([propertyColor](int newBlue) {
        QColor color = propertyColor->value();
        color.setBlue(newBlue);
        propertyColor->setValue(color);
    });
    blueProperty->setMinValue(0);
    blueProperty->setMaxValue(255);
    QtnPropertyBase::connectMasterState(*propertyColor, *blueProperty);

    return blueProperty;
}
