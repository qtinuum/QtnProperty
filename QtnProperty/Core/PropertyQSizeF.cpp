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
#include <QRegularExpression>

QtnPropertyQSizeFBase::QtnPropertyQSizeFBase(QObject *parent)
	: ParentClass(parent)
{
}

QtnProperty *QtnPropertyQSizeFBase::createWidthProperty()
{
	return createFieldProperty(&QSizeF::width, &QSizeF::setWidth,
		QtnPropertyQSize::widthKey(), QtnPropertyQSize::widthDisplayName(),
		QtnPropertyQSize::widthDescriptionFmt());
}

QtnProperty *QtnPropertyQSizeFBase::createHeightProperty()
{
	return createFieldProperty(&QSizeF::height, &QSizeF::setHeight,
		QtnPropertyQSize::heightKey(), QtnPropertyQSize::heightDisplayName(),
		QtnPropertyQSize::heightDescriptionFmt());
}

bool QtnPropertyQSizeFBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
    static QRegularExpression parserSize(
        "^\\s*QSizeF\\s*\\(([^\\)]+)\\)\\s*$", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = parserSize.match(str);
    if (!match.hasMatch())
		return false;

    QStringList params = match.capturedTexts();

	if (params.size() != 2)
		return false;

    static QRegularExpression parserParams(
		"^\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*,\\s*(\\-?\\d+(\\.\\d{0,})?)\\s*$",
        QRegularExpression::CaseInsensitiveOption);

    match = parserParams.match(params[1]);
    if (!match.hasMatch())
		return false;

    params = match.capturedTexts();

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
