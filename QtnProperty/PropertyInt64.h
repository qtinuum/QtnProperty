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

class QTN_IMPORT_EXPORT QtnPropertyInt64Base
	: public QtnNumericPropertyBase<QtnSinglePropertyBase<qint64>>
{
	Q_OBJECT

private:
	QtnPropertyInt64Base(const QtnPropertyInt64Base &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyInt64Base(QObject *parent);

protected:
	// string conversion implementation
	virtual bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	virtual bool toStrImpl(QString &str) const override;

	// variant conversion implementation
	virtual bool fromVariantImpl(
		const QVariant &var, QtnPropertyChangeReason reason) override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyInt64Base)
};

class QTN_IMPORT_EXPORT QtnPropertyInt64
	: public QtnNumericPropertyValue<QtnPropertyInt64Base>
{
	Q_OBJECT

private:
	QtnPropertyInt64(const QtnPropertyInt64 &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyInt64(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyInt64, QtnPropertyInt64Base)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyInt64Base, qint64)

class QTN_IMPORT_EXPORT QtnPropertyInt64Callback
	: public QtnSinglePropertyCallback<QtnPropertyInt64Base>
{
	Q_OBJECT

private:
	QtnPropertyInt64Callback(
		const QtnPropertyInt64Callback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyInt64Callback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyInt64Callback, QtnPropertyInt64Base)
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateInt64
	: public QtnPropertyDelegateTyped<QtnPropertyInt64Base>
{
	Q_DISABLE_COPY(QtnPropertyDelegateInt64)

	QString m_suffix;
	QVariant m_min;
	QVariant m_max;
	QVariant m_step;

public:
	QtnPropertyDelegateInt64(QtnPropertyInt64Base &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

	qint64 stepValue() const;
	qint64 minValue() const;
	qint64 maxValue() const;
	qint64 currentValue() const;

protected:
	virtual bool acceptKeyPressedForInplaceEditImpl(
		QKeyEvent *keyEvent) const override;

	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;

	virtual bool propertyValueToStrImpl(QString &strValue) const override;
};
