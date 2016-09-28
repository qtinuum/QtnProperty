#pragma once

#include "PropertyWidget.h"

class QMimeData;

enum class QtnApplyPosition
{
	Before,
	Over,
	After
};

struct QTN_IMPORT_EXPORT QtnPropertyWidgetExDelegate
{
	virtual ~QtnPropertyWidgetExDelegate() {}

	virtual bool canDeleteProperty(QtnPropertyBase *property) = 0;
	virtual bool canCutToClipboard() = 0;
	virtual bool canCopyToClipboard() = 0;
	virtual bool canPasteFromClipboard() = 0;
	virtual bool dataHasSupportedFormats(const QMimeData *data) = 0;
	virtual void deleteProperty(QtnPropertyBase *property) = 0;
	virtual QMimeData *getPropertyDataForAction(QtnPropertyBase *property,
												Qt::DropAction action) = 0;
	virtual bool applyPropertyData(const QMimeData *data,
								   QtnPropertyBase *destination,
								   QtnApplyPosition position) = 0;
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

private slots:
	void onMouseReleased();

public slots:
	void deleteActiveProperty();
	void cutToClipboard();
	void copyToClipboard();
	void pasteFromClipboard();

protected:
	virtual bool dataHasSupportedFormats(const QMimeData *data) override;
	virtual void deleteProperty(QtnPropertyBase *property) override;
	virtual QMimeData *getPropertyDataForAction(QtnPropertyBase *property,
												Qt::DropAction dropAction) override;
	virtual bool applyPropertyData(const QMimeData *data,
								   QtnPropertyBase *destination,
								   QtnApplyPosition position) override;

	virtual bool eventFilter(QObject *obj, QEvent *event) override;

	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dragMoveEvent(QDragMoveEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;

private:
	bool dragAndDrop();
	void internalConnect(QAction *dropAction,
						 void (QtnPropertyWidgetEx::*slot)(),
						 bool connect);

	QPoint dragStartPos;
	QtnPropertyBase *draggedProperty;
	Qt::DropAction dropAction;
	bool canRemove;
};
