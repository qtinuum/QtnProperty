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

#include "PropertyQPen.h"
#include "QtnProperty/GUI/PropertyQColor.h"

#include <QMap>
#include <QMetaEnum>
#include <QCoreApplication>

static const char *TRANSLATIONS[] = {
	QT_TRANSLATE_NOOP("Qt", "NoPen"),
	QT_TRANSLATE_NOOP("Qt", "SolidLine"),
	QT_TRANSLATE_NOOP("Qt", "DashLine"),
	QT_TRANSLATE_NOOP("Qt", "DotLine"),
	QT_TRANSLATE_NOOP("Qt", "DashDotLine"),
	QT_TRANSLATE_NOOP("Qt", "DashDotDotLine"),
	QT_TRANSLATE_NOOP("Qt", "CustomDashLine"),

	QT_TRANSLATE_NOOP("Qt", "FlatCap"),
	QT_TRANSLATE_NOOP("Qt", "SquareCap"),
	QT_TRANSLATE_NOOP("Qt", "RoundCap"),

	QT_TRANSLATE_NOOP("Qt", "MiterJoin"),
	QT_TRANSLATE_NOOP("Qt", "BevelJoin"),
	QT_TRANSLATE_NOOP("Qt", "RoundJoin"),
	QT_TRANSLATE_NOOP("Qt", "SvgMiterJoin"),
};

QtnPropertyQPenStyleBase::QtnPropertyQPenStyleBase(QObject *parent)
	: QtnSinglePropertyBase<Qt::PenStyle>(parent)
{
	Q_UNUSED(TRANSLATIONS);
}

bool QtnPropertyQPenStyleBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	auto metaEnum = QMetaEnum::fromType<Qt::PenStyle>();
	bool ok = false;
	int value = metaEnum.keyToValue(str.toLatin1(), &ok);

	if (ok)
	{
		setValue(Qt::PenStyle(value), reason);
		return true;
	}

	return false;
}

bool QtnPropertyQPenStyleBase::toStrImpl(QString &str) const
{
	auto metaEnum = QMetaEnum::fromType<Qt::PenStyle>();
	auto key = metaEnum.valueToKey(int(value()));
	if (key)
	{
		str = QLatin1String(key);
		return true;
	}

	return false;
}

QDataStream &operator<<(QDataStream &stream, Qt::PenStyle penStyle)
{
	stream << (qint32) penStyle;
	return stream;
}

QDataStream &operator>>(QDataStream &stream, Qt::PenStyle &penStyle)
{
	qint32 value = 0;
	stream >> value;
	penStyle = (Qt::PenStyle) value;
	return stream;
}

QtnPropertyQPenBase::QtnPropertyQPenBase(QObject *parent)
	: QtnSinglePropertyBase<QPen>(parent)
{
}

const QtnEnumInfo &QtnPropertyQPenBase::penStyleEnum()
{
	static auto enumInfo = QtnEnumInfo::withEnum<Qt::PenStyle>();
	return enumInfo;
}

const QtnEnumInfo &QtnPropertyQPenBase::penCapStyleEnum()
{
	static auto enumInfo = QtnEnumInfo::withEnum<Qt::PenCapStyle>();
	return enumInfo;
}

const QtnEnumInfo &QtnPropertyQPenBase::penJoinStyleEnum()
{
	static auto enumInfo = QtnEnumInfo::withEnum<Qt::PenJoinStyle>();
	return enumInfo;
}

bool QtnPropertyQPenBase::penFromStr(const QString &str, QPen &pen)
{
	QStringList penParts = str.split(',');
	if (penParts.size() != 5)
		return false;

	QColor color;
	if (!QtnPropertyQColor::colorFromStr(penParts[0], color))
		return false;

	auto styleEnum = penStyleEnum().fromStr(penParts[1]);
	if (!styleEnum)
		return false;
	auto style = Qt::PenStyle(styleEnum->value());

	bool ok = false;
	int width = penParts[2].trimmed().toInt(&ok);
	if (!ok)
		return false;

	auto capStyleEnum = penCapStyleEnum().fromStr(penParts[3]);
	if (!capStyleEnum)
		return false;
	auto capStyle = Qt::PenCapStyle(capStyleEnum->value());

	auto joinStyleEnum = penJoinStyleEnum().fromStr(penParts[4]);
	if (!joinStyleEnum)
		return false;
	auto joinStyle = Qt::PenJoinStyle(joinStyleEnum->value());

	pen.setColor(color);
	pen.setStyle(style);
	pen.setWidth(width);
	pen.setCapStyle(capStyle);
	pen.setJoinStyle(joinStyle);

	return true;
}

bool QtnPropertyQPenBase::strFromPen(const QPen &pen, QString &str)
{
	QString color;
	QtnPropertyQColor::strFromColor(pen.color(), color);
	QString style;
	penStyleEnum().toStr(style, pen.style());
	QString width = QString::number(pen.width());
	QString capStyle;
	penCapStyleEnum().toStr(capStyle, pen.capStyle());
	QString joinStyle;
	penJoinStyleEnum().toStr(joinStyle, pen.joinStyle());
	str = QString("%1, %2, %3, %4, %5")
			  .arg(color, style, width, capStyle, joinStyle);
	return true;
}

bool QtnPropertyQPenBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	QPen pen;
	if (!penFromStr(str, pen))
		return false;

	return setValue(pen, reason);
}

bool QtnPropertyQPenBase::toStrImpl(QString &str) const
{
	return strFromPen(value(), str);
}

bool QtnPropertyQPenBase::fromVariantImpl(
	const QVariant &var, QtnPropertyChangeReason reason)
{
	if (!var.canConvert<QPen>())
		return false;

	setValue(var.value<QPen>(), reason);
	return true;
}

bool QtnPropertyQPenBase::toVariantImpl(QVariant &var) const
{
	var = QVariant::fromValue<QPen>(value());
	return true;
}

QtnPropertyQPenStyleCallback::QtnPropertyQPenStyleCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQPenStyleBase>(parent)
{
}

QtnPropertyQPenStyle::QtnPropertyQPenStyle(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQPenStyleBase>(parent)
{
}

QtnPropertyQPenCallback::QtnPropertyQPenCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQPenBase>(parent)
{
}

QtnPropertyQPen::QtnPropertyQPen(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQPenBase>(parent)
{
}

QString QtnPropertyQPen::rootDisplayValue()
{
	return tr("(Pen)");
}

QString QtnPropertyQPen::colorKey()
{
	return QStringLiteral("color");
}

QString QtnPropertyQPen::colorDisplayName()
{
	return tr("Color");
}

QString QtnPropertyQPen::colorDescriptionFmt()
{
	return tr("Color of the %1");
}

QString QtnPropertyQPen::styleKey()
{
	return QStringLiteral("style");
}

QString QtnPropertyQPen::styleDisplayName()
{
	return tr("Style");
}

QString QtnPropertyQPen::styleDescriptionFmt()
{
	return tr("Style of the %1");
}

QString QtnPropertyQPen::capStyleKey()
{
	return QStringLiteral("capStyle");
}

QString QtnPropertyQPen::capStyleDisplayName()
{
	return tr("Cap Style");
}

QString QtnPropertyQPen::capStyleDescriptionFmt()
{
	return tr("Cap Style of the %1");
}

QString QtnPropertyQPen::joinStyleKey()
{
	return QStringLiteral("joinStyle");
}

QString QtnPropertyQPen::joinStyleDisplayName()
{
	return tr("Join Style");
}

QString QtnPropertyQPen::joinStyleDescriptionFmt()
{
	return tr("Join Style of the %1");
}
