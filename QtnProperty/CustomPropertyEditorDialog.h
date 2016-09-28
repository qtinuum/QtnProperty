#pragma once

#include "CustomPropertyOptionsDialog.h"
#include "PropertyWidgetEx.h"

#include <QDialog>
#include <QVariant>

#include <functional>

namespace Ui
{
	class CustomPropertyEditorDialog;
}

class QAbstractButton;
class QtnProperty;
class QtnPropertyBase;
class QtnPropertySet;
class VarProperty;

class QTN_IMPORT_EXPORT CustomPropertyEditorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CustomPropertyEditorDialog(QWidget *parent = nullptr);
	virtual ~CustomPropertyEditorDialog();

	bool execute(const QString &title, QVariant &data);

	void setReadOnly(bool value);

	virtual void accept() override;
	virtual void reject() override;

signals:
	void apply(const QVariant &data);

private slots:
	void onActivePropertyChanged(QtnPropertyBase* activeProperty);

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_propertyWidget_customContextMenuRequested(const QPoint &pos);

	void on_actionPropertyAdd_triggered();

	void on_actionPropertyDuplicate_triggered();

	void on_actionPropertyOptions_triggered();

private:
	void updateActions(QtnPropertyBase *property = nullptr);
	void updateTitle();

	void addShortcutForAction(const QKeySequence &key_seq, QAction *action);

	Ui::CustomPropertyEditorDialog *ui;
};
