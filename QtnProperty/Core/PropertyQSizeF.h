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

#ifndef PROPERTYQSIZEF_H
#define PROPERTYQSIZEF_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include "PropertyDouble.h"
#include "QtnProperty/StructPropertyBase.h"
#include <QSizeF>

class QTN_IMPORT_EXPORT QtnPropertyQSizeFBase
	: public QtnStructPropertyBase<QSizeF, QtnPropertyDoubleCallback>
{
	Q_OBJECT

private:
	QtnPropertyQSizeFBase(const QtnPropertyQSizeFBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQSizeFBase(QObject *parent);

	QtnProperty *createWidthProperty();
	QtnProperty *createHeightProperty();

protected:
	// string conversion implementation
	bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQSizeFBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQSizeFBase, QSizeF)

class QTN_IMPORT_EXPORT QtnPropertyQSizeFCallback
	: public QtnSinglePropertyCallback<QtnPropertyQSizeFBase>
{
	Q_OBJECT

private:
	QtnPropertyQSizeFCallback(
		const QtnPropertyQSizeFCallback &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQSizeFCallback(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQSizeFCallback, QtnPropertyQSizeFBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQSizeF
	: public QtnSinglePropertyValue<QtnPropertyQSizeFBase>
{
	Q_OBJECT

private:
	QtnPropertyQSizeF(const QtnPropertyQSizeF &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQSizeF(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSizeF, QtnPropertyQSizeFBase)
};

#endif // PROPERTYQSIZEF_H
