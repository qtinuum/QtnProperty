/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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

#ifndef PROPERTYSTRING_H
#define PROPERTYSTRING_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"

class QTN_IMPORT_EXPORT QtnPropertyQStringBase: public QtnSinglePropertyBase<QString>
{
	Q_OBJECT
	QtnPropertyQStringBase(const QtnPropertyQStringBase& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQStringBase(QObject *parent);

	bool isMultilineEnabled() const;
	void setMultilineEnabled(bool enabled);

	QString getPlaceholderText() const;

	QtnPropertyQStringBase& operator=(const char* newValue);

protected:
	// string conversion implementation
	bool fromStrImpl(const QString& str, bool edit) override;
	bool toStrImpl(QString& str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQStringBase)

private:
	bool multiline;
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyQStringBase, QString)

class QTN_IMPORT_EXPORT QtnPropertyQStringCallback: public QtnSinglePropertyCallback<QtnPropertyQStringBase>
{
	Q_OBJECT
	QtnPropertyQStringCallback(const QtnPropertyQStringCallback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQStringCallback(QObject *parent);

	QtnPropertyQStringCallback& operator=(const char* newValue);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQStringCallback, QtnPropertyQStringBase)
};

class QTN_IMPORT_EXPORT QtnPropertyQString: public QtnSinglePropertyValue<QtnPropertyQStringBase>
{
	Q_OBJECT
	QtnPropertyQString(const QtnPropertyQString& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyQString(QObject *parent);

	QtnPropertyQString& operator=(const char* newValue);

	static bool isMultilineText(const QString &text);
	static QString getPlaceholderStr(const QString &text, bool check_multiline);
	static QString getReadOnlyPropertyTitleFormat();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQString, QtnPropertyQStringBase)
};

#endif // PROPERTYSTRING_H
