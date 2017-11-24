/*******************************************************************************
Copyright 2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
#include "Delegates/PropertyDelegate.h"

#include <QKeySequence>

class QTN_IMPORT_EXPORT QtnPropertyQKeySequenceBase
	: public QtnSinglePropertyBase<const QKeySequence &>
{
	Q_OBJECT

	QtnPropertyQKeySequenceBase(
		const QtnPropertyQKeySequenceBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQKeySequenceBase(QObject *parent);

protected:
	virtual bool fromStrImpl(const QString &str, bool edit) override;
	virtual bool toStrImpl(QString &str) const override;
	virtual bool fromVariantImpl(const QVariant &var, bool edit) override;
	virtual bool toVariantImpl(QVariant &var) const override;
};

class QTN_IMPORT_EXPORT QtnPropertyQKeySequenceCallback
	: public QtnSinglePropertyCallback<QtnPropertyQKeySequenceBase>
{
	Q_OBJECT

	QtnPropertyQKeySequenceCallback(
		const QtnPropertyQKeySequenceCallback &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQKeySequenceCallback(QObject *parent = nullptr);
};

class QTN_IMPORT_EXPORT QtnPropertyQKeySequence
	: public QtnSinglePropertyValue<QtnPropertyQKeySequenceBase>
{
	Q_OBJECT

	QtnPropertyQKeySequence(
		const QtnPropertyQKeySequence &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQKeySequence(QObject *parent = nullptr);

	static bool Register();
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQKeySequence
	: public QtnPropertyDelegateTyped<QtnPropertyQKeySequenceBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQKeySequence)

public:
	typedef QtnPropertyDelegateTyped<QtnPropertyQKeySequenceBase> Inherited;

	QtnPropertyDelegateQKeySequence(QtnPropertyQKeySequenceBase &owner);

protected:
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	virtual bool propertyValueToStr(QString &strValue) const override;
};
