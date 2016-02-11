#pragma once

#include <QDialog>
#include <QVariant>

#include <functional>

class QLabel;
class QLineEdit;
class QSpinBox;
class QDialogButtonBox;
class QAbstractButton;

typedef std::function<bool (const QString &)> IsNameAvailableCB;

class BasePropertyDialog : public QDialog
{
	Q_OBJECT

public:
	explicit BasePropertyDialog(QWidget *parent = nullptr);

private slots:
	void on_buttonBox_clicked(QAbstractButton *button);

protected:
	bool execute();
	void initWithCount(int actual_index, int existing_count);
	void initWithName(const QString &actual_name, const IsNameAvailableCB &is_name_available);

	virtual bool ValidateInput();
	virtual QLabel *GetLabel() = 0;
	virtual QLineEdit *GetNameEdit() = 0;
	virtual QSpinBox *GetIndexEdit() = 0;
	virtual QDialogButtonBox *GetButtonBox() = 0;

	IsNameAvailableCB is_name_available;
	int result_index;
	QString result_name;

	QString index_label;
	QString name_label;
};

namespace Ui
{
	class CustomPropertyOptionsDialog;
}

class CustomPropertyOptionsDialog : public BasePropertyDialog
{
	Q_OBJECT

public:
	explicit CustomPropertyOptionsDialog(QWidget *parent = nullptr);
	virtual ~CustomPropertyOptionsDialog();

	struct Result
	{
		int index;
		QString name;
		QVariant value;
	};

	static bool execute(QWidget *parent, const QString &title, Result &result, int existing_count, int index = -1);
	static bool execute(QWidget *parent, const QString &title, Result &result, const QString &default_name, const IsNameAvailableCB &is_name_available);

	bool execute(Result &result);

protected:
	virtual bool ValidateInput() override;
	virtual QLabel *GetLabel() override;
	virtual QLineEdit *GetNameEdit() override;
	virtual QSpinBox *GetIndexEdit() override;
	virtual QDialogButtonBox *GetButtonBox() override;

	QVariant value_type;
	Ui::CustomPropertyOptionsDialog *ui;
};
