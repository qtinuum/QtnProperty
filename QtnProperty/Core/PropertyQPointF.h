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

#ifndef PROPERTYQPOINTF_H
#define PROPERTYQPOINTF_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include "PropertyDouble.h"
#include "QtnProperty/StructPropertyBase.h"
#include <QPointF>

class QTN_IMPORT_EXPORT QtnPropertyQPointFBase
	: public QtnStructPropertyBase<QPointF, QtnPropertyDoubleCallback>
{
	Q_OBJECT

private:
	QtnPropertyQPointFBase(
		const QtnPropertyQPointFBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPointFBase(QObject *parent);

	QtnProperty *createXProperty();
	QtnProperty *createYProperty();

	virtual QString getXLabel() const;
	virtual QString getXDescriptionFormat() const;
	virtual QString getYLabel() const;
	virtual QString getYDescriptionFormat() const;

protected:
	// string conversion implementation
	bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPointFBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPointFBase, QPointF)

class QTN_IMPORT_EXPORT QtnPropertyQPointFCallback
	: public QtnSinglePropertyCallback<QtnPropertyQPointFBase>
{
	Q_OBJECT

private:
	QtnPropertyQPointFCallback(
		const QtnPropertyQPointFCallback &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQPointFCallback(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQPointFCallback, QtnPropertyQPointFBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQPointF
	: public QtnSinglePropertyValue<QtnPropertyQPointFBase>
{
	Q_OBJECT

private:
	QtnPropertyQPointF(const QtnPropertyQPointF &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQPointF(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQPointF, QtnPropertyQPointFBase)
};

#endif // PROPERTYQPOINTF_H
