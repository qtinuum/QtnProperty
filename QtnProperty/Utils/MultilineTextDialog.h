#pragma once

#include "QtnProperty/CoreAPI.h"

#include <QDialog>

namespace Ui
{
	class MultilineTextDialog;
}

class QAbstractButton;

class QTN_IMPORT_EXPORT MultilineTextDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MultilineTextDialog(QWidget *parent = nullptr);
	virtual ~MultilineTextDialog();

	void setReadOnly(bool value);

	void setText(const QString &text);
	QString getText() const;

private slots:
	void on_buttonBox_clicked(QAbstractButton *button);

private:
	Ui::MultilineTextDialog *ui;
};
