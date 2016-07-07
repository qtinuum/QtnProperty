#include "CustomPropertyEditorDialog.h"
#include "ui_CustomPropertyEditorDialog.h"

#include "QtnProperty/Widget/Utils/InplaceEditing.h"

#include "VarProperty.h"

#include <QMenu>
#include <QShortcut>

CustomPropertyEditorDialog::CustomPropertyEditorDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::CustomPropertyEditorDialog)
{
	ui->setupUi(this);

	updateTitle();

	setWindowFlags((windowFlags() & ~(Qt::WindowContextHelpButtonHint))
				   | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);

	QObject::connect(ui->propertyWidget->propertyView(), &QtnPropertyView::activePropertyChanged,
					 this, &CustomPropertyEditorDialog::onActivePropertyChanged);

	addShortcutForAction(ui->actionPropertyOptions->shortcut(), ui->actionPropertyOptions);
#ifdef Q_OS_MAC
	addShortcutForAction(QKeySequence(Qt::CTRL | Qt::Key_Backspace), ui->actionPropertyDelete);
#else
	addShortcutForAction(ui->actionPropertyDelete->shortcut(), ui->actionPropertyDelete);
#endif
	addShortcutForAction(QKeySequence::Cut, ui->actionPropertyCut);
	addShortcutForAction(QKeySequence::Copy, ui->actionPropertyCopy);
	addShortcutForAction(QKeySequence::Paste, ui->actionPropertyPaste);

	addShortcutForAction(ui->actionPropertyAdd->shortcut(), ui->actionPropertyAdd);

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
		ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
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

void CustomPropertyEditorDialog::onActivePropertyChanged(QtnPropertyBase *activeProperty)
{
	updateActions(activeProperty);
}

void CustomPropertyEditorDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	switch (ui->buttonBox->buttonRole(button))
	{
		case QDialogButtonBox::ApplyRole:
		{
			auto data_ptr = ui->propertyWidget->getData();
			if (nullptr != data_ptr)
			{
				ui->propertyWidget->updateData();
				emit apply(*data_ptr);
			}
		}	break;

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

void CustomPropertyEditorDialog::on_propertyWidget_customContextMenuRequested(const QPoint &pos)
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

void CustomPropertyEditorDialog::addShortcutForAction(const QKeySequence &key_seq, QAction *action)
{
	auto shortcut = new QShortcut(key_seq, this);
	QObject::connect(shortcut, &QShortcut::activated,
					 action, &QAction::trigger);
	if (QKeySequence::ExactMatch != action->shortcut().matches(key_seq))
	{
		shortcut = new QShortcut(action->shortcut(), this);
		QObject::connect(shortcut, &QShortcut::activated,
						 action, &QAction::trigger);
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
		ui->actionPropertyAdd->setEnabled(property->id() == VarProperty::PID_EXTRA);
		ui->actionPropertyDuplicate->setEnabled(not_top_parent);
		ui->actionPropertyOptions->setEnabled(true);
		ui->actionPropertyDelete->setEnabled(not_top_parent && widget->canDeleteProperty(property));
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
	setWindowTitle(ui->propertyWidget->isReadOnly() ? tr("Read-only Properties") : tr("Edit Custom Properties"));
}
