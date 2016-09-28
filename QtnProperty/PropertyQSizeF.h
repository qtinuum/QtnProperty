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
	QtnPropertyQSizeFBase(const QtnPropertyQSizeFBase& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQSizeFBase(QObject *parent);

	QtnProperty *createWidthProperty();
	QtnProperty *createHeightProperty();

protected:
	// string conversion implementation
	virtual bool fromStrImpl(const QString& str, bool edit) override;
	virtual bool toStrImpl(QString& str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQSizeFBase)

private:
	QRegExp size_parser;
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQSizeFBase, QSizeF)

class QTN_IMPORT_EXPORT QtnPropertyQSizeFCallback
	: public QtnSinglePropertyCallback<QtnPropertyQSizeFBase>
{
	Q_OBJECT
	QtnPropertyQSizeFCallback(const QtnPropertyQSizeFCallback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQSizeFCallback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSizeFCallback, QtnPropertyQSizeFBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQSizeF
	: public QtnSinglePropertyValue<QtnPropertyQSizeFBase>
{
	Q_OBJECT
	QtnPropertyQSizeF(const QtnPropertyQSizeF& other) Q_DECL_EQ_DELETE;

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
	QtnPropertyDelegateQSizeF(QtnPropertyQSizeFBase& owner);

protected:
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString& strValue) const override;
};
