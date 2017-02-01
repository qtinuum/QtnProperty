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

#pragma once

#include "CoreAPI.h"

#include <QDialog>
#include <QVariant>

#include <functional>

class QLabel;
class QLineEdit;
class QSpinBox;
class QDialogButtonBox;
class QAbstractButton;

typedef std::function<bool (const QString &)> IsNameAvailableCB;

class QTN_IMPORT_EXPORT BasePropertyDialog : public QDialog
{
	Q_OBJECT

public:
	explicit BasePropertyDialog(QWidget *parent = nullptr);

	void initWithCount(
		int actual_index, int existing_count,
		bool readonly = false);
	void initWithName(
		const QString &actual_name,
		const IsNameAvailableCB &is_name_available,
		bool readonly = false);

private slots:
	void on_buttonBox_clicked(QAbstractButton *button);

protected:
	bool execute();

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

struct QTN_IMPORT_EXPORT CustomPropertyData
{
	int index;
	QString name;
	QVariant value;
};

class QTN_IMPORT_EXPORT CustomPropertyOptionsDialog : public BasePropertyDialog
{
	Q_OBJECT

public:
	explicit CustomPropertyOptionsDialog(QWidget *parent = nullptr);
	virtual ~CustomPropertyOptionsDialog();

	void executeReadOnly();
	bool execute(CustomPropertyData &result);

	void setType(QVariant::Type type);
	void setTypeBoxEnabled(bool value);

	void setReadOnly(bool readOnly);

protected:
	virtual bool ValidateInput() override;
	virtual QLabel *GetLabel() override;
	virtual QLineEdit *GetNameEdit() override;
	virtual QSpinBox *GetIndexEdit() override;
	virtual QDialogButtonBox *GetButtonBox() override;

	QVariant value_type;
	Ui::CustomPropertyOptionsDialog *ui;
};
