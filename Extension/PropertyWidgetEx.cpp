#include "PropertyWidgetEx.h"

#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QShortcut>
#include <QClipboard>
#include <QAction>
#include <QDebug>

static const QString kTextPlain = "text/plain";
const QString *pTextPlain = &kTextPlain;

QtnPropertyWidgetEx::QtnPropertyWidgetEx(QWidget *parent)
	: QtnPropertyWidget(parent)
	, dragged_property(nullptr)
	, drop_action(Qt::IgnoreAction)
	, can_remove(false)
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

void QtnPropertyWidgetEx::connectRemoveAction(QAction *action, bool connect)
{
	internalConnect(action, &QtnPropertyWidgetEx::removeActiveProperty, connect);
}

void QtnPropertyWidgetEx::connectCutAction(QAction *action, bool connect)
{
	internalConnect(action, &QtnPropertyWidgetEx::cutToClipboard, connect);
}

void QtnPropertyWidgetEx::connectCopyAction(QAction *action, bool connect)
{
	internalConnect(action, &QtnPropertyWidgetEx::copyToClipboard, connect);
}

void QtnPropertyWidgetEx::connectPasteAction(QAction *action, bool connect)
{
	internalConnect(action, &QtnPropertyWidgetEx::pasteFromClipboard, connect);
}

bool QtnPropertyWidgetEx::canRemoveActiveProperty()
{
	return canRemoveProperty(propertyView()->activeProperty());
}

bool QtnPropertyWidgetEx::canRemoveProperty(QtnPropertyBase *)
{
	return false;
}

bool QtnPropertyWidgetEx::canCutToClipboard()
{
	return false;
}

bool QtnPropertyWidgetEx::canCopyToClipboard()
{
	return (nullptr != propertyView()->activeProperty());
}

bool QtnPropertyWidgetEx::canPasteFromClipboard()
{
	return dataHasSupportedFormats(QApplication::clipboard()->mimeData())
		&& (nullptr != propertyView()->activeProperty());
}

bool QtnPropertyWidgetEx::dataHasSupportedFormats(const QMimeData *data)
{
	return (nullptr != data && data->hasFormat(kTextPlain));
}

void QtnPropertyWidgetEx::removeActiveProperty()
{
	removeProperty(propertyView()->activeProperty());
}

void QtnPropertyWidgetEx::cutToClipboard()
{
	copyToClipboard();
	removeActiveProperty();
}

void QtnPropertyWidgetEx::copyToClipboard()
{
	auto property = propertyView()->activeProperty();
	if (nullptr != property)
	{
		auto mime = getPropertyDataForAction(property, Qt::IgnoreAction);
		if (nullptr != mime)
			QApplication::clipboard()->setMimeData(mime);
	}
}

void QtnPropertyWidgetEx::pasteFromClipboard()
{
	auto data = QApplication::clipboard()->mimeData();
	if (dataHasSupportedFormats(data))
	{
		applyPropertyData(data,
						  propertyView()->activeProperty(),
						  QtnApplyPosition::Over);
	}
}

void QtnPropertyWidgetEx::removeProperty(QtnPropertyBase *)
{
	// do nothing
}

QMimeData *QtnPropertyWidgetEx::getPropertyDataForAction(QtnPropertyBase *property,
														 Qt::DropAction)
{
	QString str;
	if (property->toStr(str))
	{
		auto mime = new QMimeData;
		mime->setText(str);
		return mime;
	}

	return nullptr;
}

bool QtnPropertyWidgetEx::applyPropertyData(const QMimeData *data,
											QtnPropertyBase *destination,
											QtnApplyPosition)
{
	if (nullptr != destination)
	{
		Q_ASSERT(nullptr != data);
		return destination->fromStr(data->text());
	}

	return false;
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
				dragged_property = propertyView()->getPropertyAt(drag_start_pos);
				can_remove = canRemoveProperty(dragged_property);
				return true;
			}
		}	break;

		case QEvent::MouseMove:
		{
			auto mevent = static_cast<QMouseEvent *>(event);
			if (nullptr != dragged_property
			&&	0 != (mevent->buttons() & Qt::LeftButton))
			{
				if ((mevent->pos() - drag_start_pos).manhattanLength()
					 < QApplication::startDragDistance())
				{
					drop_action = Qt::IgnoreAction;
					if (dragAndDrop())
						return true;
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
	if (dataHasSupportedFormats(event->mimeData()))
		event->acceptProposedAction();
}

void QtnPropertyWidgetEx::dragMoveEvent(QDragMoveEvent *event)
{
	if (Qt::ControlModifier == QApplication::keyboardModifiers()
	||	!can_remove)
	{
		event->setDropAction(Qt::CopyAction);
		drop_action = Qt::CopyAction;
	} else
	{
		event->setDropAction(Qt::MoveAction);		
		drop_action = Qt::MoveAction;
	}

	event->accept();
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
			QRect rect;
			auto property = view->getPropertyAt(pos, &rect);
			int part_height = view->itemHeight() / 3;

			QtnApplyPosition apply_position;
			if (QRect(rect.left(), rect.top(),
					  rect.width(), part_height).contains(pos))
				apply_position = QtnApplyPosition::Before;
			else if (QRect(rect.left(), rect.bottom() - part_height,
						   rect.width(), part_height).contains(pos))
				apply_position = QtnApplyPosition::After;
			else
				apply_position = QtnApplyPosition::Over;

			if (property == dragged_property
			||	nullptr == property)
			{
				dragged_property = nullptr;
			}

			auto data = event->mimeData();
			if (nullptr != dragged_property
			&&	dataHasSupportedFormats(data)
			&&	applyPropertyData(data, property, apply_position))
				event->accept();
		}	break;

		default:
			break;
	}
}

bool QtnPropertyWidgetEx::dragAndDrop()
{
	auto data = getPropertyDataForAction(dragged_property, Qt::CopyAction);
	if (nullptr != data)
	{
		auto drag = new QDrag(this);

		drag->setMimeData(data);

		// TODO generate cursor

		drag->exec(Qt::CopyAction | Qt::MoveAction);

		if (nullptr != dragged_property)
		{
			if (Qt::MoveAction == drop_action)
				removeProperty(dragged_property);
		}
		return true;
	}

	return false;
}

void QtnPropertyWidgetEx::internalConnect(QAction *action,
										  void (QtnPropertyWidgetEx::*slot)(),
										  bool connect)
{
	if (connect)
		QObject::connect(action, &QAction::triggered, this, slot);
	else
		QObject::disconnect(action, &QAction::triggered, this, slot);
}

