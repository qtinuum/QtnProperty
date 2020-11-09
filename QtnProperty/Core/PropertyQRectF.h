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

#ifndef PROPERTYQRECTF_H
#define PROPERTYQRECTF_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include "PropertyDouble.h"
#include "QtnProperty/StructPropertyBase.h"
#include <QRectF>

class QTN_IMPORT_EXPORT QtnPropertyQRectFBase
	: public QtnStructPropertyBase<QRectF, QtnPropertyDoubleCallback>
{
	Q_OBJECT

private:
	QtnPropertyQRectFBase(const QtnPropertyQRectFBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectFBase(QObject *parent);

	QtnProperty *createLeftProperty(bool move);
	QtnProperty *createTopProperty(bool move);
	QtnProperty *createRightProperty(bool move);
	QtnProperty *createBottomProperty(bool move);
	QtnProperty *createWidthProperty();
	QtnProperty *createHeightProperty();

protected:
	// string conversion implementation
	bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQRectFBase)
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
	Q_INVOKABLE explicit QtnPropertyQRectFCallback(QObject *parent = nullptr);

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
	Q_INVOKABLE explicit QtnPropertyQRectF(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRectF, QtnPropertyQRectFBase)
};

#endif // PROPERTYQRECT_H
