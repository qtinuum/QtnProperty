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
#include "Core/PropertyDouble.h"
#include "Delegates/Core/PropertyDelegateDouble.h"

class QTN_IMPORT_EXPORT QtnPropertyPercentBase
	: public QtnPropertyDoubleBase
{
	Q_OBJECT

public:
	explicit QtnPropertyPercentBase(QObject *parent);

protected:
	virtual bool fromStrImpl(const QString& str, bool edit) override;
	virtual bool toStrImpl(QString& str) const override;
};

class QTN_IMPORT_EXPORT QtnPropertyPercent
	: public QtnSinglePropertyValue<QtnPropertyPercentBase>
{
	Q_OBJECT
	QtnPropertyPercent(const QtnPropertyPercent& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyPercent(QObject *parent);

	static void Register();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyPercent, QtnPropertyPercentBase)
};

class QTN_IMPORT_EXPORT QtnPropertyPercentCallback
	: public QtnSinglePropertyCallback<QtnPropertyPercentBase>
{
	Q_OBJECT
	QtnPropertyPercentCallback(const QtnPropertyPercentCallback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyPercentCallback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyPercentCallback, QtnPropertyPercentBase)
};

class QTN_IMPORT_EXPORT QtnPropertyDelegatePercent
	: public QtnPropertyDelegateDouble
{
public:
	QtnPropertyDelegatePercent(QtnPropertyPercentBase& owner);
};
