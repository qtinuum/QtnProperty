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

#include "QObjectPropertyWidget.h"

#include "QObjectPropertySet.h"
#include "PropertyConnector.h"
#include "MultiProperty.h"

#include <QMenu>
#include <QContextMenuEvent>

QObjectPropertyWidget::QObjectPropertyWidget(QWidget *parent)
	: QtnPropertyWidgetEx(parent)
{
}

void QObjectPropertyWidget::deselectAllObjects()
{
	disconnectObjects();

	selectedObjects.clear();
}

void QObjectPropertyWidget::selectObject(QObject *object, bool addSelection)
{
	auto it = selectedObjects.find(object);

	if (it == selectedObjects.end()
	||	(!addSelection && selectedObjects.size() > 1))
	{
		if (addSelection)
			disconnectObjects();
		else
			deselectAllObjects();

		selectedObjects.insert(object);

		connectObjects();
	}
}

void QObjectPropertyWidget::selectObjects(const Objects &objects, bool addSelection)
{
	if (objects != selectedObjects)
	{
		if (addSelection)
		{
			disconnectObjects();

			selectedObjects.insert(objects.begin(), objects.end());
		} else
		{
			deselectAllObjects();

			selectedObjects = objects;
		}

		connectObjects();
	}
}

void QObjectPropertyWidget::deselectObject(QObject *object, bool destroyed)
{
	if (destroyed)
		onObjectDestroyed(object);
	else
	{
		auto it = selectedObjects.find(object);
		if (it != selectedObjects.end())
		{
			disconnectObjects();

			selectedObjects.erase(it);

			connectObjects();
		}
	}
}

void QObjectPropertyWidget::onResetTriggered()
{
	auto multiProperty = getMultiProperty();
	if (nullptr != multiProperty)
		multiProperty->resetValues(true);
	else
	{
		auto connector = getPropertyConnector();
		if (nullptr != connector)
			connector->resetPropertyValue(true);
	}
}

void QObjectPropertyWidget::onObjectDestroyed(QObject *object)
{
	auto it = selectedObjects.find(object);
	if (it != selectedObjects.end())
	{
		selectedObjects.erase(it);

		disconnectObjects();
		connectObjects();
	}
}

void QObjectPropertyWidget::contextMenuEvent(QContextMenuEvent *event)
{
	auto property = getActiveProperty();
	if (nullptr == property)
		return;

	if (!property->isEditableByUser())
		return;

	auto connector = getPropertyConnector();
	auto multiProperty = getMultiProperty();
	if (nullptr != connector || nullptr != multiProperty)
	{
		QMenu menu(this);

		auto action = menu.addAction(tr("Reset to default"));
		action->setStatusTip(tr("Reset value of %1 to default").arg(property->name()));

		bool resettable = (nullptr != multiProperty)
			? multiProperty->hasResettableValues()
			: connector->isResettablePropertyValue();

		action->setEnabled(resettable);

		if (resettable)
		{
			QObject::connect(action, &QAction::triggered,
							 this, &QObjectPropertyWidget::onResetTriggered);
		}

		menu.exec(event->globalPos());
	}
}

QtnMultiProperty *QObjectPropertyWidget::getMultiProperty() const
{
	return dynamic_cast<QtnMultiProperty *>(getActiveProperty());
}

QtnPropertyConnector *QObjectPropertyWidget::getPropertyConnector() const
{
	auto property = getActiveProperty();

	if (nullptr != property)
		return property->findChild<QtnPropertyConnector *>(QString(),
														Qt::FindDirectChildrenOnly);

	return nullptr;
}

void QObjectPropertyWidget::connectObjects()
{
	if (selectedObjects.size() == 1)
	{
		auto object = *selectedObjects.begin();
		auto set = qtnCreateQObjectPropertySet(object);
		if (nullptr != set)
			set->setParent(this);
		setPropertySet(set);

		connectObject(object);
	} else
	if (selectedObjects.size() > 1)
	{
		auto set = qtnCreateQObjectMultiPropertySet(selectedObjects);
		if (nullptr != set)
			set->setParent(this);
		setPropertySet(set);

		for (auto object : selectedObjects)
			connectObject(object);
	}

	hack();
}

void QObjectPropertyWidget::connectObject(QObject *object)
{
	Q_ASSERT(nullptr != object);

	QObject::connect(object, &QObject::destroyed,
					 this, &QObjectPropertyWidget::onObjectDestroyed);
}

void QObjectPropertyWidget::disconnectObjects()
{
	auto set = propertySet();
	setPropertySet(nullptr);
	delete set;

	hack();

	for (auto object : selectedObjects)
	{
		disconnectObject(object);
	}
}

void QObjectPropertyWidget::disconnectObject(QObject *object)
{
	Q_ASSERT(nullptr != object);

	QObject::disconnect(object, &QObject::destroyed,
						this, &QObjectPropertyWidget::onObjectDestroyed);
}

void QObjectPropertyWidget::hack()
{
	bool enabled = isEnabled();
	setEnabled(!enabled);
	setEnabled(enabled);
}
