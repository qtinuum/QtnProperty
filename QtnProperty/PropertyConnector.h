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

#include "CoreAPI.h"

#include <QObject>
#include <QMetaProperty>

class QtnPropertyBase;
class QtnPropertySet;

class QTN_IMPORT_EXPORT QtnPropertyConnector : public QObject
{
	Q_OBJECT

public:
	explicit QtnPropertyConnector(QtnPropertyBase *parent);

	void connectProperty(QObject *object, const QMetaProperty &metaProperty);

	bool isResettablePropertyValue() const;
	void resetPropertyValue(bool edit);

	inline QObject *getObject() const;
	inline const QMetaProperty &getMetaProperty() const;

private slots:
	void onValueChanged();
	void onModifiedSetChanged();

private:
	QObject *object;
	QMetaProperty metaProperty;
};

QObject *QtnPropertyConnector::getObject() const
{
	return object;
}

const QMetaProperty &QtnPropertyConnector::getMetaProperty() const
{
	return metaProperty;
}
