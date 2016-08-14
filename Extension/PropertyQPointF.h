#pragma once

#include "QtnProperty/Core/Auxiliary/PropertyTemplates.h"
#include "QtnProperty/Widget/Delegates/PropertyDelegate.h"
#include "QtnProperty/Core/Core/PropertyDouble.h"

#include "Extension.h"

#include <QPointF>
#include <QRegExp>

class QtnPropertyQPointFBase: public QtnStructPropertyBase<QPointF, QtnPropertyDoubleCallback>
{
	Q_OBJECT
	QtnPropertyQPointFBase(const QtnPropertyQPointFBase& other) Q_DECL_EQ_DELETE;

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
	virtual bool fromStrImpl(const QString& str, bool edit) override;
	virtual bool toStrImpl(QString& str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPointFBase)

private:
	QRegExp point_parser;
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPointFBase, QPointF)

class QtnPropertyQPointFCallback: public QtnSinglePropertyCallback<QtnPropertyQPointFBase>
{
	Q_OBJECT
	QtnPropertyQPointFCallback(const QtnPropertyQPointFCallback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPointFCallback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPointFCallback, QtnPropertyQPointFBase)
};

class QtnPropertyQPointF: public QtnSinglePropertyValue<QtnPropertyQPointFBase>
{
	Q_OBJECT
	QtnPropertyQPointF(const QtnPropertyQPointF& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQPointF(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPointF, QtnPropertyQPointFBase)

	static void Register();
};

class QtnPropertyDelegateQPointF: public QtnPropertyDelegateTypedEx<QtnPropertyQPointFBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQPointF)

public:
	QtnPropertyDelegateQPointF(QtnPropertyQPointFBase& owner);

protected:
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString& strValue) const override;
};
