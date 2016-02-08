#include "MultilineTextDialog.h"
#include "ui_MultilineTextDialog.h"

MultilineTextDialog::MultilineTextDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::MultilineTextDialog)
{
	ui->setupUi(this);
}

MultilineTextDialog::~MultilineTextDialog()
{
	delete ui;
}

void MultilineTextDialog::setText(const QString &text)
{
	ui->plainTextEdit->setPlainText(text);
}

QString MultilineTextDialog::getText() const
{
	return ui->plainTextEdit->toPlainText();
}
