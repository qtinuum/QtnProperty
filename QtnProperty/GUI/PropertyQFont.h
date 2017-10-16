/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef PROPERTYFONT_H
#define PROPERTYFONT_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include <QFont>

class QTN_IMPORT_EXPORT QtnPropertyQFontBase
	: public QtnSinglePropertyBase<QFont>
{
	Q_OBJECT

private:
	QtnPropertyQFontBase(const QtnPropertyQFontBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQFontBase(QObject *parent);

protected:
	// string conversion implementation
	bool fromStrImpl(const QString &str, bool edit) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQFontBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQFontBase, QFont)

class QTN_IMPORT_EXPORT QtnPropertyQFontCallback
	: public QtnSinglePropertyCallback<QtnPropertyQFontBase>
{
	Q_OBJECT

private:
	QtnPropertyQFontCallback(
		const QtnPropertyQFontCallback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQFontCallback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQFontCallback, QtnPropertyQFontBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQFont
	: public QtnSinglePropertyValue<QtnPropertyQFontBase>
{
	Q_OBJECT

private:
	QtnPropertyQFont(const QtnPropertyQFont &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQFont(QObject *parent);

	static QString getPixelStr();
	static QString getPointStr();
	static QString getPreferDefaultStr();
	static QString getNoAntialiasStr();
	static QString getPreferAntialiasStr();
	static QString getFamilyLabel();
	static QString getFamilyDescription(const QString &ownerName);
	static QString getStyleLabel();
	static QString getStyleDescription(const QString &ownerName);
	static QString getSizeLabel();
	static QString getSizeDescription(const QString &ownerName);
	static QString getSizeUnitLabel();
	static QString getSizeUnitDescription(const QString &ownerName);
	static QString getBoldLabel();
	static QString getBoldDescription(const QString &ownerName);
	static QString getItalicLabel();
	static QString getItalicDescription(const QString &ownerName);
	static QString getUnderlineLabel();
	static QString getUnderlineDescription(const QString &ownerName);
	static QString getStrikeoutLabel();
	static QString getStrikeoutDescription(const QString &ownerName);
	static QString getKerningLabel();
	static QString getKerningDescription(const QString &ownerName);
	static QString getAntialiasingLabel();
	static QString getAntialiasingDescription(const QString &ownerName);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQFont, QtnPropertyQFontBase)
};

#endif	// PROPERTYFONT_H
