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

#ifndef PROPERTYQSIZE_H
#define PROPERTYQSIZE_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include <QSize>

class QTN_IMPORT_EXPORT QtnPropertyQSizeBase
	: public QtnSinglePropertyBase<QSize>
{
	Q_OBJECT

private:
	QtnPropertyQSizeBase(const QtnPropertyQSizeBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQSizeBase(QObject *parent);

protected:
	// string conversion implementation
	bool fromStrImpl(const QString &str, bool edit) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQSizeBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQSizeBase, QSize)

QTN_IMPORT_EXPORT QtnProperty * qtnCreateWidthProperty(
	QObject * parent, QtnPropertyQSizeBase * propertySize);
QTN_IMPORT_EXPORT QtnProperty *qtnCreateHeightProperty(
	QObject *parent, QtnPropertyQSizeBase *propertySize);

class QTN_IMPORT_EXPORT QtnPropertyQSizeCallback
	: public QtnSinglePropertyCallback<QtnPropertyQSizeBase>
{
	Q_OBJECT

private:
	QtnPropertyQSizeCallback(
		const QtnPropertyQSizeCallback &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQSizeCallback(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQSizeCallback, QtnPropertyQSizeBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQSize
	: public QtnSinglePropertyValue<QtnPropertyQSizeBase>
{
	Q_OBJECT

private:
	QtnPropertyQSize(const QtnPropertyQSize &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQSize(QObject *parent = nullptr);

	static QString getToStringFormat();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSize, QtnPropertyQSizeBase)
};

#endif	// PROPERTYQSIZE_H
