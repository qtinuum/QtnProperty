/*
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "MultilineTextDialog.h"
#include "ui_MultilineTextDialog.h"

#include <QAbstractButton>

MultilineTextDialog::MultilineTextDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::MultilineTextDialog)
{
	ui->setupUi(this);

	setWindowFlags((windowFlags() & ~(Qt::WindowContextHelpButtonHint))
				   | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);
}

MultilineTextDialog::~MultilineTextDialog()
{
	delete ui;
}

void MultilineTextDialog::setReadOnly(bool value)
{
	ui->plainTextEdit->setReadOnly(value);

	if (value)
	{
		ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
	} else
	{
		ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	}
}

void MultilineTextDialog::setText(const QString &text)
{
	ui->plainTextEdit->setPlainText(text);
}

QString MultilineTextDialog::getText() const
{
	return ui->plainTextEdit->toPlainText();
}

void MultilineTextDialog::on_buttonBox_clicked(QAbstractButton *button)
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
