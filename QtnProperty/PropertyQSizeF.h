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

#include <QSizeF>
#include <QRegExp>

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
	virtual bool fromStrImpl(const QString &str, bool edit) override;
	virtual bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQSizeFBase)

private:
	QRegExp sizeParser;
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
	explicit QtnPropertyQSizeFCallback(QObject *parent);

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
	explicit QtnPropertyQSizeF(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSizeF, QtnPropertyQSizeFBase)

	static void Register();
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQSizeF
	: public QtnPropertyDelegateTypedEx<QtnPropertyQSizeFBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQSizeF)

public:
	QtnPropertyDelegateQSizeF(QtnPropertyQSizeFBase &owner);

protected:
	virtual QWidget *createValueEditorImpl(
		QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString &strValue) const override;
};
