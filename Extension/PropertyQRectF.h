#pragma once

#include "QtnProperty/Core/Auxiliary/PropertyTemplates.h"
#include "QtnProperty/Widget/Delegates/PropertyDelegate.h"
#include "QtnProperty/Core/Core/PropertyDouble.h"

#include "Extension.h"

#include <QRectF>
#include <QRegExp>

class QtnPropertyQRectFBase: public QtnStructPropertyBase<QRectF, QtnPropertyDoubleCallback>
{
	Q_OBJECT
	QtnPropertyQRectFBase(const QtnPropertyQRectFBase& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectFBase(QObject *parent);

	QtnProperty *createLeftProperty();
	QtnProperty *createTopProperty();
	QtnProperty *createWidthProperty();
	QtnProperty *createHeightProperty();

protected:
	// string conversion implementation
	virtual bool fromStrImpl(const QString& str) override;
	virtual bool toStrImpl(QString& str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQRectFBase)

private:
	QRegExp rect_parser;
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQRectFBase, QRectF)

class QtnPropertyQRectFCallback: public QtnSinglePropertyCallback<QtnPropertyQRectFBase>
{
	Q_OBJECT
	QtnPropertyQRectFCallback(const QtnPropertyQRectFCallback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectFCallback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRectFCallback, QtnPropertyQRectFBase)
};

class QtnPropertyQRectF: public QtnSinglePropertyValue<QtnPropertyQRectFBase>
{
	Q_OBJECT
	QtnPropertyQRectF(const QtnPropertyQRectF& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQRectF(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRectF, QtnPropertyQRectFBase)

	static void Register();
};

class QtnPropertyDelegateQRectF: public QtnPropertyDelegateTypedEx<QtnPropertyQRectFBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQRectF)

public:
	QtnPropertyDelegateQRectF(QtnPropertyQRectFBase& owner);

protected:
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString& strValue) const override;
};
