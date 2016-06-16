#include "QObjectPropertyWidget.h"

#include "Extension.h"

#include <QMenu>
#include <QContextMenuEvent>

using namespace QtnPropertyExtension;

QObjectPropertyWidget::QObjectPropertyWidget(QWidget *parent)
	: QtnPropertyWidgetEx(parent)
	, selected_object(nullptr)
{
}

void QObjectPropertyWidget::selectObject(QObject *object)
{
	if (object != selected_object)
	{
		onObjectDeselect();

		selected_object = object;

		onObjectSelect();
	}
}

void QObjectPropertyWidget::onResetTriggered()
{
	auto connector = getPropertyConnector();
	if (nullptr != connector)
		connector->resetPropertyValue();
}

void QObjectPropertyWidget::contextMenuEvent(QContextMenuEvent *event)
{
	auto connector = getPropertyConnector();
	if (nullptr != connector)
	{
		QMenu menu(this);

		auto action = menu.addAction(tr("Reset to default"));
		action->setStatusTip(tr("Reset value of %1 to default").arg(getActiveProperty()->name()));
		bool resettable = connector->isResettablePropertyValue();
		action->setEnabled(resettable);
		if (resettable)
			QObject::connect(action, &QAction::triggered, this, &QObjectPropertyWidget::onResetTriggered);

		menu.exec(event->globalPos());
	}
}

QtnPropertyBase *QObjectPropertyWidget::getActiveProperty()
{
	return propertyView()->activeProperty();
}

PropertyConnector *QObjectPropertyWidget::getPropertyConnector()
{
	auto property = getActiveProperty();

	if (nullptr != property)
		return property->findChild<PropertyConnector *>(QString(), Qt::FindDirectChildrenOnly);

	return nullptr;
}

void QObjectPropertyWidget::onObjectSelect()
{
	if (nullptr != selected_object)
	{
		auto property_set = QtnPropertyExtension::CreateQObjectPropertySet(selected_object);
		property_set->setParent(this);
		setPropertySet(property_set);

		connection = QObject::connect(selected_object, &QObject::destroyed, [this](QObject *object)
		{
			if (selected_object == object)
				onObjectDeselect(false);
		});

		hack();
	}
}

void QObjectPropertyWidget::onObjectDeselect(bool disconnect)
{
	if (nullptr != selected_object)
	{
		if (disconnect)
			QObject::disconnect(connection);

		auto old_set = propertySet();
		setPropertySet(nullptr);
		delete old_set;
		selected_object = nullptr;

		hack();
	}
}

void QObjectPropertyWidget::hack()
{
	bool enabled = isEnabled();
	setEnabled(!enabled);
	setEnabled(enabled);
}
