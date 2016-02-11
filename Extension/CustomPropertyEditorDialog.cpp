#include "CustomPropertyEditorDialog.h"
#include "ui_CustomPropertyEditorDialog.h"

#include "VarProperty.h"

#include <QAbstractButton>
#include <QMenu>

CustomPropertyEditorDialog::CustomPropertyEditorDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::CustomPropertyEditorDialog)
{
	ui->setupUi(this);

	setWindowFlags((windowFlags() & ~(Qt::WindowContextHelpButtonHint | Qt::WindowMinMaxButtonsHint)));
}

CustomPropertyEditorDialog::~CustomPropertyEditorDialog()
{
	delete ui;
}

bool CustomPropertyEditorDialog::execute(const QString &title, const QVariant &data)
{
	auto widget = ui->propertyWidget;

	auto property_set = new QtnPropertySet(widget);

	newProperty(property_set, data, title, -1, nullptr);

	widget->setPropertySet(property_set);

	show();
	raise();
	exec();

	widget->setPropertySet(nullptr);
	delete property_set;

	return (result() == Accepted);
}

void CustomPropertyEditorDialog::onPropertyValueAccept(const QtnProperty *property, void *valueToAccept, bool *accept)
{
	if (nullptr != accept)
	{
		bool ok = VarProperty::PropertyValueAccept(property, valueToAccept, data);
		Q_ASSERT(ok);

		*accept = ok;
	}
}

void CustomPropertyEditorDialog::on_buttonBox_clicked(QAbstractButton *button)
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

void CustomPropertyEditorDialog::on_propertyWidget_customContextMenuRequested(const QPoint &pos)
{
	QtnPropertyBase *property;
	VarProperty *var_property;

	if (getActiveVarProperty(property, var_property))
	{
		auto menu = new QMenu(this);

		menu->addAction(ui->actionPropertyAdd);
		menu->addAction(ui->actionPropertyDuplicate);
		menu->addAction(ui->actionPropertyOptions);
		menu->addAction(ui->actionPropertyRemove);

		bool not_top_parent = (var_property != var_property->TopParent());
		ui->actionPropertyAdd->setEnabled(property->id() == VarProperty::PID_EXTRA);
		ui->actionPropertyRemove->setEnabled(not_top_parent);
		ui->actionPropertyDuplicate->setEnabled(not_top_parent);
		ui->actionPropertyOptions->setEnabled(not_top_parent);

		menu->exec(ui->propertyWidget->mapToGlobal(pos));
	}
}

bool CustomPropertyEditorDialog::getActiveVarProperty(QtnPropertyBase *&property, VarProperty *&var_property)
{
	property = ui->propertyWidget->propertyView()->activeProperty();

	var_property = getVarProperty(property);

	return var_property != nullptr;
}

VarProperty *CustomPropertyEditorDialog::getVarProperty(QtnPropertyBase *source)
{
	auto var_property = nullptr;
	if (nullptr != source)
		var_property = source->findChild<VarProperty *>(QString(), Qt::FindDirectChildrenOnly);

	return var_property;
}

QtnPropertyBase *CustomPropertyEditorDialog::newProperty(QtnPropertySet *parent,
														 const QVariant &value,
														 const QString &key,
														 int index,
														 VarProperty *map_parent)
{
	return VarProperty::NewExtraProperty(parent, value, key, index, map_parent,
	[this](QtnProperty *property)
	{
		QObject::connect(property, &QtnProperty::propertyValueAccept,
						 this, &CustomPropertyEditorDialog::onPropertyValueAccept);
	});
}

void CustomPropertyEditorDialog::addProperty(QtnPropertyBase *source, const QVariant &value,
											 const QString &name, int index)
{
	auto var_property = getVarProperty(source);
	auto set = dynamic_cast<QtnPropertySet *>(source);
	Q_ASSERT(nullptr != set);
	auto new_property = newProperty(nullptr, value, name, index, var_property);

	std::vector<QtnPropertyBase *> children;
	for(auto child : set->childProperties())
	{
		children.push_back(child);
	}

	children.push_back(new_property);

	std::sort(children.begin(), children.end(),
	[](QtnPropertyBase *a, QtnPropertyBase *b) -> bool
	{
		return a->name() < b->name();
	});

	auto it = std::find(children.begin(), children.end(), new_property);

	set->addChildProperty(new_property, true, it - children.begin());
	ui->propertyWidget->setActiveProperty(new_property);
}

void CustomPropertyEditorDialog::duplicateProperty(QtnPropertyBase *source,
												   const QString &name, int index)
{

}

void CustomPropertyEditorDialog::propertyOptions(QtnPropertyBase *source,
												 const QString &name, int index)
{

}

void CustomPropertyEditorDialog::propertyAction(QtnPropertyBase *property,
												const QString &dialog_title,
												const IsNameAvailableCB &is_name_available,
												const PropertyActionCB &action)
{
	Q_ASSERT(nullptr != property);
	auto var_property = getVarProperty(property);
	Q_ASSERT(nullptr != var_property);

	CustomPropertyOptionsDialog::Result result;
	auto var_parent = var_property->VarParent();

	CustomPropertyOptionsDialog dialog(this);

	dialog.setWindowTitle(dialog_title);

	switch (var_parent->GetType())
	{
		case VarProperty::List:
		{
			//TODO
			dialog.initWithCount(var_property->GetIndex(),
								 static_cast<int>(var_parent->VarChildren().size());

			if (CustomPropertyOptionsDialog::execute(result))
			{
				action(property, "", result.index);
			}
		} break;

		case VarProperty::Map:
		{
			dialog.initWithName(var_property->GetName(),
								is_name_available);

			if (CustomPropertyOptionsDialog::execute(result))
			{
				action(property, result.name, -1);
			}
		} break;

		default:
			Q_ASSERT(false);
			break;
	}
}

void CustomPropertyEditorDialog::on_actionPropertyAdd_triggered()
{
	QtnPropertyBase *property;
	VarProperty *var_property;

	if (getActiveVarProperty(property, var_property))
	{
		auto var_parent = var_property->VarParent();

		using namespace std::placeholders;
		propertyAction(property, tr("Add Property"),
			std::bind(&VarProperty::IsChildNameAvailable, var_parent, _1, false),
			std::bind(&CustomPropertyEditorDialog::addProperty, this, _1, QVariant(), _2, _3));
	}
}

void CustomPropertyEditorDialog::on_actionPropertyRemove_triggered()
{
	QtnPropertyBase *property;
	VarProperty *var_property;

	if (getActiveVarProperty(property, var_property))
	{
		auto set = dynamic_cast<QtnPropertySet *>(property->parent());
		if (nullptr != set)
		{
			set->removeChildProperty(property);

			var_property->RemoveFromParent();

			delete property;
		}
	}
}

void CustomPropertyEditorDialog::on_actionPropertyDuplicate_triggered()
{
	QtnPropertyBase *property;
	VarProperty *var_property;

	if (getActiveVarProperty(property, var_property))
	{
		auto var_parent = var_property->VarParent();

		using namespace std::placeholders;
		propertyAction(property, tr("Duplicate Property"),
			std::bind(&VarProperty::IsChildNameAvailable, var_parent, _1, false),
			std::bind(&CustomPropertyEditorDialog::duplicateProperty, this, _1, _2, _3));
	}
}

void CustomPropertyEditorDialog::on_actionPropertyOptions_triggered()
{
	QtnPropertyBase *property;
	VarProperty *var_property;

	if (getActiveVarProperty(property, var_property))
	{
		auto var_parent = var_property->VarParent();

		using namespace std::placeholders;
		propertyAction(property, tr("Property Options"),
			std::bind(&VarProperty::IsChildNameAvailable, var_parent, _1, true),
			std::bind(&CustomPropertyEditorDialog::propertyOptions, this, _1, _2, _3));
	}
}
