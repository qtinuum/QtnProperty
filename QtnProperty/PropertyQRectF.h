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

#include <QRectF>
#include <QRegExp>

class QTN_IMPORT_EXPORT QtnPropertyQRectFBase
	: public QtnStructPropertyBase<QRectF, QtnPropertyDoubleCallback>
{
	Q_OBJECT

private:
	QtnPropertyQRectFBase(const QtnPropertyQRectFBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectFBase(QObject *parent);

	void setMode(bool coordinates);

	QtnProperty *createLeftProperty(bool move);
	QtnProperty *createTopProperty(bool move);
	QtnProperty *createRightProperty(bool move);
	QtnProperty *createBottomProperty(bool move);
	QtnProperty *createWidthProperty();
	QtnProperty *createHeightProperty();

protected:
	// string conversion implementation
	virtual bool fromStrImpl(const QString &str, bool edit) override;
	virtual bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQRectFBase)

private:
	QRegExp rect_parser;
	bool coordinates;
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQRectFBase, QRectF)

class QTN_IMPORT_EXPORT QtnPropertyQRectFCallback
	: public QtnSinglePropertyCallback<QtnPropertyQRectFBase>
{
	Q_OBJECT

private:
	QtnPropertyQRectFCallback(
		const QtnPropertyQRectFCallback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectFCallback(QObject *parent, bool coordinates);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQRectFCallback, QtnPropertyQRectFBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQRectF
	: public QtnSinglePropertyValue<QtnPropertyQRectFBase>
{
	Q_OBJECT

private:
	QtnPropertyQRectF(const QtnPropertyQRectF &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectF(QObject *parent, bool coordinates);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRectF, QtnPropertyQRectFBase)

	static void Register();
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQRectF
	: public QtnPropertyDelegateTypedEx<QtnPropertyQRectFBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQRectF)

public:
	QtnPropertyDelegateQRectF(QtnPropertyQRectFBase &owner, bool coordinates);

	static QtnPropertyDelegate *createLTWH(QtnProperty &owner);
	static QtnPropertyDelegate *createLTRB(QtnProperty &owner);

protected:
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString &strValue) const override;

private:
	bool coordinates;
};
