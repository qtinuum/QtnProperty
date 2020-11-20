/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
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

#ifndef PROPERTYSTRING_H
#define PROPERTYSTRING_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"

class QTN_IMPORT_EXPORT QtnPropertyQStringBase
	: public QtnSinglePropertyBase<QString>
{
	Q_OBJECT

private:
	QtnPropertyQStringBase(
		const QtnPropertyQStringBase &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQStringBase(QObject *parent);

	QtnPropertyQStringBase &operator=(const char *newValue);

protected:
	// string conversion implementation
	bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	bool toStrImpl(QString &str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQStringBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyQStringBase, QString)

class QTN_IMPORT_EXPORT QtnPropertyQStringCallback
	: public QtnSinglePropertyCallback<QtnPropertyQStringBase>
{
	Q_OBJECT

private:
	QtnPropertyQStringCallback(
		const QtnPropertyQStringCallback &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQStringCallback(QObject *parent = nullptr);

	QtnPropertyQStringCallback &operator=(const char *newValue);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQStringCallback, QtnPropertyQStringBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQString
	: public QtnSinglePropertyValue<QtnPropertyQStringBase>
{
	Q_OBJECT

private:
	QtnPropertyQString(const QtnPropertyQString &other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyQString(QObject *parent = nullptr);

	QtnPropertyQString &operator=(const char *newValue);

	static bool isMultilineText(const QString &text);
	static QString getEmptyPlaceholderStr();
	static QString getPlaceholderStr(const QString &text, bool checkMultiline);
	static QString getReadOnlyPropertyTitleFormat();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(
		QtnPropertyQString, QtnPropertyQStringBase)
};

#endif // PROPERTYSTRING_H
