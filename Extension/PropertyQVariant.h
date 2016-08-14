#pragma once

#include "QtnProperty/Core/Auxiliary/PropertyTemplates.h"
#include "QtnProperty/Widget/Delegates/PropertyDelegate.h"

#include <QVariant>

class QtnPropertyQVariantBase: public QtnSinglePropertyBase<const QVariant &>
{
	Q_OBJECT
	QtnPropertyQVariantBase(const QtnPropertyQVariantBase& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVariantBase(QObject *parent);

protected:
	virtual bool fromStrImpl(const QString &str, bool edit) override;
	virtual bool toStrImpl(QString &str) const override;
	virtual bool fromVariantImpl(const QVariant &var, bool edit) override;
	virtual bool toVariantImpl(QVariant &var) const override;
};


class QtnPropertyQVariantCallback: public QtnSinglePropertyCallback<QtnPropertyQVariantBase>
{
	Q_OBJECT
	QtnPropertyQVariantCallback(const QtnPropertyQVariantCallback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVariantCallback(QObject *object, const QMetaProperty &meta_property);

private:
	QVariant value;
};

class QtnPropertyQVariant: public QtnSinglePropertyValue<QtnPropertyQVariantBase>
{
	Q_OBJECT
	QtnPropertyQVariant(const QtnPropertyQVariant& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVariant(QObject *parent);

	static void Register();
	static QString valueToString(const QVariant &value);
	static bool variantIsObject(QVariant::Type type);
	static QString getPlaceholderStr(QVariant::Type type);
};

class QtnPropertyDelegateQVariant: public QtnPropertyDelegateTyped<QtnPropertyQVariantBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQVariant)

public:
	typedef QtnPropertyDelegateTyped<QtnPropertyQVariantBase> Inherited;

	QtnPropertyDelegateQVariant(QtnPropertyQVariantBase& owner);

protected:
	virtual bool acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const override;
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString &strValue) const override;
	virtual void drawValueImpl(QStylePainter &painter, const QRect &rect,
							   const QStyle::State &state, bool *needTooltip = nullptr) const override;
};

