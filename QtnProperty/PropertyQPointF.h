/*
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

#pragma once

#include "Auxiliary/PropertyTemplates.h"
#include "Delegates/PropertyDelegate.h"
#include "Core/PropertyDouble.h"
#include "StructPropertyBase.h"

#include <QPointF>
#include <QRegExp>

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

protected:
	virtual QString getXLabel();
	virtual QString getXDescriptionFormat();
	virtual QString getYLabel();
	virtual QString getYDescriptionFormat();

	// string conversion implementation
	virtual bool fromStrImpl(const QString &str, bool edit) override;
	virtual bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPointFBase)

private:
	QRegExp point_parser;
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
	explicit QtnPropertyQPointFCallback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQPointFCallback, QtnPropertyQPointFBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQPointF
	: public QtnSinglePropertyValue<QtnPropertyQPointFBase>
{
	Q_OBJECT

private:
	QtnPropertyQPointF(
		const QtnPropertyQPointF &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPointF(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQPointF, QtnPropertyQPointFBase)

	static void Register();
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQPointF
	: public QtnPropertyDelegateTypedEx<QtnPropertyQPointFBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQPointF)

public:
	QtnPropertyDelegateQPointF(QtnPropertyQPointFBase &owner);

protected:
	virtual QWidget *createValueEditorImpl(
		QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString &strValue) const override;
};
