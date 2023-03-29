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

#include "PropertyQSize.h"
#include "PropertyInt.h"
#include <QRegularExpression>

QtnPropertyQSizeBase::QtnPropertyQSizeBase(QObject *parent)
	: ParentClass(parent)
{
}

QtnProperty *QtnPropertyQSizeBase::createWidthProperty()
{
	return createFieldProperty(&QSize::width, &QSize::setWidth,
		QtnPropertyQSize::widthKey(), QtnPropertyQSize::widthDisplayName(),
		QtnPropertyQSize::widthDescriptionFmt());
}

QtnProperty *QtnPropertyQSizeBase::createHeightProperty()
{
	return createFieldProperty(&QSize::height, &QSize::setHeight,
		QtnPropertyQSize::heightKey(), QtnPropertyQSize::heightDisplayName(),
		QtnPropertyQSize::heightDescriptionFmt());
}

bool QtnPropertyQSizeBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
    static QRegularExpression parserSize(
        "^\\s*QSize\\s*\\(([^\\)]+)\\)\\s*$", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = parserSize.match(str);
    if (!match.hasMatch())
		return false;

    QStringList params = match.capturedTexts();

	if (params.size() != 2)
		return false;

    static QRegularExpression parserParams(
        "^\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*$", QRegularExpression::CaseInsensitiveOption);

    match = parserParams.match(params[1]);
    if (!match.hasMatch())
		return false;

    params = match.capturedTexts();

	if (params.size() != 3)
		return false;

	bool ok = false;
	int width = params[1].toInt(&ok);

	if (!ok)
		return false;

	int height = params[2].toInt(&ok);

	if (!ok)
		return false;

	return setValue(QSize(width, height), reason);
}

bool QtnPropertyQSizeBase::toStrImpl(QString &str) const
{
	QSize v = value();
	str = QString("QSize(%1, %2)").arg(v.width()).arg(v.height());
	return true;
}

QtnPropertyQSize::QtnPropertyQSize(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQSizeBase>(parent)
{
}

QString QtnPropertyQSize::getToStringFormat()
{
	return tr("[%1 x %2]");
}

QString QtnPropertyQSize::widthKey()
{
	return QStringLiteral("width");
}

QString QtnPropertyQSize::widthDisplayName()
{
	return tr("Width");
}

QString QtnPropertyQSize::widthDescriptionFmt()
{
	return tr("Width of the %1");
}

QString QtnPropertyQSize::heightKey()
{
	return QStringLiteral("height");
}

QString QtnPropertyQSize::heightDisplayName()
{
	return tr("Height");
}

QString QtnPropertyQSize::heightDescriptionFmt()
{
	return tr("Height of the %1");
}

QtnPropertyQSizeCallback::QtnPropertyQSizeCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQSizeBase>(parent)
{
}
