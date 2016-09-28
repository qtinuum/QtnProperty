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

private slots:
	void onResetTriggered();
	void onObjectDestroyed(QObject *object);

protected:
	virtual void contextMenuEvent(QContextMenuEvent *event) override;

	QtnMultiProperty *getMultiProperty() const;
	QtnPropertyConnector *getPropertyConnector() const;
	void hack();

	void connectObjects();
	void disconnectObjects();

	void disconnectObject(QObject *object);
	void connectObject(QObject *object);

	Objects selectedObjects;
};

const QObjectPropertyWidget::Objects &QObjectPropertyWidget::getSelectedObjects() const
{
	return selectedObjects;
}
