#include "PropertyWidgetEx.h"

#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>

QtnPropertyWidgetEx::QtnPropertyWidgetEx(QWidget *parent)
	: QtnPropertyWidget(parent)
	, to_copy(nullptr)
{
	for (auto child : children())
	{
		auto vbox = dynamic_cast<QVBoxLayout *>(child);

		if (nullptr != vbox)
		{
			vbox->setContentsMargins(0, 0, 0, 0);
			vbox->setMargin(0);
			vbox->setSpacing(0);
		}
	}

	setAcceptDrops(true);
	propertyView()->installEventFilter(this);
}

bool QtnPropertyWidgetEx::eventFilter(QObject *obj, QEvent *event)
{
	switch (event->type())
	{
		case QEvent::MouseButtonPress:
		{
			auto mevent = static_cast<QMouseEvent *>(event);
			if (mevent->button() == Qt::LeftButton)
			{
				drag_start_pos = mevent->pos();
				to_copy = propertyView()->getPropertyAt(drag_start_pos);
				return true;
			}
		}	break;

		case QEvent::MouseMove:
		{
			auto mevent = static_cast<QMouseEvent *>(event);
			if (nullptr != to_copy
			&&	0 != (mevent->buttons() & Qt::LeftButton))
			{
				if ((mevent->pos() - drag_start_pos).manhattanLength()
					 < QApplication::startDragDistance())
				{
					QString str;
					if (to_copy->toStr(str))
					{
						auto drag = new QDrag(this);
						auto mime_data = new QMimeData;

						mime_data->setText(str);
						drag->setMimeData(mime_data);

						// TODO generate cursor

						drag->exec(Qt::CopyAction);
						return true;
					}
				}
			}
		}	break;

		default:
			break;
	}

	return QtnPropertyWidget::eventFilter(obj, event);
}

void QtnPropertyWidgetEx::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/plain"))
		event->acceptProposedAction();
}

void QtnPropertyWidgetEx::dropEvent(QDropEvent *event)
{
	switch (event->dropAction())
	{
		case Qt::MoveAction:
		case Qt::CopyAction:
		{
			auto view = propertyView();
			auto pos = view->mapFrom(this, event->pos());
			auto property = view->getPropertyAt(pos);
			if (nullptr != property && property != to_copy)
			{
				if (property->fromStr(event->mimeData()->text()))
					event->acceptProposedAction();
			}
		}	break;

		default:
			break;
	}
}

