/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
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

#ifndef PROPERTYFLOAT_H
#define PROPERTYFLOAT_H

#include "QtnProperty/Auxiliary/PropertyTemplates.h"

class QTN_IMPORT_EXPORT QtnPropertyFloatBase: public QtnNumericPropertyBase<QtnSinglePropertyBase<float> >
{
	Q_OBJECT
	QtnPropertyFloatBase(const QtnPropertyFloatBase& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyFloatBase(QObject *parent);

protected:
	// string conversion implementation
	bool fromStrImpl(const QString& str, bool edit) override;
	bool toStrImpl(QString& str) const override;

	// variant conversion implementation
	bool fromVariantImpl(const QVariant& var, bool edit) override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyFloatBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyFloatBase, float)

class QTN_IMPORT_EXPORT QtnPropertyFloatCallback: public QtnSinglePropertyCallback<QtnPropertyFloatBase>
{
	Q_OBJECT
	QtnPropertyFloatCallback(const QtnPropertyFloatCallback& other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyFloatCallback(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyFloatCallback, QtnPropertyFloatBase)
};

class QTN_IMPORT_EXPORT QtnPropertyFloat: public QtnSinglePropertyValue<QtnPropertyFloatBase>
{
	Q_OBJECT
	QtnPropertyFloat(const QtnPropertyFloat& other) Q_DECL_EQ_DELETE;

public:
	Q_INVOKABLE explicit QtnPropertyFloat(QObject *parent = nullptr);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyFloat, QtnPropertyFloatBase)
};

#endif // PROPERTYFLOAT_H
