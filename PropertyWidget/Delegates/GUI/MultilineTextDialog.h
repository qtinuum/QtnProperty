#pragma once

#include <QDialog>

namespace Ui
{
	class MultilineTextDialog;
}

class MultilineTextDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MultilineTextDialog(QWidget *parent = nullptr);
	virtual ~MultilineTextDialog();

	void setText(const QString &text);
	QString getText() const;

private:
	Ui::MultilineTextDialog *ui;
};
