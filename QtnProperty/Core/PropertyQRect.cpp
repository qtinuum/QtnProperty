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

#include "PropertyQRect.h"
#include "PropertyQSize.h"
#include <QRegularExpression>

QtnProperty *QtnPropertyQRectBase::createLeftProperty(bool move)
{
	return createFieldProperty(&QRect::left,
		move ? &QRect::moveLeft : &QRect::setLeft, QtnPropertyQRect::leftKey(),
		QtnPropertyQRect::leftString(), QtnPropertyQRect::leftDescriptionFmt());
}

QtnProperty *QtnPropertyQRectBase::createTopProperty(bool move)
{
	return createFieldProperty(&QRect::top,
		move ? &QRect::moveTop : &QRect::setTop, QtnPropertyQRect::topKey(),
		QtnPropertyQRect::topString(), QtnPropertyQRect::topDescriptionFmt());
}

QtnProperty *QtnPropertyQRectBase::createRightProperty(bool move)
{
	return createFieldProperty(&QRect::right,
		move ? &QRect::moveRight : &QRect::setRight,
		QtnPropertyQRect::rightKey(), QtnPropertyQRect::rightString(),
		QtnPropertyQRect::rightDescriptionFmt());
}

QtnProperty *QtnPropertyQRectBase::createBottomProperty(bool move)
{
	return createFieldProperty(&QRect::bottom,
		move ? &QRect::moveBottom : &QRect::setBottom,
		QtnPropertyQRect::bottomKey(), QtnPropertyQRect::bottomString(),
		QtnPropertyQRect::bottomDescriptionFmt());
}

QtnProperty *QtnPropertyQRectBase::createWidthProperty()
{
	return createFieldProperty(&QRect::width, &QRect::setWidth,
		QtnPropertyQSize::widthKey(), QtnPropertyQSize::widthDisplayName(),
		QtnPropertyQSize::widthDescriptionFmt());
}

QtnProperty *QtnPropertyQRectBase::createHeightProperty()
{
	return createFieldProperty(&QRect::height, &QRect::setHeight,
		QtnPropertyQSize::heightKey(), QtnPropertyQSize::heightDisplayName(),
		QtnPropertyQSize::heightDescriptionFmt());
}

QByteArray QtnPropertyQRect::delegateName(bool coordinateMode)
{
	if (coordinateMode)
	{
		return QByteArrayLiteral("LTRB");
	} else
	{
		return QByteArrayLiteral("LTWH");
	}
}

QtnPropertyQRectBase::QtnPropertyQRectBase(QObject *parent)
	: ParentClass(parent)
{
}

bool QtnPropertyQRectBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
    static QRegularExpression parserRect(
        "^\\s*QRect\\s*\\(([^\\)]+)\\)\\s*$", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = parserRect.match(str);
    if (!match.hasMatch())
		return false;

    QStringList params = match.capturedTexts();
	if (params.size() != 2)
		return false;

    static QRegularExpression parserParams(
        "^\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*$",
        QRegularExpression::CaseInsensitiveOption);

    match = parserParams.match(params[1]);
    if (!match.hasMatch())
		return false;

    params = match.capturedTexts();
	if (params.size() != 5)
		return false;

	bool ok = false;
	int left = params[1].toInt(&ok);
	if (!ok)
		return false;

	int top = params[2].toInt(&ok);
	if (!ok)
		return false;

	int width = params[3].toInt(&ok);
	if (!ok)
		return false;

	int height = params[4].toInt(&ok);
	if (!ok)
		return false;

	return setValue(QRect(left, top, width, height), reason);
}

bool QtnPropertyQRectBase::toStrImpl(QString &str) const
{
	auto v = value();

	str = QStringLiteral("QRect(%1, %2, %3, %4)")
			  .arg(v.left())
			  .arg(v.top())
			  .arg(v.width())
			  .arg(v.height());

	return true;
}

QtnPropertyQRect::QtnPropertyQRect(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQRectBase>(parent)
{
}

QString QtnPropertyQRect::getToStringFormat(bool coordinates)
{
	return coordinates ? tr("[(%1, %2), (%3, %4)]") : tr("[(%1, %2) %3 x %4]");
}

QString QtnPropertyQRect::leftKey()
{
	return QStringLiteral("left");
}

QString QtnPropertyQRect::leftString()
{
	return tr("Left");
}

QString QtnPropertyQRect::leftDescriptionFmt()
{
	return tr("Left position of the %1");
}

QString QtnPropertyQRect::topKey()
{
	return QStringLiteral("top");
}

QString QtnPropertyQRect::topString()
{
	return tr("Top");
}

QString QtnPropertyQRect::topDescriptionFmt()
{
	return tr("Top position of the %1");
}

QString QtnPropertyQRect::rightKey()
{
	return QStringLiteral("right");
}

QString QtnPropertyQRect::rightString()
{
	return tr("Right");
}

QString QtnPropertyQRect::rightDescriptionFmt()
{
	return tr("Right position of the %1");
}

QString QtnPropertyQRect::bottomKey()
{
	return QStringLiteral("bottom");
}

QString QtnPropertyQRect::bottomString()
{
	return tr("Bottom");
}

QString QtnPropertyQRect::bottomDescriptionFmt()
{
	return tr("Bottom position of the %1");
}

QtnPropertyQRectCallback::QtnPropertyQRectCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQRectBase>(parent)
{
}
