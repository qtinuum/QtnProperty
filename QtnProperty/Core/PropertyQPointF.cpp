/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include "PropertyQPointF.h"

#include "PropertyQPoint.h"

QtnPropertyQPointFBase::QtnPropertyQPointFBase(QObject *parent)
	: ParentClass(parent)
{
}

QtnProperty *QtnPropertyQPointFBase::createXProperty()
{
	return createFieldProperty(QtnPropertyQPoint::xKey(),
		QtnPropertyQPoint::xDisplayName(), QtnPropertyQPoint::xDescriptionFmt(),
		&QPointF::x, &QPointF::setX);
}

QtnProperty *QtnPropertyQPointFBase::createYProperty()
{
	return createFieldProperty(QtnPropertyQPoint::yKey(),
		QtnPropertyQPoint::yDisplayName(), QtnPropertyQPoint::yDescriptionFmt(),
		&QPointF::y, &QPointF::setY);
}

bool QtnPropertyQPointFBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	static QRegExp parserPoint(
		QLatin1String("^\\s*QPointF\\s*\\(([^\\)]+)\\)\\s*$"),
		Qt::CaseInsensitive);

	if (!parserPoint.exactMatch(str))
		return false;

	QStringList params = parserPoint.capturedTexts();

	if (params.size() != 2)
		return false;

	static QRegExp parserParams(
		QLatin1String("^\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*$"),
		Qt::CaseInsensitive);

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

	return setValue(QPointF(x, y), reason);
}

bool QtnPropertyQPointFBase::toStrImpl(QString &str) const
{
	QPointF v = value();
	str = QStringLiteral("QPointF(%1, %2)").arg(v.x()).arg(v.y());
	return true;
}

QtnPropertyQPointF::QtnPropertyQPointF(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQPointFBase>(parent)
{
}

QtnPropertyQPointFCallback::QtnPropertyQPointFCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQPointFBase>(parent)
{
}
