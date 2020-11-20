/*******************************************************************************
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyWidgetEx.h"

#include "PropertyView.h"
#include "Utils/QtnConnections.h"

#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QShortcut>
#include <QClipboard>
#include <QAction>
#include <QColor>
#include <QMenu>
#include <QContextMenuEvent>

QtnPropertyWidgetEx::QtnPropertyWidgetEx(QWidget *parent)
	: QtnPropertyWidget(parent)
	, draggedProperty(nullptr)
	, mDrag(nullptr)
	, dropAction(Qt::IgnoreAction)
	, canRemove(false)
{
	setAcceptDrops(true);
	propertyView()->installEventFilter(this);

	QObject::connect(propertyView(), &QtnPropertyView::mouseReleased, this,
		&QtnPropertyWidgetEx::onMouseReleased);
}

void QtnPropertyWidgetEx::connectDeleteAction(QAction *action, bool connect)
{
	internalConnect(
		action, &QtnPropertyWidgetEx::deleteActiveProperty, connect);
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
	return dataHasSupportedFormats(QApplication::clipboard()->mimeData()) &&
		(nullptr != getActiveProperty());
}

QtnPropertyBase *QtnPropertyWidgetEx::getActiveProperty() const
{
	return propertyView()->activeProperty();
}

void QtnPropertyWidgetEx::addShortcutForAction(const QKeySequence &seq,
	QAction *action, QWidget *parent, Qt::ShortcutContext shortcutContext)
{
	if (seq.isEmpty())
		return;

	Q_ASSERT(nullptr != action);
	Q_ASSERT(nullptr != parent);

	auto shortcut =
		new QShortcut(seq, parent, nullptr, nullptr, shortcutContext);
	QObject::connect(
		shortcut, &QShortcut::activated, action, &QAction::trigger);
}

void QtnPropertyWidgetEx::onMouseReleased()
{
	draggedProperty = nullptr;
}

void QtnPropertyWidgetEx::onResetTriggered()
{
	auto activeProperty = getActiveProperty();
	if (activeProperty && activeProperty->isResettable() &&
		activeProperty->isEditableByUser())
	{
		QtnConnections connections;
		propertyView()->connectPropertyToEdit(activeProperty, connections);
		QtnPropertyChangeReason reason = QtnPropertyChangeReasonEdit;
		activeProperty->reset(reason);
	}
}

void QtnPropertyWidgetEx::onLockToggleTriggered()
{
	auto activeProperty = getActiveProperty();
	if (activeProperty && activeProperty->isUnlockable() &&
		activeProperty->isVisible())
	{
		QtnConnections connections;
		propertyView()->connectPropertyToEdit(activeProperty, connections);
		QtnPropertyChangeReason reason = QtnPropertyChangeReasonEdit;
		activeProperty->toggleLock(reason);
	}
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
		applyPropertyData(data, getActiveProperty(), QtnApplyPosition::None);
	}
}

void QtnPropertyWidgetEx::contextMenuEvent(QContextMenuEvent *event)
{
	auto property = getActiveProperty();
	if (!property)
		return;

	auto menu = new QMenu;
	int actionCount = 0;

	if (property->isResettable() && property->isVisible() &&
		!property->valueIsDefault() &&
		(!property->isUnlockable() || !property->isLocked()))
	{
		auto action = menu->addAction(tr("Reset to default"));
		action->setIcon(QtnPropertyDelegate::resetIcon());
		action->setStatusTip(
			tr("Reset value of %1 to default").arg(property->displayName()));

		QObject::connect(action, &QAction::triggered, this,
			&QtnPropertyWidgetEx::onResetTriggered);

		actionCount++;
	}

	if (property->isUnlockable() && property->isVisible())
	{
		auto action = menu->addAction(
			property->isLocked() ? tr("Unlock property") : tr("Lock property"));
		auto statusFmt = property->isLocked() ? tr("Unlock %1") : tr("Lock %1");
		action->setStatusTip(statusFmt.arg(property->displayName()));

		QObject::connect(action, &QAction::triggered, this,
			&QtnPropertyWidgetEx::onLockToggleTriggered);

		actionCount++;
	}

	if (actionCount == 0)
	{
		delete menu;
		return;
	}

	menu->setAttribute(Qt::WA_DeleteOnClose);
	menu->popup(event->globalPos());
}

void QtnPropertyWidgetEx::deleteProperty(QtnPropertyBase *)
{
	// do nothing
}

QMimeData *QtnPropertyWidgetEx::getPropertyDataForAction(
	QtnPropertyBase *property, Qt::DropAction)
{
	Q_ASSERT(nullptr != property);

	QString str;

	if (property->toStr(str))
	{
		auto mime = new QMimeData;
		mime->setText(str);
		return mime;
	}

	return nullptr;
}

bool QtnPropertyWidgetEx::applyPropertyData(
	const QMimeData *data, QtnPropertyBase *destination, QtnApplyPosition)
{
	if (nullptr != destination && destination->isWritable())
	{
		QtnConnections connections;

		propertyView()->connectPropertyToEdit(destination, connections);

		Q_ASSERT(nullptr != data);

		QString str;
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

			str = list.join('\n');
		} else if (data->hasColor())
		{
			auto color = data->colorData().value<QColor>();
			str = color.name(
				(color.alpha() < 255) ? QColor::HexArgb : QColor::HexRgb);
		} else if (data->hasText())
		{
			str = data->text();
		} else
		{
			return false;
		}
		return destination->fromStr(str, QtnPropertyChangeReasonEdit);
	}

	return false;
}

bool QtnPropertyWidgetEx::eventFilter(QObject *obj, QEvent *event)
{
	switch (event->type())
	{
		case QEvent::MouseButtonPress:
		{
			if (draggedProperty)
				break;

			auto mevent = static_cast<QMouseEvent *>(event);

			if (mevent->button() == Qt::LeftButton)
			{
				dragStartPos = mevent->pos();
				draggedProperty = propertyView()->getPropertyAt(dragStartPos);
				canRemove = canDeleteProperty(draggedProperty);
				return true;
			}

			break;
		}

		case QEvent::MouseMove:
		{
			if (mDrag)
				break;

			auto mevent = static_cast<QMouseEvent *>(event);

			if (nullptr != draggedProperty &&
				0 != (mevent->buttons() & Qt::LeftButton))
			{
				if ((mevent->pos() - dragStartPos).manhattanLength() >=
					QApplication::startDragDistance())
				{
					dragAndDrop();
					return true;
				}
			}

			break;
		}

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
	if (Qt::ControlModifier == QApplication::keyboardModifiers() || !canRemove)
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
			auto destination = view->getPropertyAt(pos, &rect);

			if (destination == draggedProperty)
			{
				draggedProperty = nullptr;
				break;
			}

			QtnApplyPosition applyPosition;

			if (destination == nullptr)
			{
				applyPosition = QtnApplyPosition::After;
			} else
			{
				int partHeight = view->itemHeight() / 3;

				if (QRect(rect.left(), rect.top(), rect.width(), partHeight)
						.contains(pos))
					applyPosition = QtnApplyPosition::Before;
				else if (QRect(rect.left(), rect.bottom() - partHeight,
							 rect.width(), partHeight)
							 .contains(pos))
					applyPosition = QtnApplyPosition::After;
				else
					applyPosition = QtnApplyPosition::Over;
			}

			auto data = event->mimeData();

			if (dataHasSupportedFormats(data) &&
				drop(data, destination, applyPosition))
			{
				event->accept();
			}
			break;
		}

		default:
			break;
	}
}

bool QtnPropertyWidgetEx::drop(const QMimeData *data, QtnPropertyBase *property,
	QtnApplyPosition applyPosition)
{
	return applyPropertyData(data, property, applyPosition);
}

void QtnPropertyWidgetEx::dropEnd()
{
	if (nullptr != draggedProperty)
	{
		if (Qt::MoveAction == dropAction)
			deleteProperty(draggedProperty);
	}
}

void QtnPropertyWidgetEx::onDropFinished(Qt::DropAction)
{
	dropEnd();
	draggedProperty = nullptr;
	mDrag = nullptr;
}

bool QtnPropertyWidgetEx::dragAndDrop()
{
	dropAction = Qt::IgnoreAction;
	auto data = getPropertyDataForAction(draggedProperty, Qt::CopyAction);

	if (nullptr != data)
	{
		mDrag = new QDrag(this);

		mDrag->setMimeData(data);

		// TODO generate cursor

#ifdef Q_OS_WASM
		QObject::connect(mDrag, &QDrag::finished, this,
			&QtnPropertyWidgetEx::onDropFinished);
		mDrag->exec(Qt::CopyAction | Qt::MoveAction);
#else
		onDropFinished(mDrag->exec(Qt::CopyAction | Qt::MoveAction));
#endif

		return true;
	}

	draggedProperty = nullptr;
	return false;
}

void QtnPropertyWidgetEx::internalConnect(
	QAction *action, void (QtnPropertyWidgetEx::*slot)(), bool connect)
{
	if (connect)
		QObject::connect(action, &QAction::triggered, this, slot);
	else
		QObject::disconnect(action, &QAction::triggered, this, slot);
}

QtnPropertyWidgetExDelegate::~QtnPropertyWidgetExDelegate() {}
