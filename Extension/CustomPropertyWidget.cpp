#include "CustomPropertyWidget.h"

#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDrag>
#include <QMimeData>

static const QString kDragDropMimeType = "QtnPropertyDragDrop";

CustomPropertyWidget::CustomPropertyWidget(QWidget *parent)
{
	setAcceptDrops(true);
}

void CustomPropertyWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		auto view = propertyView();

		auto property = view->getPropertyAt(event->pos());

		if (nullptr != property
		&&	0 != (property->stateLocal() & QtnPropertyStateDraggable))
		{

		}
	}
}


void CustomPropertyWidget::dragEnterEvent(QDragEnterEvent *event)
{

}

void CustomPropertyWidget::dropEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(kDragDropMimeType))
		event->acceptProposedAction();
}
