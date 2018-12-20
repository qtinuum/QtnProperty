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

	bool coordinates;

private:
	QtnPropertyQRectBase(const QtnPropertyQRectBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectBase(QObject *parent);

	inline bool coordinateMode() const;
	void setMode(bool coordinates);

	QtnProperty *createLeftProperty(bool move);
	QtnProperty *createTopProperty(bool move);
	QtnProperty *createRightProperty(bool move);
	QtnProperty *createBottomProperty(bool move);
	QtnProperty *createWidthProperty();
	QtnProperty *createHeightProperty();

	static QByteArray qtnQRect_LTRB();
	static QByteArray qtnQRect_LTWH();

protected:
	// string conversion implementation
	bool fromStrImpl(const QString &str, bool edit) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQRectBase)
};

bool QtnPropertyQRectBase::coordinateMode() const
{
	return coordinates;
}

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

	static QString getToStringFormat(bool coordinates);
	static QString leftString();
	static QString leftDescription();
	static QString topString();
	static QString topDescription();
	static QString rightString();
	static QString rightDescription();
	static QString bottomString();
	static QString bottomDescription();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRect, QtnPropertyQRectBase)
};

#endif // PROPERTYQRECT_H
