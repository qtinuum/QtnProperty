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

#include "PropertyQSizeF.h"

#include "PropertyQSize.h"

QtnPropertyQSizeFBase::QtnPropertyQSizeFBase(QObject *parent)
	: ParentClass(parent)
{
}

QtnProperty *QtnPropertyQSizeFBase::createWidthProperty()
{
	return createFieldProperty(QtnPropertyQSize::widthKey(),
		QtnPropertyQSize::widthDisplayName(),
		QtnPropertyQSize::widthDescriptionFmt(), &QSizeF::width,
		&QSizeF::setWidth);
}

QtnProperty *QtnPropertyQSizeFBase::createHeightProperty()
{
	return createFieldProperty(QtnPropertyQSize::heightKey(),
		QtnPropertyQSize::heightDisplayName(),
		QtnPropertyQSize::heightDescriptionFmt(), &QSizeF::height,
		&QSizeF::setHeight);
}

bool QtnPropertyQSizeFBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	static QRegExp parserSize(
		"^\\s*QSizeF\\s*\\(([^\\)]+)\\)\\s*$", Qt::CaseInsensitive);

	if (!parserSize.exactMatch(str))
		return false;

	QStringList params = parserSize.capturedTexts();

	if (params.size() != 2)
		return false;

	static QRegExp parserParams(
		"^\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*,\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*$",
		Qt::CaseInsensitive);

	if (!parserParams.exactMatch(params[1]))
		return false;

	params = parserParams.capturedTexts();

	if (params.size() != 5)
		return false;

	bool ok = false;
	double width = params[1].toDouble(&ok);

	if (!ok)
		return false;

	double height = params[3].toDouble(&ok);

	if (!ok)
		return false;

	return setValue(QSizeF(width, height), reason);
}

bool QtnPropertyQSizeFBase::toStrImpl(QString &str) const
{
	QSizeF v = value();
	str = QString("QSizeF(%1, %2)").arg(v.width()).arg(v.height());
	return true;
}

QtnPropertyQSizeF::QtnPropertyQSizeF(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQSizeFBase>(parent)
{
}

QtnPropertyQSizeFCallback::QtnPropertyQSizeFCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQSizeFBase>(parent)
{
}
