#pragma once

#include "QtnProperty/Widget/PropertyWidget.h"

class QtnPropertyWidgetEx : public QtnPropertyWidget
{
	Q_OBJECT

public:
	explicit QtnPropertyWidgetEx(QWidget *parent = nullptr);

protected:
	virtual bool eventFilter(QObject *obj, QEvent *event) override;

	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;

private:
	QPoint drag_start_pos;
	QtnPropertyBase *to_copy;
};
