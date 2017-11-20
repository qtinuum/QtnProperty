/*******************************************************************************
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#pragma once

#include "CustomPropertyOptionsDialog.h"
#include "PropertyWidgetEx.h"

#include <QDialog>
#include <QVariant>

#include <functional>

namespace Ui
{
class CustomPropertyEditorDialog;
}

class QAbstractButton;
class QtnProperty;
class QtnPropertyBase;
class QtnPropertySet;
class VarProperty;

class QTN_IMPORT_EXPORT CustomPropertyEditorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CustomPropertyEditorDialog(QWidget *parent = nullptr);
	virtual ~CustomPropertyEditorDialog();

	bool execute(const QString &title, QVariant &data);

	void setReadOnly(bool value);

	virtual void accept() override;
	virtual void reject() override;

signals:
	void apply(const QVariant &data);

private slots:
	void onActivePropertyChanged(QtnPropertyBase *activeProperty);

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_propertyWidget_customContextMenuRequested(const QPoint &pos);

	void on_actionPropertyAdd_triggered();

	void on_actionPropertyDuplicate_triggered();

	void on_actionPropertyOptions_triggered();

private:
	void updateActions(QtnPropertyBase *property = nullptr);
	void updateTitle();

	Ui::CustomPropertyEditorDialog *ui;
};
