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

#pragma once

#include "PropertyWidget.h"

class QMimeData;
class QShortcut;
class QDrag;

enum class QtnApplyPosition
{
	None,
	Before,
	Over,
	After
};

struct QTN_IMPORT_EXPORT QtnPropertyWidgetExDelegate
{
	virtual ~QtnPropertyWidgetExDelegate();

	virtual bool canDeleteProperty(QtnPropertyBase *property) = 0;
	virtual bool canCutToClipboard() = 0;
	virtual bool canCopyToClipboard() = 0;
	virtual bool canPasteFromClipboard() = 0;
	virtual bool dataHasSupportedFormats(const QMimeData *data) = 0;
	virtual void deleteProperty(QtnPropertyBase *property) = 0;
	virtual QMimeData *getPropertyDataForAction(
		QtnPropertyBase *property, Qt::DropAction action) = 0;
	virtual bool applyPropertyData(const QMimeData *data,
		QtnPropertyBase *destination, QtnApplyPosition position) = 0;
};

class QTN_IMPORT_EXPORT QtnPropertyWidgetEx
	: public QtnPropertyWidget
	, public QtnPropertyWidgetExDelegate
{
	Q_OBJECT

public:
	explicit QtnPropertyWidgetEx(QWidget *parent = nullptr);

	void connectDeleteAction(QAction *dropAction, bool connect);
	void connectCutAction(QAction *dropAction, bool connect);
	void connectCopyAction(QAction *dropAction, bool connect);
	void connectPasteAction(QAction *dropAction, bool connect);

	bool canDeleteActiveProperty();

	virtual bool canDeleteProperty(QtnPropertyBase *property) override;
	virtual bool canCutToClipboard() override;
	virtual bool canCopyToClipboard() override;
	virtual bool canPasteFromClipboard() override;

	QtnPropertyBase *getActiveProperty() const;

	static void addShortcutForAction(const QKeySequence &seq, QAction *action,
		QWidget *parent,
		Qt::ShortcutContext shortcutContext = Qt::WidgetWithChildrenShortcut);

private:
	void onMouseReleased();
	void onResetTriggered();
	void onLockToggleTriggered();

public slots:
	void deleteActiveProperty();
	void cutToClipboard();
	void copyToClipboard();
	void pasteFromClipboard();

protected:
	virtual void contextMenuEvent(QContextMenuEvent *event) override;

	virtual bool dataHasSupportedFormats(const QMimeData *data) override;
	virtual void deleteProperty(QtnPropertyBase *property) override;

	virtual QMimeData *getPropertyDataForAction(
		QtnPropertyBase *property, Qt::DropAction dropAction) override;

	virtual bool applyPropertyData(const QMimeData *data,
		QtnPropertyBase *destination, QtnApplyPosition position) override;

	virtual bool eventFilter(QObject *obj, QEvent *event) override;

	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dragMoveEvent(QDragMoveEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;
	virtual bool drop(const QMimeData *data, QtnPropertyBase *property,
		QtnApplyPosition applyPosition);
	virtual void dropEnd();

private:
	void onDropFinished(Qt::DropAction action);
	bool dragAndDrop();
	void internalConnect(
		QAction *dropAction, void (QtnPropertyWidgetEx::*slot)(), bool connect);

	QPoint dragStartPos;
	QtnPropertyBase *draggedProperty;
	QDrag *mDrag;
	Qt::DropAction dropAction;
	bool canRemove;
};
