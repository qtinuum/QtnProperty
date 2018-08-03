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

#ifndef PROPERTYQPOINT_H
#define PROPERTYQPOINT_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include "Core/PropertyInt.h"
#include "StructPropertyBase.h"
#include <QPoint>

class QTN_IMPORT_EXPORT QtnPropertyQPointBase
	: public QtnStructPropertyBase<QPoint, QtnPropertyIntCallback>
{
	Q_OBJECT

private:
	QtnPropertyQPointBase(const QtnPropertyQPointBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPointBase(QObject *parent);

	QtnProperty *createXProperty();
	QtnProperty *createYProperty();

protected:
	// string conversion implementation
	bool fromStrImpl(const QString &str, bool edit) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPointBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPointBase, QPoint)

class QTN_IMPORT_EXPORT QtnPropertyQPointCallback
	: public QtnSinglePropertyCallback<QtnPropertyQPointBase>
{
	Q_OBJECT

private:
	QtnPropertyQPointCallback(
		const QtnPropertyQPointCallback &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQPointCallback(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQPointCallback, QtnPropertyQPointBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQPoint
	: public QtnSinglePropertyValue<QtnPropertyQPointBase>
{
	Q_OBJECT

private:
	QtnPropertyQPoint(const QtnPropertyQPoint &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQPoint(QObject *parent = nullptr);

	static QString getToStringFormat();

	static QString xString();
	static QString xDescription();
	static QString yString();
	static QString yDescription();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPoint, QtnPropertyQPointBase)
};

#endif // PROPERTYQPOINT_H
