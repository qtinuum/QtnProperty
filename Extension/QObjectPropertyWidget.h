#pragma once

#include "PropertyWidgetEx.h"

#include <QMetaObject>

namespace QtnPropertyExtension
{
	class PropertyConnector;
}

class QObjectPropertyWidget : public QtnPropertyWidgetEx
{
	Q_OBJECT

public:
	explicit QObjectPropertyWidget(QWidget *parent = nullptr);

public slots:
	void selectObject(QObject *object);

private slots:
	void onResetTriggered();

protected:
	virtual void contextMenuEvent(QContextMenuEvent *event) override;

	QtnPropertyExtension::PropertyConnector *getPropertyConnector();

	void onObjectSelect();
	void onObjectDeselect(bool disconnect = true);

	QObject *selected_object;

private:
	void hack();

	QMetaObject::Connection connection;
};
