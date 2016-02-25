#include "CustomPropertyEditorDialog.h"
#include "ui_CustomPropertyEditorDialog.h"

#include "VarProperty.h"

#include "QtnProperty/Widget/Utils/InplaceEditing.h"

#include <QAbstractButton>
#include <QMenu>
#include <QShortcut>

CustomPropertyEditorDialog::CustomPropertyEditorDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::CustomPropertyEditorDialog)
	, last_add_type(QVariant::Invalid)
	, data_ptr(nullptr)
	, read_only(false)
{
	ui->setupUi(this);

	updateTitle();

	setWindowFlags((windowFlags() & ~(Qt::WindowContextHelpButtonHint | Qt::WindowMinMaxButtonsHint)));

	QObject::connect(ui->propertyWidget->propertyView(), &QtnPropertyView::activePropertyChanged,
					 this, &CustomPropertyEditorDialog::onActivePropertyChanged);

	auto shortcut = new QShortcut(ui->actionPropertyOptions->shortcut(), this);
	QObject::connect(shortcut, &QShortcut::activated,
					 this, &CustomPropertyEditorDialog::on_actionPropertyOptions_triggered);


	shortcut = new QShortcut(ui->actionPropertyRemove->shortcut(), this);
	QObject::connect(shortcut, &QShortcut::activated,
					 this, &CustomPropertyEditorDialog::on_actionPropertyRemove_triggered);

}

CustomPropertyEditorDialog::~CustomPropertyEditorDialog()
{
	delete ui;
}

bool CustomPropertyEditorDialog::execute(const QString &title, QVariant &data)
{
	data_ptr = &data;
	auto widget = ui->propertyWidget;

	auto property_set = new QtnPropertySet(widget);
	if (read_only)
		property_set->setState(QtnPropertyStateImmutable, true);

	newProperty(property_set, data, title, -1, nullptr);

	widget->setPropertySet(property_set);

	updateActions(widget->propertyView()->activeProperty());

	show();
	raise();
	exec();

	updateData();

	data_ptr = nullptr;

	widget->setPropertySet(nullptr);
	delete property_set;

	return (result() == Accepted);
}

void CustomPropertyEditorDialog::setReadOnly(bool value)
{
	if (read_only != value)
	{
		read_only = value;
		updateTitle();

		if (value)
		{
			ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
		} else
		{
			ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
		}
	}
}

void CustomPropertyEditorDialog::accept()
{
	qtnStopInplaceEdit(false);

	QDialog::accept();
}

void CustomPropertyEditorDialog::reject()
{
	qtnStopInplaceEdit(false);

	QDialog::reject();
}

void CustomPropertyEditorDialog::onActivePropertyChanged(QtnPropertyBase *activeProperty)
{
	updateActions(activeProperty);
}

void CustomPropertyEditorDialog::onPropertyValueAccept(const QtnProperty *property, void *valueToAccept, bool *accept)
{
	if (nullptr != accept)
	{
		bool ok = VarProperty::PropertyValueAccept(property, valueToAccept);
		Q_ASSERT(ok);

		*accept = ok;
	}
}

void CustomPropertyEditorDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	switch (ui->buttonBox->buttonRole(button))
	{
		case QDialogButtonBox::ApplyRole:
			if (nullptr != data_ptr)
			{
				updateData();
				emit apply(*data_ptr);
			}
			break;

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
	auto property = ui->propertyWidget->propertyView()->activeProperty();

	if (nullptr != property)
	{
		qtnStopInplaceEdit(false);

		auto menu = new QMenu(this);

		menu->addAction(ui->actionPropertyAdd);
		menu->addAction(ui->actionPropertyDuplicate);
		menu->addAction(ui->actionPropertyOptions);
		menu->addAction(ui->actionPropertyRemove);

		updateActions(property);

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
	VarProperty *var_property = nullptr;
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

void CustomPropertyEditorDialog::addProperty(QtnPropertyBase *source, const CustomPropertyData &data)
{
	auto var_property = getVarProperty(source);
	Q_ASSERT(nullptr != var_property);
	auto set = dynamic_cast<QtnPropertySet *>(source);
	Q_ASSERT(nullptr != set);
	auto new_property = newProperty(nullptr, data.value, data.name, data.index, var_property);

	std::vector<QtnPropertyBase *> children;
	for (auto child : set->childProperties())
	{
		children.push_back(child);
	}

	children.push_back(new_property);

	auto is_list = (VarProperty::List == var_property->GetType());

	if (is_list)
	{
		std::sort(children.begin(), children.end(),
		[](QtnPropertyBase *a, QtnPropertyBase *b) -> bool
		{
			auto va = getVarProperty(a);
			auto vb = getVarProperty(b);

			return va->GetIndex() < vb->GetIndex();
		});

		int count = static_cast<int>(children.size());
		for (int i = 0; i < count; i++)
		{
			auto property = children.at(i);
			auto var_property = getVarProperty(property);
			var_property->SetIndex(i);
			property->setName(var_property->GetName());
		}
	} else
	{
		std::sort(children.begin(), children.end(),
		[](QtnPropertyBase *a, QtnPropertyBase *b) -> bool
		{
			return a->name() < b->name();
		});
	}

	auto it = std::find(children.begin(), children.end(), new_property);

	set->addChildProperty(new_property, true, it - children.begin());
	ui->propertyWidget->propertyView()->setActiveProperty(new_property);
}

void CustomPropertyEditorDialog::duplicateProperty(QtnPropertyBase *source,
												   const CustomPropertyData &data)
{
	auto var_property = getVarProperty(source);
	Q_ASSERT(nullptr != var_property);

	auto set = dynamic_cast<QtnPropertySet *>(source->parent());
	Q_ASSERT(nullptr != set);

	addProperty(set, { data.index, data.name, var_property->CreateVariant() });
}

void CustomPropertyEditorDialog::updatePropertyOptions(QtnPropertyBase *source,
												 const CustomPropertyData &data)
{
	auto var_property = getVarProperty(source);
	Q_ASSERT(nullptr != var_property);

	auto set = dynamic_cast<QtnPropertySet *>(source->parent());
	Q_ASSERT(nullptr != set);

	bool refresh_siblings = false;

	int old_index = var_property->GetIndex();

	if (var_property != var_property->TopParent())
	{
		refresh_siblings = var_property->SetIndex(data.index);
		refresh_siblings = var_property->SetName(data.name) || refresh_siblings;
	}

	auto var_parent = var_property->VarParent();
	if (!refresh_siblings)
	{
		int index = set->childProperties().indexOf(source);

		var_property->RemoveFromParent();
		set->removeChildProperty(source);

		delete source;

		auto new_property = newProperty(nullptr, data.value, data.name, data.index, var_parent);

		set->addChildProperty(new_property, true, index);
		ui->propertyWidget->propertyView()->setActiveProperty(new_property);
	} else
	{
		var_property->SetValue(data.value);

		int index = data.index;

		auto children = var_parent->GetChildren();

		if (index < 0)
		{
			std::sort(children.begin(), children.end(), [](VarProperty *a, VarProperty *b) -> bool
			{
				return a->GetName() < b->GetName();
			});

			auto it = std::find(children.begin(), children.end(), var_property);

			index = it - children.begin();
		} else
		if (old_index != index)
		{
			children.at(index)->SetIndex(old_index);
		}

		updateSet(set, index);
	}
}

void CustomPropertyEditorDialog::on_actionPropertyAdd_triggered()
{
	QtnPropertyBase *property;
	VarProperty *var_property;

	if (getActiveVarProperty(property, var_property))
	{
		CustomPropertyOptionsDialog dialog(this);
		CustomPropertyData result_data;

		switch (var_property->GetType())
		{
			case VarProperty::List:
			{
				dialog.setWindowTitle(tr("Add Element"));
				dialog.initWithCount(-1, var_property->GetChildrenCount());
			} break;

			case VarProperty::Map:
			{
				dialog.setWindowTitle(tr("Add Property"));
				dialog.initWithName(QString(),
									std::bind(&VarProperty::IsChildNameAvailable,
											  var_property, std::placeholders::_1, nullptr));
			} break;

			default:
				Q_ASSERT(false);
				break;
		}

		dialog.setType(last_add_type);

		if (dialog.execute(result_data))
		{
			last_add_type = result_data.value.type();
			addProperty(property, result_data);
		}
	}
}

void CustomPropertyEditorDialog::on_actionPropertyRemove_triggered()
{
	QtnPropertyBase *property;
	VarProperty *var_property;

	if (getActiveVarProperty(property, var_property))
	{
		if (var_property != var_property->TopParent())
		{
			var_property->RemoveFromParent();

			auto set = dynamic_cast<QtnPropertySet *>(property->parent());
			updateSet(set, set->childProperties().indexOf(property));
		}
	}
}

void CustomPropertyEditorDialog::on_actionPropertyDuplicate_triggered()
{
	QtnPropertyBase *property;
	VarProperty *var_property;

	if (getActiveVarProperty(property, var_property))
	{
		CustomPropertyOptionsDialog dialog(this);
		CustomPropertyData result_data;

		auto var_parent = var_property->VarParent();

		switch (var_parent->GetType())
		{
			case VarProperty::List:
			{
				dialog.setWindowTitle(tr("Duplicate Element"));
				dialog.initWithCount(var_property->GetIndex(),
									 var_parent->GetChildrenCount());
			} break;

			case VarProperty::Map:
			{
				dialog.setWindowTitle(tr("Duplicate Property"));
				dialog.initWithName(var_property->GetName(),
									std::bind(&VarProperty::IsChildNameAvailable,
											  var_parent, std::placeholders::_1, nullptr));
			} break;

			default:
				Q_ASSERT(false);
				break;
		}


		dialog.setType(var_property->GetVariantType());
		dialog.setTypeBoxEnabled(false);

		if (dialog.execute(result_data))
			duplicateProperty(property, result_data);
	}
}

void CustomPropertyEditorDialog::on_actionPropertyOptions_triggered()
{
	QtnPropertyBase *property;
	VarProperty *var_property;

	if (getActiveVarProperty(property, var_property))
	{
		CustomPropertyOptionsDialog dialog(this);
		CustomPropertyData result_data;

		auto var_parent = var_property->VarParent();

		dialog.setWindowTitle(tr("Property Options"));

		if (nullptr == var_parent)
		{
			dialog.initWithName(var_property->GetName(), nullptr, true);
		} else
		{
			switch (var_parent->GetType())
			{
				case VarProperty::List:
				{
					dialog.setWindowTitle(tr("Element Options"));
					dialog.initWithCount(var_property->GetIndex(),
										 var_parent->GetChildrenCount() - 1);
				} break;

				case VarProperty::Map:
				{
					dialog.initWithName(var_property->GetName(),
										std::bind(&VarProperty::IsChildNameAvailable,
												  var_parent, std::placeholders::_1, var_property));
				} break;

				default:
					Q_ASSERT(false);
					break;
			}
		}
		dialog.setType(var_property->GetVariantType());

		if (dialog.execute(result_data))
		{
			auto old_data = var_property->CreateVariant();
			switch (result_data.value.type())
			{
				case QVariant::Bool:
					result_data.value = old_data.toBool();
					break;

				case QVariant::Char:
				case QVariant::String:
					result_data.value = old_data.toString();
					break;

				case QVariant::Int:
				case QVariant::UInt:
				case QVariant::LongLong:
				case QVariant::ULongLong:
				case QVariant::Double:
					result_data.value = old_data.toDouble();
					break;

				default:
					if (var_property->GetType() == VarProperty::GetTypeFromValue(result_data.value))
						result_data.value.swap(old_data);
					else
					{
						switch (var_property->GetType())
						{
							case VarProperty::List:
							{
								if (result_data.value.type() == QVariant::Map)
								{
									old_data.clear();
									QVariantMap result;
									for (auto property : var_property->GetChildren())
										result.insert(property->GetName(), property->CreateVariant());

									result_data.value = result;
								}
							}	break;

							case VarProperty::Map:
							{
								if (result_data.value.type() == QVariant::List)
								{
									old_data.clear();
									QVariantList result;
									for (auto property : var_property->GetChildren())
										result.push_back(property->CreateVariant());

									result_data.value = result;
								}
							}	break;

							default:
								break;
						}
					}
					break;
			}

			updatePropertyOptions(property, result_data);
		}
	}
}

void CustomPropertyEditorDialog::updateData()
{
	auto property_set = ui->propertyWidget->propertySet();
	if (!read_only && nullptr != data_ptr && nullptr != property_set)
	{
		qtnStopInplaceEdit(false);

		auto var_property = getVarProperty(property_set->childProperties().at(0));
		*data_ptr = var_property->CreateVariant();
	}
}

void CustomPropertyEditorDialog::updateSet(QtnPropertyBase *set_property, int child_index)
{
	Q_ASSERT(nullptr != set_property);

	auto var_property = getVarProperty(set_property);
	Q_ASSERT(nullptr != var_property);

	auto data = var_property->CreateVariant();

	auto var_parent = var_property->VarParent();

	var_property->RemoveFromParent();

	auto new_set = dynamic_cast<QtnPropertySet *>(newProperty(nullptr, data, var_property->GetName(),
															  var_property->GetIndex(), var_parent));

	Q_ASSERT(nullptr != new_set);

	auto property_parent = dynamic_cast<QtnPropertySet *>(set_property->parent());
	Q_ASSERT(nullptr != property_parent);

	int property_index = property_parent->childProperties().indexOf(set_property);
	property_parent->removeChildProperty(set_property);

	delete set_property;

	property_parent->addChildProperty(new_set, true, property_index);
	auto view = ui->propertyWidget->propertyView();
	auto &child_properties = new_set->childProperties();
	if (child_properties.isEmpty())
		view->setActiveProperty(new_set);
	else
	{
		int count = child_properties.count();
		if (child_index >= count)
			child_index = count - 1;
		view->setActiveProperty(child_properties.at(child_index));
	}
}

void CustomPropertyEditorDialog::updateActions(QtnPropertyBase *property)
{
	auto add_text = tr("Add...");

	auto var_property = getVarProperty(property);
	if (nullptr != var_property)
	{
		switch (var_property->GetType())
		{
			case VarProperty::Map:
				add_text = tr("Add Property...");
				break;

			case VarProperty::List:
				add_text = tr("Add Element...");
				break;

			default:
				break;
		}

		bool not_top_parent = (var_property != var_property->TopParent());
		ui->actionPropertyAdd->setEnabled(property->id() == VarProperty::PID_EXTRA);
		ui->actionPropertyDuplicate->setEnabled(not_top_parent);
		ui->actionPropertyOptions->setEnabled(true);
		ui->actionPropertyRemove->setEnabled(not_top_parent);
	} else
	{
		ui->actionPropertyAdd->setEnabled(false);
		ui->actionPropertyDuplicate->setEnabled(false);
		ui->actionPropertyOptions->setEnabled(false);
		ui->actionPropertyRemove->setEnabled(false);
	}

	ui->actionPropertyAdd->setText(add_text);
}

void CustomPropertyEditorDialog::updateTitle()
{
	setWindowTitle(read_only ? tr("Read-only Properties") : tr("Edit Custom Properties"));
}
