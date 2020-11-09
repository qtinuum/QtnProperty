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

#ifndef PROPERTY_BRUSH_H
#define PROPERTY_BRUSH_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include "QtnProperty/Enum.h"

#include <QBrush>

Q_DECLARE_METATYPE(Qt::BrushStyle)

class QTN_IMPORT_EXPORT QtnPropertyQBrushStyleBase
	: public QtnSinglePropertyBase<Qt::BrushStyle>
{
	Q_OBJECT
	QtnPropertyQBrushStyleBase(
		const QtnPropertyQBrushStyleBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQBrushStyleBase(QObject *parent);

	static bool brushStyleToStr(Qt::BrushStyle brushStyle, QByteArray &result);

protected:
	// string conversion implementation
	bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQBrushStyleBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQBrushStyleBase, Qt::BrushStyle)
QTN_IMPORT_EXPORT QDataStream &operator<<(
	QDataStream &stream, Qt::BrushStyle brushStyle);
QTN_IMPORT_EXPORT QDataStream &operator>>(
	QDataStream &stream, Qt::BrushStyle &brushStyle);

class QTN_IMPORT_EXPORT QtnPropertyQBrushStyleCallback
	: public QtnSinglePropertyCallback<QtnPropertyQBrushStyleBase>
{
	Q_OBJECT
	QtnPropertyQBrushStyleCallback(
		const QtnPropertyQBrushStyleCallback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQBrushStyleCallback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQBrushStyleCallback, QtnPropertyQBrushStyleBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQBrushStyle
	: public QtnSinglePropertyValue<QtnPropertyQBrushStyleBase>
{
	Q_OBJECT
	QtnPropertyQBrushStyle(
		const QtnPropertyQBrushStyle &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQBrushStyle(QObject *parent);

	static bool translateBrushStyle(Qt::BrushStyle brushStyle, QString &result);

	using StrBrushStyleMap = QMap<QByteArray, Qt::BrushStyle>;
	using BrushStyleToStrMap = QMap<Qt::BrushStyle, QByteArray>;

	static StrBrushStyleMap CreateStr2BrushStyle();
	static BrushStyleToStrMap CreateBrushStyle2Str();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQBrushStyle, QtnPropertyQBrushStyleBase)
};

#endif // PROPERTY_BRUSH_H
