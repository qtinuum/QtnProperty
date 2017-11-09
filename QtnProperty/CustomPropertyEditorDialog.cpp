/*******************************************************************************
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "CustomPropertyEditorDialog.h"
#include "ui_CustomPropertyEditorDialog.h"

#include "Utils/InplaceEditing.h"

#include "VarProperty.h"

#include <QMenu>
#include <QShortcut>

CustomPropertyEditorDialog::CustomPropertyEditorDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::CustomPropertyEditorDialog)
{
	ui->setupUi(this);

	updateTitle();

	setWindowFlags((windowFlags() & ~(Qt::WindowContextHelpButtonHint)) |
		Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);

	QObject::connect(ui->propertyWidget->propertyView(),
		&QtnPropertyView::activePropertyChanged, this,
		&CustomPropertyEditorDialog::onActivePropertyChanged);

#ifdef Q_OS_MAC
	QtnPropertyWidgetEx::addShortcutForAction(
		QKeySequence(Qt::Key_Backspace), ui->actionPropertyDelete, this);
#endif
	QtnPropertyWidgetEx::addShortcutForAction(
		QKeySequence(Qt::Key_Delete), ui->actionPropertyDelete, this);

#ifdef Q_OS_WIN
	QtnPropertyWidgetEx::addShortcutForAction(
		QKeySequence(Qt::CTRL | Qt::Key_Insert), ui->actionPropertyCopy, this);
	QtnPropertyWidgetEx::addShortcutForAction(
		QKeySequence(Qt::SHIFT | Qt::Key_Insert), ui->actionPropertyPaste,
		this);
#endif

	QtnPropertyWidgetEx::addShortcutForAction(
		ui->actionPropertyCut->shortcut(), ui->actionPropertyCut, this);
	QtnPropertyWidgetEx::addShortcutForAction(
		ui->actionPropertyCopy->shortcut(), ui->actionPropertyCopy, this);
	QtnPropertyWidgetEx::addShortcutForAction(
		ui->actionPropertyPaste->shortcut(), ui->actionPropertyPaste, this);

	QtnPropertyWidgetEx::addShortcutForAction(
		ui->actionPropertyOptions->shortcut(), ui->actionPropertyOptions, this);
	QtnPropertyWidgetEx::addShortcutForAction(
		ui->actionPropertyAdd->shortcut(), ui->actionPropertyAdd, this);

	ui->propertyWidget->connectDeleteAction(ui->actionPropertyDelete, true);
	ui->propertyWidget->connectCutAction(ui->actionPropertyCut, true);
	ui->propertyWidget->connectCopyAction(ui->actionPropertyCopy, true);
	ui->propertyWidget->connectPasteAction(ui->actionPropertyPaste, true);
}

CustomPropertyEditorDialog::~CustomPropertyEditorDialog()
{
	delete ui;
}

bool CustomPropertyEditorDialog::execute(const QString &title, QVariant &data)
{
	ui->propertyWidget->setData(&data, title);

	updateActions();

	show();
	raise();
	exec();

	if (result() == Accepted)
	{
		ui->propertyWidget->updateData();
		return true;
	}

	return false;
}

void CustomPropertyEditorDialog::setReadOnly(bool value)
{
	ui->propertyWidget->setReadOnly(value);
	updateTitle();

	if (value)
	{
		ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
	} else
	{
		ui->buttonBox->setStandardButtons(
			QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	}
}

void CustomPropertyEditorDialog::accept()
{
	qtnStopInplaceEdit(false);

	QDialog::accept();
}

void CustomPropertyEditorDialog::reject()
{
	qtnStopInplaceEdit(false);

	QDialog::reject();
}

void CustomPropertyEditorDialog::onActivePropertyChanged(
	QtnPropertyBase *activeProperty)
{
	updateActions(activeProperty);
}

void CustomPropertyEditorDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	switch (ui->buttonBox->buttonRole(button))
	{
		case QDialogButtonBox::AcceptRole:
			accept();
			break;

		case QDialogButtonBox::RejectRole:
			reject();
			break;

		default:
			break;
	}
}

void CustomPropertyEditorDialog::on_propertyWidget_customContextMenuRequested(
	const QPoint &pos)
{
	auto property = ui->propertyWidget->propertyView()->activeProperty();

	if (nullptr != property)
	{
		qtnStopInplaceEdit(false);

		QMenu menu(this);

		menu.addAction(ui->actionPropertyOptions);
		menu.addSeparator();

		menu.addAction(ui->actionPropertyAdd);
		menu.addAction(ui->actionPropertyDuplicate);
		menu.addSeparator();
		menu.addAction(ui->actionPropertyCut);
		menu.addAction(ui->actionPropertyCopy);
		menu.addAction(ui->actionPropertyPaste);
		menu.addSeparator();
		menu.addAction(ui->actionPropertyDelete);

		updateActions(property);
		menu.exec(ui->propertyWidget->mapToGlobal(pos));
	}
}

void CustomPropertyEditorDialog::on_actionPropertyAdd_triggered()
{
	ui->propertyWidget->addProperty();
}

void CustomPropertyEditorDialog::on_actionPropertyDuplicate_triggered()
{
	ui->propertyWidget->duplicateProperty();
}

void CustomPropertyEditorDialog::on_actionPropertyOptions_triggered()
{
	ui->propertyWidget->propertyOptions();
}

void CustomPropertyEditorDialog::updateActions(QtnPropertyBase *property)
{
	auto widget = ui->propertyWidget;
	bool readOnly = widget->isReadOnly();

	if (nullptr == property)
		property = ui->propertyWidget->propertyView()->activeProperty();

	auto add_text = tr("New...");

	auto var_property = CustomPropertyWidget::getVarProperty(property);

	if (nullptr != var_property)
	{
		switch (var_property->GetType())
		{
			case VarProperty::Map:
				add_text = tr("New Property...");
				break;

			case VarProperty::List:
				add_text = tr("New Element...");
				break;

			default:
				break;
		}

		bool not_top_parent = (var_property != var_property->TopParent());
		ui->actionPropertyAdd->setEnabled(
			!readOnly && property->id() == VarProperty::PID_EXTRA);
		ui->actionPropertyDuplicate->setEnabled(!readOnly && not_top_parent);
		ui->actionPropertyOptions->setEnabled(true);
		ui->actionPropertyDelete->setEnabled(
			!readOnly && not_top_parent && widget->canDeleteProperty(property));
	} else
	{
		ui->actionPropertyAdd->setEnabled(false);
		ui->actionPropertyDuplicate->setEnabled(false);
		ui->actionPropertyOptions->setEnabled(false);
		ui->actionPropertyDelete->setEnabled(false);
	}

	ui->actionPropertyAdd->setText(add_text);

	ui->actionPropertyCut->setEnabled(widget->canCutToClipboard());
	ui->actionPropertyCopy->setEnabled(widget->canCopyToClipboard());
	ui->actionPropertyPaste->setEnabled(widget->canPasteFromClipboard());
}

void CustomPropertyEditorDialog::updateTitle()
{
	setWindowTitle(ui->propertyWidget->isReadOnly()
			? tr("Read-only Properties")
			: tr("Edit Custom Properties"));
}
