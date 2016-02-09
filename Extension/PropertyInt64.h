#pragma once

#include "QtnProperty/Core/Auxiliary/PropertyTemplates.h"
#include "QtnProperty/Widget/Delegates/PropertyDelegate.h"

class QtnPropertyInt64Base: public QtnNumericPropertyBase<QtnSinglePropertyBase<qint64>>
{
	Q_OBJECT
	QtnPropertyInt64Base(const QtnPropertyInt64Base& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyInt64Base(QObject *parent);

protected:
	// string conversion implementation
	virtual bool fromStrImpl(const QString& str) override;
	virtual bool toStrImpl(QString& str) const override;

	// variant conversion implementation
	virtual bool fromVariantImpl(const QVariant& var) override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyInt64Base)
};


class QtnPropertyInt64: public QtnSinglePropertyValue<QtnPropertyInt64Base>
{
	Q_OBJECT
	QtnPropertyInt64(const QtnPropertyInt64& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyInt64(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyInt64, QtnPropertyInt64Base)

	static void Register();
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyInt64Base, qint64)

class  QtnPropertyInt64Callback: public QtnSinglePropertyCallback<QtnPropertyInt64Base>
{
	Q_OBJECT
	QtnPropertyInt64Callback(const QtnPropertyInt64Callback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyInt64Callback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyInt64Callback, QtnPropertyInt64Base)
};

class QLineEdit;
class QtnPropertyDelegateInt64: public QObject, public QtnPropertyDelegateTyped<QtnPropertyInt64Base>
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnPropertyDelegateInt64)

public:
	QtnPropertyDelegateInt64(QtnPropertyInt64Base& owner);

	virtual bool eventFilter(QObject *obj, QEvent *event) override;

protected:
	virtual bool acceptKeyPressedForInplaceEditImpl(QKeyEvent* keyEvent) const override;
	virtual QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString& strValue) const override;

private slots:
	void onEditingFinished();
	void onEditorDestroyed();

private:
	void updateEditor();

	QLineEdit *editor;
};
