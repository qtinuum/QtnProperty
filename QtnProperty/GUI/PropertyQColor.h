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

#ifndef PROPERTY_COLOR_H
#define PROPERTY_COLOR_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include "StructPropertyBase.h"
#include "Core/PropertyInt.h"
#include <QColor>

enum QtnColorDelegateShape
{
	QtnColorDelegateShapeNone = 0x1,
	QtnColorDelegateShapeSquare = 0x2,
	QtnColorDelegateShapeCircle = 0x3
};

class QTN_IMPORT_EXPORT QtnPropertyQColorBase
	: public QtnStructPropertyBase<QColor, QtnPropertyIntCallback>
{
	Q_OBJECT

private:
	QtnPropertyQColorBase(const QtnPropertyQColorBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQColorBase(QObject *parent);

	QtnProperty *createRedProperty();
	QtnProperty *createGreenProperty();
	QtnProperty *createBlueProperty();

protected:
	// string conversion implementation
	bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQColorBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQColorBase, QColor)

class QTN_IMPORT_EXPORT QtnPropertyQColorCallback
	: public QtnSinglePropertyCallback<QtnPropertyQColorBase>
{
	Q_OBJECT

private:
	QtnPropertyQColorCallback(
		const QtnPropertyQColorCallback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQColorCallback(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQColorCallback, QtnPropertyQColorBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQColor
	: public QtnSinglePropertyValue<QtnPropertyQColorBase>
{
	Q_OBJECT

private:
	QtnPropertyQColor(const QtnPropertyQColor &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQColor(QObject *parent = nullptr);

	static QString redKey();
	static QString redDisplayName();
	static QString redDescriptionFmt();
	static QString greenKey();
	static QString greenDisplayName();
	static QString greenDescriptionFmt();
	static QString blueKey();
	static QString blueDisplayName();
	static QString blueDescriptionFmt();

	static bool colorFromStr(const QString &str, QColor &color);
	static bool strFromColor(const QColor &color, QString &str);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQColor, QtnPropertyQColorBase)
};

#endif // PROPERTY_COLOR_H
