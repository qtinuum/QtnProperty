/*******************************************************************************
Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
#include "PropertyInt.h"

QtnPropertyQRectBase::QtnPropertyQRectBase(QObject *parent)
	: QtnSinglePropertyBase<QRect>(parent)
{
	addState(QtnPropertyStateCollapsed);
}

bool QtnPropertyQRectBase::fromStrImpl(const QString &str, bool edit)
{
	static QRegExp parserRect(
		QStringLiteral("^\\s*QRect\\s*\\(([^\\)]+)\\)\\s*$"),
		Qt::CaseInsensitive);

	if (!parserRect.exactMatch(str))
		return false;

	QStringList params = parserRect.capturedTexts();

	if (params.size() != 2)
		return false;

	static QRegExp parserParams(QStringLiteral("\
^\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*$"),
		Qt::CaseInsensitive);

	if (!parserParams.exactMatch(params[1]))
		return false;

	params = parserParams.capturedTexts();

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

	return setValue(QRect(left, top, width, height), edit);
}

bool QtnPropertyQRectBase::toStrImpl(QString &str) const
{
	QRect v = value();
	str = QString("QRect(%1, %2, %3, %4)")
			  .arg(v.left())
			  .arg(v.top())
			  .arg(v.width())
			  .arg(v.height());
	return true;
}

QtnProperty *qtnCreateLeftProperty(
	QObject *parent, QtnPropertyQRectBase *propertyRect)
{
	QtnPropertyIntCallback *leftProperty = new QtnPropertyIntCallback(parent);
	leftProperty->setName(QtnPropertyQRect::tr("Left"));
	leftProperty->setDescription(
		QtnPropertyQRect::tr("Left side of the %1").arg(propertyRect->name()));
	leftProperty->setCallbackValueGet(
		[propertyRect]() -> int { return propertyRect->value().left(); });
	leftProperty->setCallbackValueSet([propertyRect](int newLeft) {
		QRect rect = propertyRect->value();
		rect.setLeft(newLeft);
		propertyRect->setValue(rect);
	});

	return leftProperty;
}

QtnProperty *qtnCreateRightProperty(
	QObject *parent, QtnPropertyQRectBase *propertyRect)
{
	QtnPropertyIntCallback *rightProperty = new QtnPropertyIntCallback(parent);
	rightProperty->setName(QtnPropertyQRect::tr("Right"));
	rightProperty->setDescription(
		QtnPropertyQRect::tr("Right side of the %1").arg(propertyRect->name()));
	rightProperty->setCallbackValueGet(
		[propertyRect]() -> int { return propertyRect->value().right(); });
	rightProperty->setCallbackValueSet([propertyRect](int newRight) {
		QRect rect = propertyRect->value();
		rect.setRight(newRight);
		propertyRect->setValue(rect);
	});

	return rightProperty;
}

QtnProperty *qtnCreateTopProperty(
	QObject *parent, QtnPropertyQRectBase *propertyRect)
{
	QtnPropertyIntCallback *topProperty = new QtnPropertyIntCallback(parent);
	topProperty->setName(QtnPropertyQRect::tr("Top"));
	topProperty->setDescription(
		QtnPropertyQRect::tr("Top of the %1").arg(propertyRect->name()));
	topProperty->setCallbackValueGet(
		[propertyRect]() -> int { return propertyRect->value().top(); });
	topProperty->setCallbackValueSet([propertyRect](int newTop) {
		QRect rect = propertyRect->value();
		rect.setTop(newTop);
		propertyRect->setValue(rect);
	});

	return topProperty;
}

QtnProperty *qtnCreateBottomProperty(
	QObject *parent, QtnPropertyQRectBase *propertyRect)
{
	QtnPropertyIntCallback *bottomProperty = new QtnPropertyIntCallback(parent);
	bottomProperty->setName(QtnPropertyQRect::tr("Bottom"));
	bottomProperty->setDescription(
		QtnPropertyQRect::tr("Bottom of the %1").arg(propertyRect->name()));
	bottomProperty->setCallbackValueGet(
		[propertyRect]() -> int { return propertyRect->value().bottom(); });
	bottomProperty->setCallbackValueSet([propertyRect](int newBottom) {
		QRect rect = propertyRect->value();
		rect.setBottom(newBottom);
		propertyRect->setValue(rect);
	});

	return bottomProperty;
}

QtnProperty *qtnCreateWidthProperty(
	QObject *parent, QtnPropertyQRectBase *propertyRect)
{
	QtnPropertyIntCallback *widthProperty = new QtnPropertyIntCallback(parent);
	widthProperty->setName(QtnPropertyQRect::tr("Width"));
	widthProperty->setDescription(
		QtnPropertyQRect::tr("Width of the %1").arg(propertyRect->name()));
	widthProperty->setCallbackValueGet(
		[propertyRect]() -> int { return propertyRect->value().width(); });
	widthProperty->setCallbackValueSet([propertyRect](int newWidth) {
		QRect rect = propertyRect->value();
		rect.setWidth(newWidth);
		propertyRect->setValue(rect);
	});

	return widthProperty;
}

QtnProperty *qtnCreateHeightProperty(
	QObject *parent, QtnPropertyQRectBase *propertyRect)
{
	QtnPropertyIntCallback *heightProperty = new QtnPropertyIntCallback(parent);
	heightProperty->setName(QtnPropertyQRect::tr("Height"));
	heightProperty->setDescription(
		QtnPropertyQRect::tr("Height of the %1").arg(propertyRect->name()));
	heightProperty->setCallbackValueGet(
		[propertyRect]() -> int { return propertyRect->value().height(); });
	heightProperty->setCallbackValueSet([propertyRect](int newHeight) {
		QRect rect = propertyRect->value();
		rect.setHeight(newHeight);
		propertyRect->setValue(rect);
	});

	return heightProperty;
}

QtnPropertyQRect::QtnPropertyQRect(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQRectBase>(parent)
{
}

QString QtnPropertyQRect::getToStringFormat(bool coordinates)
{
	return coordinates ? tr("[(%1, %2), (%3, %4)]") : tr("[(%1, %2) %3 x %4]");
}

QtnPropertyQRectCallback::QtnPropertyQRectCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQRectBase>(parent)
{
}
