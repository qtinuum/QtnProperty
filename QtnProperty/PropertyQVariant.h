/*******************************************************************************
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include <QVariant>

class QTN_IMPORT_EXPORT QtnPropertyQVariantBase
	: public QtnSinglePropertyBase<QVariant>
{
	Q_OBJECT

private:
	QtnPropertyQVariantBase(
		const QtnPropertyQVariantBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVariantBase(QObject *parent);

protected:
	virtual bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	virtual bool toStrImpl(QString &str) const override;
	virtual bool fromVariantImpl(
		const QVariant &var, QtnPropertyChangeReason reason) override;
	virtual bool toVariantImpl(QVariant &var) const override;
};

class QTN_IMPORT_EXPORT QtnPropertyQVariantCallback
	: public QtnSinglePropertyCallback<QtnPropertyQVariantBase>
{
	Q_OBJECT

private:
	QtnPropertyQVariantCallback(
		const QtnPropertyQVariantCallback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVariantCallback(QObject *object,
		const QMetaProperty &metaProperty, QObject *parent = nullptr);
	explicit QtnPropertyQVariantCallback(QObject *parent = nullptr);
};

class QTN_IMPORT_EXPORT QtnPropertyQVariant
	: public QtnSinglePropertyValue<QtnPropertyQVariantBase>
{
	Q_OBJECT

private:
	QtnPropertyQVariant(const QtnPropertyQVariant &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVariant(QObject *parent);

	static QString valueToString(const QVariant &value);
	static bool variantIsObject(QVariant::Type type);
	static QString getPlaceholderStr(QVariant::Type type);
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQVariant
	: public QtnPropertyDelegateTyped<QtnPropertyQVariantBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQVariant)

public:
	typedef QtnPropertyDelegateTyped<QtnPropertyQVariantBase> Inherited;

	QtnPropertyDelegateQVariant(QtnPropertyQVariantBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	virtual bool acceptKeyPressedForInplaceEditImpl(
		QKeyEvent *keyEvent) const override;

	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	virtual bool propertyValueToStrImpl(QString &strValue) const override;
	virtual bool isPlaceholderColor() const override;
};

QTN_IMPORT_EXPORT bool qtnCompareQVariants(
	const QVariant &left, const QVariant &right);
