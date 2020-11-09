/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyQBrush.h"
#include <QMap>

QtnPropertyQBrushStyle::StrBrushStyleMap
QtnPropertyQBrushStyle::CreateStr2BrushStyle()
{
	StrBrushStyleMap str2BrushStyle = { //
		{ QByteArray(QT_TR_NOOP("NoBrush")), Qt::NoBrush },
		{ QByteArray(QT_TR_NOOP("Solid")), Qt::SolidPattern },
		{ QByteArray(QT_TR_NOOP("Dense1Pattern")), Qt::Dense1Pattern },
		{ QByteArray(QT_TR_NOOP("Dense2Pattern")), Qt::Dense2Pattern },
		{ QByteArray(QT_TR_NOOP("Dense3Pattern")), Qt::Dense3Pattern },
		{ QByteArray(QT_TR_NOOP("Dense4Pattern")), Qt::Dense4Pattern },
		{ QByteArray(QT_TR_NOOP("Dense5Pattern")), Qt::Dense5Pattern },
		{ QByteArray(QT_TR_NOOP("Dense6Pattern")), Qt::Dense6Pattern },
		{ QByteArray(QT_TR_NOOP("Dense7Pattern")), Qt::Dense7Pattern },
		{ QByteArray(QT_TR_NOOP("HorPattern")), Qt::HorPattern },
		{ QByteArray(QT_TR_NOOP("VerPattern")), Qt::VerPattern },
		{ QByteArray(QT_TR_NOOP("CrossPattern")), Qt::CrossPattern },
		{ QByteArray(QT_TR_NOOP("BDiagPattern")), Qt::BDiagPattern },
		{ QByteArray(QT_TR_NOOP("FDiagPattern")), Qt::FDiagPattern },
		{ QByteArray(QT_TR_NOOP("DiagCrossPattern")), Qt::DiagCrossPattern },
		{ QByteArray(QT_TR_NOOP("LinearGradientPattern")),
			Qt::LinearGradientPattern },
		{ QByteArray(QT_TR_NOOP("RadialGradientPattern")),
			Qt::RadialGradientPattern },
		{ QByteArray(QT_TR_NOOP("ConicalGradientPattern")),
			Qt::ConicalGradientPattern },
		{ QByteArray(QT_TR_NOOP("TexturePattern")), Qt::TexturePattern }
	};
	return str2BrushStyle;
}

QtnPropertyQBrushStyle::BrushStyleToStrMap
QtnPropertyQBrushStyle::CreateBrushStyle2Str()
{
	BrushStyleToStrMap brushStyle2Str;
	auto map = CreateStr2BrushStyle();
	for (auto it = map.constBegin(); it != map.constEnd(); ++it)
		brushStyle2Str[it.value()] = it.key();

	return brushStyle2Str;
}

bool QtnPropertyQBrushStyleBase::brushStyleToStr(
	Qt::BrushStyle brushStyle, QByteArray &result)
{
	static auto brushStyle2Str = QtnPropertyQBrushStyle::CreateBrushStyle2Str();
	auto it = brushStyle2Str.find(brushStyle);
	if (it != brushStyle2Str.end())
	{
		result = it.value();
		return true;
	}

	return false;
}

QtnPropertyQBrushStyle::QtnPropertyQBrushStyle(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQBrushStyleBase>(parent)
{
}

bool QtnPropertyQBrushStyle::translateBrushStyle(
	Qt::BrushStyle brushStyle, QString &result)
{
	QByteArray id;
	if (!QtnPropertyQBrushStyleBase::brushStyleToStr(brushStyle, id))
		return false;

	result = tr(id.data());
	return true;
}

QtnPropertyQBrushStyleBase::QtnPropertyQBrushStyleBase(QObject *parent)
	: QtnSinglePropertyBase<Qt::BrushStyle>(parent)
{
}

bool QtnPropertyQBrushStyleBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	static auto str2BrushStyle = QtnPropertyQBrushStyle::CreateStr2BrushStyle();
	auto it = str2BrushStyle.find(str.toLatin1());
	if (it != str2BrushStyle.end())
	{
		return setValue(it.value(), reason);
	}

	return false;
}

bool QtnPropertyQBrushStyleBase::toStrImpl(QString &str) const
{
	QByteArray id;
	if (!brushStyleToStr(value(), id))
		return false;

	str = QString::fromLatin1(id.data(), id.size());
	return true;
}

QDataStream &operator<<(QDataStream &stream, Qt::BrushStyle brushStyle)
{
	stream << (qint32) brushStyle;
	return stream;
}

QDataStream &operator>>(QDataStream &stream, Qt::BrushStyle &brushStyle)
{
	qint32 value = 0;
	stream >> value;
	brushStyle = (Qt::BrushStyle) value;
	return stream;
}

QtnPropertyQBrushStyleCallback::QtnPropertyQBrushStyleCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQBrushStyleBase>(parent)
{
}
