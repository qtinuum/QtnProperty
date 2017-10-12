/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyQPoint.h"
#include "PropertyInt.h"

QtnPropertyQPointBase::QtnPropertyQPointBase(QObject *parent)
	: QtnSinglePropertyBase<QPoint>(parent)
{
	addState(QtnPropertyStateCollapsed);
}

bool QtnPropertyQPointBase::fromStrImpl(const QString &str, bool edit)
{
	static QRegExp parserPoint(
		"^\\s*QPoint\\s*\\(([^\\)]+)\\)\\s*$", Qt::CaseInsensitive);
	static QRegExp parserParams(
		"^\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*$", Qt::CaseInsensitive);

	if (!parserPoint.exactMatch(str))
		return false;

	QStringList params = parserPoint.capturedTexts();

	if (params.size() != 2)
		return false;

	if (!parserParams.exactMatch(params[1]))
		return false;

	params = parserParams.capturedTexts();

	if (params.size() != 3)
		return false;

	bool ok = false;
	int x = params[1].toInt(&ok);

	if (!ok)
		return false;

	int y = params[2].toInt(&ok);

	if (!ok)
		return false;

	;

	return setValue(QPoint(x, y), edit);
}

bool QtnPropertyQPointBase::toStrImpl(QString &str) const
{
	QPoint v = value();
	str = QString("QPoint(%1, %2)").arg(v.x()).arg(v.y());
	return true;
}

QtnProperty *qtnCreateXProperty(
	QObject *parent, QtnPropertyQPointBase *propertyPoint)
{
	QtnPropertyIntCallback *xProperty = new QtnPropertyIntCallback(parent);
	xProperty->setName(QtnPropertyQPoint::tr("X"));
	xProperty->setDescription(
		QtnPropertyQPoint::tr("X coordinate of the %1").arg(
			propertyPoint->name()));
	xProperty->setCallbackValueGet(
		[propertyPoint]() -> int
	{
		return propertyPoint->value().x();
	});
	xProperty->setCallbackValueSet(
		[propertyPoint](int newX)
	{
		QPoint point = propertyPoint->value();
		point.setX(newX);
		propertyPoint->setValue(point);
	});

	return xProperty;
}

QtnProperty *qtnCreateYProperty(
	QObject *parent, QtnPropertyQPointBase *propertyPoint)
{
	QtnPropertyIntCallback *yProperty = new QtnPropertyIntCallback(parent);
	yProperty->setName(QtnPropertyQPoint::tr("Y"));
	yProperty->setDescription(
		QtnPropertyQPoint::tr("Y coordinate of the %1").arg(
			propertyPoint->name()));
	yProperty->setCallbackValueGet(
		[propertyPoint]() -> int
	{
		return propertyPoint->value().y();
	});
	yProperty->setCallbackValueSet(
		[propertyPoint](int newY)
	{
		QPoint point = propertyPoint->value();
		point.setY(newY);
		propertyPoint->setValue(point);
	});

	return yProperty;
}

QtnPropertyQPoint::QtnPropertyQPoint(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQPointBase>(parent)
{
}

QString QtnPropertyQPoint::getToStringFormat()
{
	return tr("%1, %2");
}

QtnPropertyQPointCallback::QtnPropertyQPointCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQPointBase>(parent)
{
}
