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

#ifndef PROPERTYQRECT_H
#define PROPERTYQRECT_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include "PropertyInt.h"
#include "QtnProperty/StructPropertyBase.h"
#include <QRect>

class QTN_IMPORT_EXPORT QtnPropertyQRectBase
	: public QtnStructPropertyBase<QRect, QtnPropertyIntCallback>
{
	Q_OBJECT

private:
	QtnPropertyQRectBase(const QtnPropertyQRectBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectBase(QObject *parent);

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

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQRectBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQRectBase, QRect)

class QTN_IMPORT_EXPORT QtnPropertyQRectCallback
	: public QtnSinglePropertyCallback<QtnPropertyQRectBase>
{
	Q_OBJECT

private:
	QtnPropertyQRectCallback(
		const QtnPropertyQRectCallback &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQRectCallback(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQRectCallback, QtnPropertyQRectBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQRect
	: public QtnSinglePropertyValue<QtnPropertyQRectBase>
{
	Q_OBJECT

private:
	QtnPropertyQRect(const QtnPropertyQRect &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQRect(QObject *parent = nullptr);

	static QByteArray delegateName(bool coordinateMode);

	static QString getToStringFormat(bool m_coordinates);
	static QString leftKey();
	static QString leftString();
	static QString leftDescriptionFmt();
	static QString topKey();
	static QString topString();
	static QString topDescriptionFmt();
	static QString rightKey();
	static QString rightString();
	static QString rightDescriptionFmt();
	static QString bottomKey();
	static QString bottomString();
	static QString bottomDescriptionFmt();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRect, QtnPropertyQRectBase)
};

#endif // PROPERTYQRECT_H
