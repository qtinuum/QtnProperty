/*******************************************************************************
Copyright (c) 2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "QtnProperty/Auxiliary/PropertyTemplates.h"
#include "QtnProperty/Core/PropertyFloat.h"
#include "QtnProperty/StructPropertyBase.h"

#include <QVector3D>

class QTN_IMPORT_EXPORT QtnPropertyQVector3DBase
	: public QtnStructPropertyBase<QVector3D, QtnPropertyFloatCallback>
{
	Q_OBJECT

private:
	QtnPropertyQVector3DBase(
		const QtnPropertyQVector3DBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQVector3DBase(QObject *parent);

	QtnProperty *createXProperty();
	QtnProperty *createYProperty();
	QtnProperty *createZProperty();

protected:
	// string conversion implementation
	bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQVector3DBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQVector3DBase, QVector3D)

class QTN_IMPORT_EXPORT QtnPropertyQVector3DCallback
	: public QtnSinglePropertyCallback<QtnPropertyQVector3DBase>
{
	Q_OBJECT

private:
	QtnPropertyQVector3DCallback(
		const QtnPropertyQVector3DCallback &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQVector3DCallback(
		QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQVector3DCallback, QtnPropertyQVector3DBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQVector3D
	: public QtnSinglePropertyValue<QtnPropertyQVector3DBase>
{
	Q_OBJECT

private:
	QtnPropertyQVector3D(const QtnPropertyQVector3D &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQVector3D(QObject *parent = nullptr);

	static QString zKey();
	static QString zDisplayName();
	static QString zDescriptionFmt();
	static QString getToStringFormat();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQVector3D, QtnPropertyQVector3DBase)
};
