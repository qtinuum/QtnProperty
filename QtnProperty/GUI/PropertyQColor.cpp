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

#include "PropertyQColor.h"

#include "Auxiliary/PropertyDelegateInfo.h"

QtnPropertyQColorBase::QtnPropertyQColorBase(QObject *parent)
	: QtnStructPropertyBase<QColor, QtnPropertyIntCallback>(parent)
{
}

QtnProperty *QtnPropertyQColorBase::createRedProperty()
{
	auto result = createFieldProperty(QtnPropertyQColor::redKey(),
		QtnPropertyQColor::redDisplayName(),
		QtnPropertyQColor::redDescriptionFmt(), &QColor::red, &QColor::setRed);
	result->setMinValue(0);
	result->setMaxValue(255);

	QtnPropertyDelegateInfo delegate;
	delegate.name = qtnSliderBoxDelegate();
	delegate.attributes[qtnLiveUpdateAttr()] = true;
	delegate.attributes[qtnAnimateAttr()] = true;
	delegate.attributes[qtnFillColorAttr()] = QColor(255, 100, 100);
	result->setDelegateInfo(delegate);
	return result;
}

QtnProperty *QtnPropertyQColorBase::createGreenProperty()
{
	auto result = createFieldProperty(QtnPropertyQColor::greenKey(),
		QtnPropertyQColor::greenDisplayName(),
		QtnPropertyQColor::greenDescriptionFmt(), &QColor::green,
		&QColor::setGreen);
	result->setMinValue(0);
	result->setMaxValue(255);

	QtnPropertyDelegateInfo delegate;
	delegate.name = qtnSliderBoxDelegate();
	delegate.attributes[qtnLiveUpdateAttr()] = true;
	delegate.attributes[qtnAnimateAttr()] = true;
	delegate.attributes[qtnFillColorAttr()] = QColor(100, 255, 100);
	result->setDelegateInfo(delegate);
	return result;
}

QtnProperty *QtnPropertyQColorBase::createBlueProperty()
{
	auto result = createFieldProperty(QtnPropertyQColor::blueKey(),
		QtnPropertyQColor::blueDisplayName(),
		QtnPropertyQColor::blueDescriptionFmt(), &QColor::blue,
		&QColor::setBlue);
	result->setMinValue(0);
	result->setMaxValue(255);

	QtnPropertyDelegateInfo delegate;
	delegate.name = qtnSliderBoxDelegate();
	delegate.attributes[qtnLiveUpdateAttr()] = true;
	delegate.attributes[qtnAnimateAttr()] = true;
	delegate.attributes[qtnFillColorAttr()] = QColor(100, 100, 255);
	result->setDelegateInfo(delegate);
	return result;
}

bool QtnPropertyQColorBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	QColor color;
	return QtnPropertyQColor::colorFromStr(str, color) &&
		setValue(color, reason);
}

bool QtnPropertyQColorBase::toStrImpl(QString &str) const
{
	return QtnPropertyQColor::strFromColor(value(), str);
}

bool QtnPropertyQColor::colorFromStr(const QString &str, QColor &color)
{
	QColor newColor(str.trimmed());
	if (!newColor.isValid())
		return false;

	color = newColor;
	return true;
}

bool QtnPropertyQColor::strFromColor(const QColor &color, QString &str)
{
	str = color.name((color.alpha() < 255) ? QColor::HexArgb : QColor::HexRgb);
	return true;
}

QtnPropertyQColorCallback::QtnPropertyQColorCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQColorBase>(parent)
{
}

QtnPropertyQColor::QtnPropertyQColor(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQColorBase>(parent)
{
}

QString QtnPropertyQColor::redKey()
{
	return QStringLiteral("red");
}

QString QtnPropertyQColor::redDisplayName()
{
	return tr("Red");
}

QString QtnPropertyQColor::redDescriptionFmt()
{
	return tr("Red component of %1");
}

QString QtnPropertyQColor::greenKey()
{
	return QStringLiteral("green");
}

QString QtnPropertyQColor::greenDisplayName()
{
	return tr("Green");
}

QString QtnPropertyQColor::greenDescriptionFmt()
{
	return tr("Green component of %1");
}

QString QtnPropertyQColor::blueKey()
{
	return QStringLiteral("blue");
}

QString QtnPropertyQColor::blueDisplayName()
{
	return tr("Blue");
}

QString QtnPropertyQColor::blueDescriptionFmt()
{
	return tr("Blue component of %1");
}
