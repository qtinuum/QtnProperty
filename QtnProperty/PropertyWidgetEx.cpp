#include "PropertyWidgetEx.h"

#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QShortcut>
#include <QClipboard>
#include <QAction>
#include <QColor>

QtnPropertyWidgetEx::QtnPropertyWidgetEx(QWidget *parent)
	: QtnPropertyWidget(parent)
	, draggedProperty(nullptr)
	, dropAction(Qt::IgnoreAction)
	, canRemove(false)
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

	QObject::connect(propertyView(), &QtnPropertyView::mouseReleased,
					 this, &QtnPropertyWidgetEx::onMouseReleased);
}

void QtnPropertyWidgetEx::connectDeleteAction(QAction *action, bool connect)
{
	internalConnect(action, &QtnPropertyWidgetEx::deleteActiveProperty, connect);
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

bool QtnPropertyWidgetEx::canDeleteActiveProperty()
{
	return canDeleteProperty(getActiveProperty());
}

bool QtnPropertyWidgetEx::canDeleteProperty(QtnPropertyBase *)
{
	return false;
}

bool QtnPropertyWidgetEx::canCutToClipboard()
{
	return false;
}

bool QtnPropertyWidgetEx::canCopyToClipboard()
{
	return (nullptr != getActiveProperty());
}

bool QtnPropertyWidgetEx::canPasteFromClipboard()
{
	return dataHasSupportedFormats(QApplication::clipboard()->mimeData())
			&& (nullptr != getActiveProperty());
}

QtnPropertyBase *QtnPropertyWidgetEx::getActiveProperty() const
{
	return propertyView()->activeProperty();
}

void QtnPropertyWidgetEx::onMouseReleased()
{
	draggedProperty = nullptr;
}

bool QtnPropertyWidgetEx::dataHasSupportedFormats(const QMimeData *data)
{
	if (nullptr != data)
	{
		return data->hasText() || data->hasUrls() || data->hasColor();
	}

	return false;
}

void QtnPropertyWidgetEx::deleteActiveProperty()
{
	deleteProperty(getActiveProperty());
}

void QtnPropertyWidgetEx::cutToClipboard()
{
	copyToClipboard();
	deleteActiveProperty();
}

void QtnPropertyWidgetEx::copyToClipboard()
{
	auto property = getActiveProperty();
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
						  getActiveProperty(),
						  QtnApplyPosition::Over);
	}
}

void QtnPropertyWidgetEx::deleteProperty(QtnPropertyBase *)
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
		if (data->hasUrls())
		{
			QStringList list;

			for (auto &url : data->urls())
			{
				if (url.isLocalFile())
					list.push_back(url.toLocalFile());
				else
					list.push_back(url.toString());
			}

			return destination->fromStr(list.join('\n'), true);
		}

		if (data->hasColor())
			return destination->fromStr(data->colorData().value<QColor>().name(), true);

		if (data->hasText())
			return destination->fromStr(data->text(), true);
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
				dragStartPos = mevent->pos();
				draggedProperty = propertyView()->getPropertyAt(dragStartPos);
				canRemove = canDeleteProperty(draggedProperty);
				return true;
			}
		}	break;

		case QEvent::MouseMove:
		{
			auto mevent = static_cast<QMouseEvent *>(event);
			if (nullptr != draggedProperty
			&&	0 != (mevent->buttons() & Qt::LeftButton))
			{
				if ((mevent->pos() - dragStartPos).manhattanLength()
					 < QApplication::startDragDistance())
				{
					dropAction = Qt::IgnoreAction;
					dragAndDrop();
					draggedProperty = nullptr;
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
	||	!canRemove)
	{
		event->setDropAction(Qt::CopyAction);
		dropAction = Qt::CopyAction;
	} else
	{
		event->setDropAction(Qt::MoveAction);
		dropAction = Qt::MoveAction;
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

			if (property == draggedProperty
			||	nullptr == property)
			{
				draggedProperty = nullptr;
				break;
			}

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

			auto data = event->mimeData();
			if (dataHasSupportedFormats(data)
			&&	applyPropertyData(data, property, apply_position))
				event->accept();
		}	break;

		default:
			break;
	}
}

bool QtnPropertyWidgetEx::dragAndDrop()
{
	auto data = getPropertyDataForAction(draggedProperty, Qt::CopyAction);
	if (nullptr != data)
	{
		auto drag = new QDrag(this);

		drag->setMimeData(data);

		// TODO generate cursor

		drag->exec(Qt::CopyAction | Qt::MoveAction);

		if (nullptr != draggedProperty)
		{
			if (Qt::MoveAction == dropAction)
				deleteProperty(draggedProperty);
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

