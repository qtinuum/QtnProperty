#pragma once

//#include "QtnProperty/Core/Core/PropertyQString.h"
//#include "QtnProperty/Widget/Delegates/Core/PropertyDelegateQString.h"

#include "QtnProperty/Core/Auxiliary/PropertyTemplates.h"
#include "QtnProperty/Widget/Delegates/PropertyDelegate.h"

#include <QVariant>

class QtnPropertyQVariantBase: public QtnSinglePropertyBase<QVariant>
{
	Q_OBJECT
	QtnPropertyQVariantBase(const QtnPropertyQVariantBase& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVariantBase(QObject *parent);

protected:
	// string conversion implementation
	bool fromStrImpl(const QString& str) override;
	bool toStrImpl(QString& str) const override;
};


class QtnPropertyQVariantCallback: public QtnSinglePropertyCallback<QtnPropertyQVariantBase>
{
	Q_OBJECT
	QtnPropertyQVariantCallback(const QtnPropertyQVariantCallback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVariantCallback(QObject *parent);
};

class QtnPropertyQVariant: public QtnSinglePropertyValue<QtnPropertyQVariantBase>
{
	Q_OBJECT
	QtnPropertyQVariant(const QtnPropertyQVariant& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVariant(QObject *parent);
};

class QtnPropertyDelegateQVariant: public QtnPropertyDelegateTyped<QtnPropertyQVariantBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQVariant)

public:
	typedef QtnPropertyDelegateTyped<QtnPropertyQVariantBase> Inherited;

	QtnPropertyDelegateQVariant(QtnPropertyQVariantBase& owner);

	static bool variantIsObject(const QVariant &variant);

protected:
	virtual bool acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const override;
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString &strValue) const override;
	virtual void drawValueImpl(QStylePainter &painter, const QRect &rect,
							   const QStyle::State &state, bool *needTooltip = nullptr) const override;

	bool check_is_object;
};

