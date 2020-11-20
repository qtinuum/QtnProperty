/*******************************************************************************
Copyright (c) 2015-2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#pragma once

#include "Auxiliary/PropertyTemplates.h"
#include "Delegates/Utils/PropertyDelegateMisc.h"

class QTN_IMPORT_EXPORT QtnPropertyUInt64Base
	: public QtnNumericPropertyBase<QtnSinglePropertyBase<quint64>>
{
	Q_OBJECT

private:
	QtnPropertyUInt64Base(const QtnPropertyUInt64Base &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyUInt64Base(QObject *parent);

protected:
	// string conversion implementation
	virtual bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	virtual bool toStrImpl(QString &str) const override;

	// variant conversion implementation
	virtual bool fromVariantImpl(
		const QVariant &var, QtnPropertyChangeReason reason) override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyUInt64Base)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyUInt64Base, quint64)

class QTN_IMPORT_EXPORT QtnPropertyUInt64Callback
	: public QtnSinglePropertyCallback<QtnPropertyUInt64Base>
{
	Q_OBJECT

private:
	QtnPropertyUInt64Callback(
		const QtnPropertyUInt64Callback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyUInt64Callback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyUInt64Callback, QtnPropertyUInt64Base)
};

class QTN_IMPORT_EXPORT QtnPropertyUInt64
	: public QtnNumericPropertyValue<QtnPropertyUInt64Base>
{
	Q_OBJECT

private:
	QtnPropertyUInt64(const QtnPropertyUInt64 &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyUInt64(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyUInt64, QtnPropertyUInt64Base)
};

class QLineEdit;
class QTN_IMPORT_EXPORT QtnPropertyDelegateUInt64
	: public QObject
	, public QtnPropertyDelegateTyped<QtnPropertyUInt64Base>
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnPropertyDelegateUInt64)

	QString m_suffix;
	QVariant m_min;
	QVariant m_max;

public:
	QtnPropertyDelegateUInt64(QtnPropertyUInt64Base &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

	quint64 minValue() const;
	quint64 maxValue() const;
	quint64 currentValue() const;

protected:
	virtual bool eventFilter(QObject *obj, QEvent *event) override;

	virtual bool acceptKeyPressedForInplaceEditImpl(
		QKeyEvent *keyEvent) const override;
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStrImpl(QString &strValue) const override;

	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;

private:
	void onEditingFinished();

private:
	void updateEditor();

	QLineEdit *editor;
	bool reverted;
	bool applied;
};
