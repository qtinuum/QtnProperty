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

#include "PropertyQPoint.h"

QtnPropertyQPointBase::QtnPropertyQPointBase(QObject *parent)
	: ParentClass(parent)
{
}

QtnProperty *QtnPropertyQPointBase::createXProperty()
{
	return createFieldProperty(&QPoint::x, &QPoint::setX,
		QtnPropertyQPoint::xKey(), QtnPropertyQPoint::xDisplayName(),
		QtnPropertyQPoint::xDescriptionFmt());
}

QtnProperty *QtnPropertyQPointBase::createYProperty()
{
	return createFieldProperty(&QPoint::y, &QPoint::setY,
		QtnPropertyQPoint::yKey(), QtnPropertyQPoint::yDisplayName(),
		QtnPropertyQPoint::yDescriptionFmt());
}

bool QtnPropertyQPointBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	static QRegExp parserPoint(
		"^\\s*QPoint\\s*\\(([^\\)]+)\\)\\s*$", Qt::CaseInsensitive);

	if (!parserPoint.exactMatch(str))
		return false;

	QStringList params = parserPoint.capturedTexts();

	if (params.size() != 2)
		return false;

	static QRegExp parserParams(
		"^\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*$", Qt::CaseInsensitive);

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

	return setValue(QPoint(x, y), reason);
}

bool QtnPropertyQPointBase::toStrImpl(QString &str) const
{
	QPoint v = value();
	str = QStringLiteral("QPoint(%1, %2)").arg(v.x()).arg(v.y());
	return true;
}

QtnPropertyQPoint::QtnPropertyQPoint(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQPointBase>(parent)
{
}

QString QtnPropertyQPoint::getToStringFormat()
{
	return tr("[%1, %2]");
}

QString QtnPropertyQPoint::xKey()
{
	return QStringLiteral("x");
}

QString QtnPropertyQPoint::xDisplayName()
{
	return tr("X");
}

QString QtnPropertyQPoint::xDescriptionFmt()
{
	return tr("X of the %1");
}

QString QtnPropertyQPoint::yKey()
{
	return QStringLiteral("y");
}

QString QtnPropertyQPoint::yDisplayName()
{
	return tr("Y");
}

QString QtnPropertyQPoint::yDescriptionFmt()
{
	return tr("Y of the %1");
}

QtnPropertyQPointCallback::QtnPropertyQPointCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQPointBase>(parent)
{
}
