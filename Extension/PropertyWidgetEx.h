#pragma once

#include "QtnProperty/Widget/PropertyWidget.h"

class QMimeData;

const QString *pTextPlain;

enum class QtnApplyPosition
{
	Before,
	Over,
	After
};

struct QtnPropertyWidgetExDelegate
{
	virtual ~QtnPropertyWidgetExDelegate() {}

	virtual bool canRemoveProperty(QtnPropertyBase *property) = 0;
	virtual bool canCutToClipboard() = 0;
	virtual bool canCopyToClipboard() = 0;
	virtual bool canPasteFromClipboard() = 0;
	virtual bool dataHasSupportedFormats(const QMimeData *data) = 0;
	virtual void removeProperty(QtnPropertyBase *property) = 0;
	virtual QMimeData *getPropertyDataForAction(QtnPropertyBase *property,
												Qt::DropAction action) = 0;
	virtual bool applyPropertyData(const QMimeData *data,
								   QtnPropertyBase *destination,
								   QtnApplyPosition position) = 0;
};

class QtnPropertyWidgetEx
	: public QtnPropertyWidget
	, public QtnPropertyWidgetExDelegate
{
	Q_OBJECT

public:
	explicit QtnPropertyWidgetEx(QWidget *parent = nullptr);

	void connectRemoveAction(QAction *action, bool connect);
	void connectCutAction(QAction *action, bool connect);
	void connectCopyAction(QAction *action, bool connect);
	void connectPasteAction(QAction *action, bool connect);

	virtual bool canRemoveProperty(QtnPropertyBase *property) override;
	virtual bool canCutToClipboard() override;
	virtual bool canCopyToClipboard() override;
	virtual bool canPasteFromClipboard() override;	

public slots:
	void removeActiveProperty();
	void cutToClipboard();
	void copyToClipboard();
	void pasteFromClipboard();

protected:
	virtual bool dataHasSupportedFormats(const QMimeData *data) override;
	virtual void removeProperty(QtnPropertyBase *property) override;
	virtual QMimeData *getPropertyDataForAction(QtnPropertyBase *property,
												Qt::DropAction action) override;
	virtual bool applyPropertyData(const QMimeData *data,
								   QtnPropertyBase *destination,
								   QtnApplyPosition position) override;

	virtual bool eventFilter(QObject *obj, QEvent *event) override;

	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dragMoveEvent(QDragMoveEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;

private:
	bool dragAndDrop();
	void internalConnect(QAction *action,
						 void (QtnPropertyWidgetEx::*slot)(),
						 bool connect);

	QPoint drag_start_pos;
	QtnPropertyBase *dragged_property;
	bool can_remove;
};
