/*******************************************************************************
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyWidgetEx.h"

#include <QMetaObject>
#include <set>

class QtnPropertyConnector;

class QtnMultiProperty;

class QTN_IMPORT_EXPORT QObjectPropertyWidget : public QtnPropertyWidgetEx
{
	Q_OBJECT

public:
	explicit QObjectPropertyWidget(QWidget *parent = nullptr);

	typedef std::set<QObject *> Objects;

	inline const Objects &getSelectedObjects() const;

public slots:
	void deselectAllObjects();
	void selectObject(QObject *object, bool addSelection = true);
	void selectObjects(const Objects &objects, bool addSelection = true);
	void deselectObject(QObject *object, bool destroyed = false);

private:
	void onObjectDestroyed(QObject *object);

protected:
	QtnMultiProperty *getMultiProperty() const;
	QtnPropertyConnector *getPropertyConnector() const;

	void connectObjects();
	void disconnectObjects();

	void disconnectObject(QObject *object);
	void connectObject(QObject *object);

	Objects selectedObjects;
};

const QObjectPropertyWidget::Objects & //
QObjectPropertyWidget::getSelectedObjects() const
{
	return selectedObjects;
}
