/*
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

#include "Auxiliary/PropertyTemplates.h"
#include "Delegates/PropertyDelegate.h"

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
	virtual bool fromStrImpl(const QString &str, bool edit) override;
	virtual bool toStrImpl(QString &str) const override;

	// variant conversion implementation
	virtual bool fromVariantImpl(const QVariant &var, bool edit) override;

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
	: public QtnSinglePropertyValue<QtnPropertyUInt64Base>
{
	Q_OBJECT

private:
	QtnPropertyUInt64(const QtnPropertyUInt64 &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyUInt64(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyUInt64, QtnPropertyUInt64Base)

	static void Register();
};

class QLineEdit;
class QTN_IMPORT_EXPORT QtnPropertyDelegateUInt64
	: public QObject
	, public QtnPropertyDelegateTyped<QtnPropertyUInt64Base>
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnPropertyDelegateUInt64)

public:
	QtnPropertyDelegateUInt64(QtnPropertyUInt64Base &owner);

	virtual bool eventFilter(QObject *obj, QEvent *event) override;

protected:
	virtual bool acceptKeyPressedForInplaceEditImpl(
		QKeyEvent *keyEvent) const override;
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual bool propertyValueToStr(QString &strValue) const override;

private slots:
	void onEditingFinished();
	void onEditorDestroyed();

private:
	void updateEditor();

	QLineEdit *editor;
	bool reverted;
	bool applied;
};
