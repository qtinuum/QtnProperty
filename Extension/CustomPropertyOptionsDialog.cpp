#include "CustomPropertyOptionsDialog.h"
#include "ui_CustomPropertyOptionsDialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QMessageBox>

BasePropertyDialog::BasePropertyDialog(QWidget *parent)
		: QDialog(parent)
		, result_index(-1)
{
	setWindowFlags((windowFlags() & ~(Qt::WindowContextHelpButtonHint | Qt::WindowMinMaxButtonsHint))
				|	Qt::MSWindowsFixedSizeDialogHint
				|	Qt::CustomizeWindowHint);
}

void BasePropertyDialog::initWithCount(int actual_index, int existing_count)
{
	if (actual_index < 0)
		actual_index = existing_count;
	result_index = -1;
	result_name = "";

	GetLabel()->setText(index_label);
	GetNameEdit()->setVisible(false);
	auto index_edit = GetIndexEdit();
	index_edit->setVisible(true);
	index_edit->setMinimum(0);
	index_edit->setMaximum(existing_count);
	index_edit->setValue(actual_index);
}

void BasePropertyDialog::initWithName(const QString &actual_name, const IsNameAvailableCB &is_name_available)
{
	result_index = -1;
	result_name = "";

	this->is_name_available = is_name_available;

	GetLabel()->setText(name_label);
	GetIndexEdit()->setVisible(false);
	auto name_edit = GetNameEdit();
	name_edit->setVisible(true);
	name_edit->setText(actual_name);
}

bool BasePropertyDialog::execute()
{
	auto connection = QObject::connect(GetButtonBox(), &QDialogButtonBox::clicked,
					 this, &BasePropertyDialog::on_buttonBox_clicked);

	show();
	raise();

	exec();

	QObject::disconnect(connection);

	return result() == DialogCode::Accepted;
}

bool BasePropertyDialog::ValidateInput()
{
	auto name_edit = GetNameEdit();
	if (name_edit->isVisible())
	{
		auto name = name_edit->text();
		if (nullptr == is_name_available || is_name_available(name))
		{
			result_name = name;
		} else
		{
			QMessageBox::critical(this, QCoreApplication::applicationName(),
								  tr("Property with name '%1' is already exist.").arg(name));
			return false;
		}
	} else
	{
		result_index = GetIndexEdit()->value();
	}

	return true;
}

void BasePropertyDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	switch (GetButtonBox()->buttonRole(button))
	{
		case QDialogButtonBox::AcceptRole:
		{
			if (ValidateInput())
				accept();
		}	break;

		case QDialogButtonBox::RejectRole:
		{
			reject();
		}	break;

		default:
			break;
	}
}

CustomPropertyOptionsDialog::CustomPropertyOptionsDialog(QWidget *parent)
		: BasePropertyDialog(parent)
		, ui(new Ui::CustomPropertyOptionsDialog)
{
	ui->setupUi(this);

	index_label = tr("Index:");
	name_label = tr("Name:");
}

CustomPropertyOptionsDialog::~CustomPropertyOptionsDialog()
{
	delete ui;
}


bool CustomPropertyOptionsDialog::execute(QWidget *parent, const QString &title,
										  Result &result, int existing_count, int index)
{
	CustomPropertyOptionsDialog dialog(parent);

	dialog.setWindowTitle(title);

	dialog.initWithCount(index, existing_count);

	return dialog.execute(result);
}

bool CustomPropertyOptionsDialog::execute(QWidget *parent, const QString &title,
										  Result &result, const QString &default_name,
										  const IsNameAvailableCB &is_name_available)
{
	CustomPropertyOptionsDialog dialog(parent);

	dialog.setWindowTitle(title);

	dialog.initWithName(default_name, is_name_available);

	return dialog.execute(result);
}

bool CustomPropertyOptionsDialog::execute(Result &result)
{
	if (BasePropertyDialog::execute())
	{
		result.index = result_index;
		result.name = result_name;
		result.value = value_type;

		return true;
	}

	return false;
}

bool CustomPropertyOptionsDialog::ValidateInput()
{
	if (BasePropertyDialog::ValidateInput())
	{
		if (ui->rbBoolean->isChecked())
			value_type = false;
		else if (ui->rbNumeric->isChecked())
			value_type = 0.0;
		else if (ui->rbString->isChecked())
			value_type = "";
		else if (ui->rbDictionary->isChecked())
			value_type = QVariantMap();
		else if (ui->rbList->isChecked())
			value_type = QVariantList();

		return true;
	}

	return false;
}

QLabel *CustomPropertyOptionsDialog::GetLabel()
{
	return ui->label;
}

QLineEdit *CustomPropertyOptionsDialog::GetNameEdit()
{
	return ui->editName;
}

QSpinBox *CustomPropertyOptionsDialog::GetIndexEdit()
{
	return ui->editIndex;
}

QDialogButtonBox *CustomPropertyOptionsDialog::GetButtonBox()
{
	return ui->buttonBox;
}
