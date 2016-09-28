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
