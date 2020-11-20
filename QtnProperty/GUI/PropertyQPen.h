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

#ifndef PROPERTY_PEN_H
#define PROPERTY_PEN_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include "QtnProperty/Enum.h"
#include "QtnProperty/StructPropertyBase.h"

#include <QPen>

Q_DECLARE_METATYPE(Qt::PenStyle)

class QTN_IMPORT_EXPORT QtnPropertyQPenStyleBase
	: public QtnSinglePropertyBase<Qt::PenStyle>
{
	Q_OBJECT
	QtnPropertyQPenStyleBase(
		const QtnPropertyQPenStyleBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPenStyleBase(QObject *parent);

protected:
	// string conversion implementation
	bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPenStyleBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPenStyleBase, Qt::PenStyle)
QTN_IMPORT_EXPORT QDataStream &operator<<(
	QDataStream &stream, Qt::PenStyle penStyle);
QTN_IMPORT_EXPORT QDataStream &operator>>(
	QDataStream &stream, Qt::PenStyle &penStyle);

class QTN_IMPORT_EXPORT QtnPropertyQPenStyleCallback
	: public QtnSinglePropertyCallback<QtnPropertyQPenStyleBase>
{
	Q_OBJECT
	QtnPropertyQPenStyleCallback(
		const QtnPropertyQPenStyleCallback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPenStyleCallback(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQPenStyleCallback, QtnPropertyQPenStyleBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQPenStyle
	: public QtnSinglePropertyValue<QtnPropertyQPenStyleBase>
{
	Q_OBJECT
	QtnPropertyQPenStyle(const QtnPropertyQPenStyle &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPenStyle(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQPenStyle, QtnPropertyQPenStyleBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQPenBase : public QtnSinglePropertyBase<QPen>
{
	Q_OBJECT
	QtnPropertyQPenBase(const QtnPropertyQPenBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPenBase(QObject *parent);

	QtnProperty *createPenStyleProperty();
	QtnProperty *createCapStyleProperty();
	QtnProperty *createJoinStyleProperty();

	static const QtnEnumInfo &penStyleEnum();
	static const QtnEnumInfo &penCapStyleEnum();
	static const QtnEnumInfo &penJoinStyleEnum();
	static bool penFromStr(const QString &str, QPen &pen);
	static bool strFromPen(const QPen &pen, QString &str);

protected:
	// string conversion implementation
	bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	bool toStrImpl(QString &str) const override;

	// variant conversion implementation
	bool fromVariantImpl(
		const QVariant &var, QtnPropertyChangeReason reason) override;
	bool toVariantImpl(QVariant &var) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPenBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPenBase, QPen)

class QTN_IMPORT_EXPORT QtnPropertyQPenCallback
	: public QtnSinglePropertyCallback<QtnPropertyQPenBase>
{
	Q_OBJECT
	QtnPropertyQPenCallback(
		const QtnPropertyQPenCallback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPenCallback(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQPenCallback, QtnPropertyQPenBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQPen
	: public QtnSinglePropertyValue<QtnPropertyQPenBase>
{
	Q_OBJECT
	QtnPropertyQPen(const QtnPropertyQPen &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPen(QObject *parent = nullptr);

	static QString rootDisplayValue();

	static QString colorKey();
	static QString colorDisplayName();
	static QString colorDescriptionFmt();

	static QString styleKey();
	static QString styleDisplayName();
	static QString styleDescriptionFmt();

	static QString capStyleKey();
	static QString capStyleDisplayName();
	static QString capStyleDescriptionFmt();

	static QString joinStyleKey();
	static QString joinStyleDisplayName();
	static QString joinStyleDescriptionFmt();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPen, QtnPropertyQPenBase)
};

#endif // PROPERTY_PEN_H
