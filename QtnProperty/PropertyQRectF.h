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
	QtnPropertyQRectFBase(const QtnPropertyQRectFBase& other) Q_DECL_EQ_DELETE;

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
	virtual bool fromStrImpl(const QString& str, bool edit) override;
	virtual bool toStrImpl(QString& str) const override;

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
	QtnPropertyQRectFCallback(const QtnPropertyQRectFCallback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectFCallback(QObject *parent, bool coordinates);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRectFCallback, QtnPropertyQRectFBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQRectF
	: public QtnSinglePropertyValue<QtnPropertyQRectFBase>
{
	Q_OBJECT
	QtnPropertyQRectF(const QtnPropertyQRectF& other) Q_DECL_EQ_DELETE;

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
	QtnPropertyDelegateQRectF(QtnPropertyQRectFBase& owner, bool coordinates);

	static QtnPropertyDelegate *createLTWH(QtnProperty &owner);
	static QtnPropertyDelegate *createLTRB(QtnProperty &owner);

protected:
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString& strValue) const override;

private:
	bool coordinates;
};
