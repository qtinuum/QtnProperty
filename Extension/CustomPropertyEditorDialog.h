#pragma once

#include "CustomPropertyOptionsDialog.h"

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

class CustomPropertyEditorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CustomPropertyEditorDialog(QWidget *parent = 0);
	virtual ~CustomPropertyEditorDialog();

	bool execute(const QString &title, const QVariant &data);

	QVariant getData() const;

private slots:
	void onPropertyValueAccept(const QtnProperty *property, void *valueToAccept, bool *accept);

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_propertyWidget_customContextMenuRequested(const QPoint &pos);

	void on_actionPropertyAdd_triggered();

	void on_actionPropertyRemove_triggered();

	void on_actionPropertyDuplicate_triggered();

	void on_actionPropertyOptions_triggered();

private:
	bool getActiveVarProperty(QtnPropertyBase *&property, VarProperty *&var_property);
	static VarProperty *getVarProperty(QtnPropertyBase *source);
	QtnPropertyBase *newProperty(QtnPropertySet *parent,
								 const QVariant &value,
								 const QString &key,
								 int index,
								 VarProperty *map_parent);


	void addProperty(QtnPropertyBase *source, const QVariant &value,
					 const QString &name, int index);
	void duplicateProperty(QtnPropertyBase *source, const QString &name, int index);
	void propertyOptions(QtnPropertyBase *source, const QString &name, int index);

	typedef std::function<void (VarProperty *, const QString &, int)> PropertyActionCB;

	void propertyAction(QtnPropertyBase *property, const QString &dialog_title,
						const IsNameAvailableCB &is_name_available,
						const PropertyActionCB &action);

	Ui::CustomPropertyEditorDialog *ui;
};
