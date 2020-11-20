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
	setWindowFlags(
		(windowFlags() &
			~(Qt::WindowContextHelpButtonHint | Qt::WindowMinMaxButtonsHint)) |
		Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint);
}

void BasePropertyDialog::initWithCount(
	int actual_index, int existing_count, bool readonly)
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
	index_edit->setReadOnly(readonly);
}

void BasePropertyDialog::initWithName(const QString &actual_name,
	const IsNameAvailableCB &is_name_available, bool readonly)
{
	result_index = -1;
	result_name = "";

	this->is_name_available = is_name_available;

	GetLabel()->setText(name_label);
	GetIndexEdit()->setVisible(false);
	auto name_edit = GetNameEdit();
	name_edit->setVisible(true);
	name_edit->setText(actual_name);
	name_edit->setReadOnly(readonly);
}

bool BasePropertyDialog::execute()
{
	show();
	raise();

	exec();

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
			QMessageBox::critical(this, QApplication::applicationName(),
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

			break;
		}

		case QDialogButtonBox::RejectRole:
		{
			reject();
			break;
		}

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

void CustomPropertyOptionsDialog::executeReadOnly()
{
	setReadOnly(true);
	BasePropertyDialog::execute();
}

bool CustomPropertyOptionsDialog::execute(QtnCustomPropertyData &result)
{
	setReadOnly(false);

	if (BasePropertyDialog::execute())
	{
		result.index = result_index;
		result.name = result_name;
		result.value = value_type;

		return true;
	}

	return false;
}

void CustomPropertyOptionsDialog::setType(QVariant::Type type)
{
	switch (type)
	{
		case QVariant::List:
			ui->rbList->setChecked(true);
			break;

		case QVariant::Map:
			ui->rbDictionary->setChecked(true);
			break;

		case QVariant::Bool:
			ui->rbBoolean->setChecked(true);
			break;

		case QVariant::Int:
		case QVariant::UInt:
		case QVariant::LongLong:
		case QVariant::ULongLong:
		case QVariant::Double:
			ui->rbNumeric->setChecked(true);
			break;

		case QVariant::String:
		case QVariant::Char:
		case QVariant::Color:
			ui->rbString->setChecked(true);
			break;

		default:
			ui->rbNull->setChecked(true);
			break;
	}
}

void CustomPropertyOptionsDialog::setTypeBoxEnabled(bool value)
{
	ui->typeBox->setEnabled(value);
}

void CustomPropertyOptionsDialog::setReadOnly(bool readOnly)
{
	setTypeBoxEnabled(!readOnly);
	GetLabel()->setEnabled(!readOnly);
	GetNameEdit()->setEnabled(!readOnly);
	GetIndexEdit()->setEnabled(!readOnly);

	GetButtonBox()->setStandardButtons(readOnly
			? QDialogButtonBox::Close
			: QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
}

bool CustomPropertyOptionsDialog::ValidateInput()
{
	if (BasePropertyDialog::ValidateInput())
	{
		if (ui->rbNull->isChecked())
			value_type.clear();
		else if (ui->rbBoolean->isChecked())
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

QString QtnCustomPropertyData::displayName() const
{
	if (index >= 0)
		return nameForIndex(index);

	return name;
}

QString QtnCustomPropertyData::nameForIndex(int index)
{
	return QStringLiteral("[%1]").arg(index);
}
