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

#include "PropertyQFont.h"
#include <QFontDatabase>

QtnPropertyQFontBase::QtnPropertyQFontBase(QObject *parent)
	: QtnSinglePropertyBase<QFont>(parent)
{
	addState(QtnPropertyStateCollapsed);
}

bool QtnPropertyQFontBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	QFont font;

	if (!font.fromString(str.trimmed()))
		return false;

	return setValue(font, reason);
}

bool QtnPropertyQFontBase::toStrImpl(QString &str) const
{
	QFont v = value();
	str = v.toString();
	return true;
}

QtnPropertyQFont::QtnPropertyQFont(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQFontBase>(parent)
{
}

QString QtnPropertyQFont::getPixelStr()
{
	return tr("Pixel");
}

QString QtnPropertyQFont::getPointStr()
{
	return tr("Point");
}

QString QtnPropertyQFont::getPreferDefaultStr()
{
	return tr("PreferDefault");
}

QString QtnPropertyQFont::getNoAntialiasStr()
{
	return tr("NoAntialias");
}

QString QtnPropertyQFont::getPreferAntialiasStr()
{
	return tr("PreferAntialias");
}

QString QtnPropertyQFont::getFamilyLabel()
{
	return tr("Family");
}

QString QtnPropertyQFont::getFamilyDescription(const QString &ownerName)
{
	return tr("Family for %1").arg(ownerName);
}

QString QtnPropertyQFont::getStyleLabel()
{
	return tr("Style");
}

QString QtnPropertyQFont::getStyleDescription(const QString &ownerName)
{
	return tr("Style for %1").arg(ownerName);
}

QString QtnPropertyQFont::getSizeLabel()
{
	return tr("Size");
}

QString QtnPropertyQFont::getSizeDescription(const QString &ownerName)
{
	return tr("Size for %1").arg(ownerName);
}

QString QtnPropertyQFont::getSizeUnitLabel()
{
	return tr("Size Unit");
}

QString QtnPropertyQFont::getSizeUnitDescription(const QString &ownerName)
{
	return tr("Size Unit for %1").arg(ownerName);
}

QString QtnPropertyQFont::getBoldLabel()
{
	return tr("Bold");
}

QString QtnPropertyQFont::getBoldDescription(const QString &ownerName)
{
	return tr("Bold flag for %1").arg(ownerName);
}

QString QtnPropertyQFont::getItalicLabel()
{
	return tr("Italic");
}

QString QtnPropertyQFont::getItalicDescription(const QString &ownerName)
{
	return tr("Italic flag for %1").arg(ownerName);
}

QString QtnPropertyQFont::getUnderlineLabel()
{
	return tr("Underline");
}

QString QtnPropertyQFont::getUnderlineDescription(const QString &ownerName)
{
	return tr("Underline flag for %1").arg(ownerName);
}

QString QtnPropertyQFont::getStrikeoutLabel()
{
	return tr("Strikeout");
}

QString QtnPropertyQFont::getStrikeoutDescription(const QString &ownerName)
{
	return tr("Strikeout flag for %1").arg(ownerName);
}

QString QtnPropertyQFont::getKerningLabel()
{
	return tr("Kerning");
}

QString QtnPropertyQFont::getKerningDescription(const QString &ownerName)
{
	return tr("Kerning flag for %1").arg(ownerName);
}

QString QtnPropertyQFont::getAntialiasingLabel()
{
	return tr("Antialiasing");
}

QString QtnPropertyQFont::getAntialiasingDescription(const QString &ownerName)
{
	return tr("Antialiasing options for %1").arg(ownerName);
}

QtnPropertyQFontCallback::QtnPropertyQFontCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQFontBase>(parent)
{
}
