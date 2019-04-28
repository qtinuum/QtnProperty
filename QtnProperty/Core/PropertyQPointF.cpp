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

#include "PropertyQPointF.h"
#include "PropertyDouble.h"

QtnPropertyQPointFBase::QtnPropertyQPointFBase(QObject *parent)
    : QtnSinglePropertyBase<QPointF>(parent)
{
}
#include <QDebug>
bool QtnPropertyQPointFBase::fromStrImpl(const QString& str)
{
    static QRegExp parserPoint("^\\s*QPointF\\s*\\(([^\\)]+)\\)\\s*$", Qt::CaseInsensitive);
    static QRegExp parserParams("^\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*,\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*$", Qt::CaseInsensitive);

    if (!parserPoint.exactMatch(str))
        return false;

    QStringList params = parserPoint.capturedTexts();

    if (params.size() != 2)
        return false;

    if (!parserParams.exactMatch(params[1]))
        return false;

    params = parserParams.capturedTexts();

    if (params.size() != 5)
        return false;

    bool ok = false;
    qreal x = params[1].toDouble(&ok);
    if (!ok)
        return false;

    qreal y = params[3].toDouble(&ok);
    if (!ok)
        return false;

    setValue(QPointF(x, y));
    return true;
}

bool QtnPropertyQPointFBase::toStrImpl(QString& str) const
{
    QPointF v = value();
    str = QString("QPointF(%1, %2)").arg(v.x()).arg(v.y());
    return true;
}

QtnProperty* qtnCreateXProperty(QObject *parent, QtnPropertyQPointFBase *propertyPoint)
{
    QtnPropertyDoubleCallback *xProperty = new QtnPropertyDoubleCallback(parent);
    xProperty->setName(QObject::tr("X"));
    xProperty->setDescription(QObject::tr("X coordinate of the %1.").arg(propertyPoint->name()));
    xProperty->setCallbackValueGet([propertyPoint]()->qreal { return propertyPoint->value().x(); });
    xProperty->setCallbackValueSet([propertyPoint](qreal newX) {
        QPointF point = propertyPoint->value();
        point.setX(newX);
        propertyPoint->setValue(point);
    });
    QtnPropertyBase::connectMasterSignals(*propertyPoint, *xProperty);

    return xProperty;
}

QtnProperty* qtnCreateYProperty(QObject *parent, QtnPropertyQPointFBase *propertyPoint)
{
    QtnPropertyDoubleCallback *yProperty = new QtnPropertyDoubleCallback(parent);
    yProperty->setName(QObject::tr("Y"));
    yProperty->setDescription(QObject::tr("Y coordinate of the %1.").arg(propertyPoint->name()));
    yProperty->setCallbackValueGet([propertyPoint]()->qreal { return propertyPoint->value().y(); });
    yProperty->setCallbackValueSet([propertyPoint](qreal newY) {
        QPointF point = propertyPoint->value();
        point.setY(newY);
        propertyPoint->setValue(point);
    });
    QtnPropertyBase::connectMasterSignals(*propertyPoint, *yProperty);

    return yProperty;
}
