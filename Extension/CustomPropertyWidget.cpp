#include "CustomPropertyWidget.h"

#include "VarProperty.h"

#include "QtnProperty/Widget/Utils/InplaceEditing.h"
#include "CustomPropertyOptionsDialog.h"

#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDrag>
#include <QMimeData>
#include <QMenu>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QCoreApplication>

static const QString kDragDropMimeType = "CustomPropertyDragDrop";
static const QString kCustomPropertyData = "QtnCustomPropertyData";

CustomPropertyWidget::CustomPropertyWidget(QWidget *parent)
	: QtnPropertyWidgetEx(parent)
	, last_add_type(QVariant::Invalid)
	, data_ptr(nullptr)
	, root_set(nullptr)
	, read_only(false)
	, auto_update(false)
{
}

void CustomPropertyWidget::setReadOnly(bool value)
{
	if (!read_only)
	{
		read_only = value;
		if (nullptr != root_set)
		{
			if (value)
				root_set->addState(QtnPropertyStateImmutable, true);
			else
				root_set->removeState(QtnPropertyStateImmutable, true);
		}
	}
}

void CustomPropertyWidget::setData(QVariant *data_ptr, const QString &title)
{
	if (this->data_ptr != data_ptr)
	{
		this->data_ptr = data_ptr;
		setPropertySet(nullptr);
		delete root_set;

		if (nullptr != data_ptr)
		{
			root_set = new QtnPropertySet(this);
			if (read_only)
				root_set->setState(QtnPropertyStateImmutable, true);

			newProperty(root_set, *data_ptr, title, -1, nullptr);

			setPropertySet(root_set);
		} else
			root_set = nullptr;
	}
}

bool CustomPropertyWidget::canDeleteProperty(QtnPropertyBase *property)
{
	auto var_property = getVarProperty(property);
	if (nullptr != var_property)
		return (var_property != var_property->TopParent());

	return false;
}

bool CustomPropertyWidget::canCutToClipboard()
{
	return canDeleteProperty(propertyView()->activeProperty());
}

void CustomPropertyWidget::addProperty()
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
				dialog.setWindowTitle(tr("New Element"));
				dialog.initWithCount(-1, var_property->GetChildrenCount());
			} break;

			case VarProperty::Map:
			{
				dialog.setWindowTitle(tr("New Property"));
				dialog.initWithName(QString(),
									std::bind(&VarProperty::IsChildNameAvailable,
											  var_property, std::placeholders::_1, nullptr));
			} break;

			default:
				return;
		}

		dialog.setType(last_add_type);

		if (dialog.execute(result_data))
		{
			last_add_type = result_data.value.type();
			addProperty(property, result_data);
		}
	}
}

void CustomPropertyWidget::duplicateProperty()
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

void CustomPropertyWidget::propertyOptions()
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

void CustomPropertyWidget::setAutoUpdate(bool yes)
{
	if (auto_update != yes)
	{
		auto_update = yes;

		if (yes)
			updateData();
	}
}

void CustomPropertyWidget::onPropertyValueAccept(const QtnProperty *property, void *valueToAccept, bool *accept)
{
	if (nullptr != accept)
	{
		bool ok = VarProperty::PropertyValueAccept(property, valueToAccept);
		Q_ASSERT(ok);

		*accept = ok;

		if (auto_update)
			updateData();
	}
}

bool CustomPropertyWidget::dataHasSupportedFormats(const QMimeData *data)
{
	if (nullptr != data)
	{
		return (QtnPropertyWidgetEx::dataHasSupportedFormats(data)
			||	data->hasFormat(kCustomPropertyData));
	}

	return false;
}

void CustomPropertyWidget::deleteProperty(QtnPropertyBase *property)
{
	auto var_property = getVarProperty(property);

	if (nullptr != var_property)
	{
		if (var_property != var_property->TopParent())
		{
			var_property->RemoveFromParent();

			auto set = dynamic_cast<QtnPropertySet *>(property->parent());
			auto &set_properties = set->childProperties();
			auto active_property_index = set_properties.indexOf(propertyView()->activeProperty());
			updateSet(set, active_property_index >= 0
							? active_property_index
							: set_properties.indexOf(property));
		}
	}
}

QMimeData *CustomPropertyWidget::getPropertyDataForAction(QtnPropertyBase *property,
														  Qt::DropAction action)
{
	auto var_property = getVarProperty(property);
	if (nullptr != var_property)
	{
		switch (action)
		{
			case Qt::MoveAction:
			case Qt::CopyAction:
			case Qt::IgnoreAction:
			{
				auto mime = new QMimeData;

				auto variant = var_property->CreateVariant();

				QJsonObject jobj;

				jobj.insert(var_property->GetName(), QJsonValue::fromVariant(variant));

				QJsonDocument doc;
				doc.setObject(jobj);

				if (action == Qt::IgnoreAction)
				{
					QByteArray json(doc.toJson());
					int start = json.indexOf('{') + 1;
					int end = json.lastIndexOf('}');

					mime->setText(QString::fromUtf8(&json.constData()[start],
													end - start).trimmed());
				} else
				{
					mime->setData(kCustomPropertyData, doc.toBinaryData());
				}

				return mime;
			}

			default:
				break;
		}
	}

	return nullptr;
}

bool CustomPropertyWidget::applyPropertyData(const QMimeData *data,
											 QtnPropertyBase *destination,
											 QtnApplyPosition position)
{
	auto var_property = getVarProperty(destination);
	if (nullptr != var_property)
	{
		CustomPropertyData custom_data;
		if (data->hasFormat(kCustomPropertyData))
		{
			auto doc = QJsonDocument::fromBinaryData(data->data(kCustomPropertyData));

			if (doc.isObject())
			{
				auto obj = doc.object();

				if (QtnApplyPosition::Over == position
				&&	var_property->GetType() == VarProperty::Value)
				{
					position = QtnApplyPosition::After;
				}

				bool ok = false;

				int insert_index = var_property->GetIndex();

				for (auto it = obj.begin(); it != obj.end(); ++it)
				{
					custom_data.value = it.value().toVariant();

					switch (position)
					{
						case QtnApplyPosition::Before:
						case QtnApplyPosition::After:
						{
							if (var_property != var_property->TopParent())
							{
								auto parent_prop = dynamic_cast<QtnPropertyBase *>(destination->parent());
								switch (getVarProperty(parent_prop)->GetType())
								{
									case VarProperty::Map:
									{
										custom_data.index = -1;
										custom_data.name = it.key();
										addProperty(parent_prop, custom_data);
										ok = true;
									}	break;

									case VarProperty::List:
									{
										custom_data.name.clear();

										custom_data.index = insert_index;

										if (QtnApplyPosition::After == position)
											custom_data.index++;

										addProperty(parent_prop, custom_data);
										ok = true;
									}	break;

									default:
										break;
								}
							}
						}	break;

						case QtnApplyPosition::Over:
						{
							switch (var_property->GetType())
							{
								case VarProperty::Map:
								{
									custom_data.index = -1;
									custom_data.name = it.key();
									addProperty(destination, custom_data);
									ok = true;
								}	break;

								case VarProperty::List:
								{
									custom_data.name.clear();
									custom_data.index = var_property->GetChildrenCount();
									addProperty(destination, custom_data);
									ok = true;
								}	break;

								default:
									break;
							}

						}	break;
					}

					insert_index++;
				}

				if (ok)
					return true;
			}

		} else
		{
			if (data->hasColor())
			{
				CustomPropertyData custom_data;
				custom_data.index = var_property->GetIndex();
				custom_data.name = var_property->GetName();
				custom_data.value = data->colorData();
				updatePropertyOptions(destination, custom_data);

				return true;
			}

			if (data->hasUrls())
			{
				QVariantList list;

				for (auto &url : data->urls())
				{
					if (url.isLocalFile())
						list.push_back(url.toLocalFile());
					else
						list.push_back(url.toString());
				}

				CustomPropertyData custom_data;
				custom_data.index = var_property->GetIndex();
				custom_data.name = var_property->GetName();

				if (list.size() > 1)
				{
					custom_data.value = list;
				} else
				{
					custom_data.value = list.at(0);
				}

				updatePropertyOptions(destination, custom_data);

				return true;
			}

			if (data->hasText())
			{
				auto text = data->text().toUtf8();
				text.prepend('{');
				text.append('}');
				QJsonParseError parse_result;
				auto doc = QJsonDocument::fromJson(text, &parse_result);

				if (QJsonParseError::NoError == parse_result.error)
				{
					auto object = doc.object();
					for (auto it = object.begin(); it != object.end(); ++it)
					{
						custom_data.index = var_property->GetIndex();
						custom_data.name = it.key();
						custom_data.value = it.value().toVariant();

						QMessageBox::StandardButton choice = QMessageBox::No;

						auto insert_destination = dynamic_cast<QtnPropertySet *>(destination);

						if (nullptr == insert_destination)
						{
							if (var_property != var_property->TopParent())
								insert_destination = dynamic_cast<QtnPropertySet *>(destination->parent());
						}

						if (nullptr != insert_destination)
						{
							choice = QMessageBox::question(this, QCoreApplication::applicationName(),
														   tr("Do you want to insert new property from clipboard?\n"
															  "If you press 'No', selected property will be replaced."),
														   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
						}

						switch (choice)
						{
							case QMessageBox::Yes:
							{
								if (insert_destination == destination)
								{
									switch (var_property->GetType())
									{
										case VarProperty::List:
											custom_data.index = var_property->GetChildrenCount();
											break;

										case VarProperty::Map:
											custom_data.index = -1;
											break;

										default:
											break;
									}
								}
								addProperty(insert_destination, custom_data);
							}	break;

							case QMessageBox::No:
							{
								updatePropertyOptions(destination, custom_data);
							}	break;

							default:
								return false;
						}

						return true;
					}
				} else
				{
					text = data->text().toUtf8();
					text.prepend('[');
					text.append(']');

					doc = QJsonDocument::fromJson(text, &parse_result);

					if (QJsonParseError::NoError == parse_result.error)
					{
						auto array = doc.array();

						custom_data.index = var_property->GetIndex();
						custom_data.name = var_property->GetName();

						if (array.size() == 1)
						{
							custom_data.value = array.at(0).toVariant();
							updatePropertyOptions(destination, custom_data);

							return true;
						}

						if (array.size() > 1)
						{
							custom_data.value = array.toVariantList();
							updatePropertyOptions(destination, custom_data);
							return true;
						}
					}
				}

				custom_data.index = var_property->GetIndex();
				custom_data.name = var_property->GetName();
				custom_data.value = data->text();
				updatePropertyOptions(destination, custom_data);
				return true;
			}
		}
	}

	return false;
}

void CustomPropertyWidget::updateData()
{
	auto property_set = propertySet();
	if (!read_only && nullptr != data_ptr && nullptr != property_set)
	{
		auto var_property = getVarProperty(property_set->childProperties().at(0));
		*data_ptr = var_property->CreateVariant();
		emit dataChanged();
	}
}

void CustomPropertyWidget::updateSet(QtnPropertyBase *set_property, int child_index)
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
	auto view = propertyView();
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

	if (auto_update)
		updateData();
}

bool CustomPropertyWidget::getActiveVarProperty(QtnPropertyBase *&property, VarProperty *&var_property)
{
	property = propertyView()->activeProperty();

	var_property = getVarProperty(property);

	return var_property != nullptr;
}

VarProperty *CustomPropertyWidget::getVarProperty(QtnPropertyBase *source)
{
	VarProperty *var_property = nullptr;
	if (nullptr != source)
		var_property = source->findChild<VarProperty *>(QString(), Qt::FindDirectChildrenOnly);

	return var_property;
}

QtnPropertyBase *CustomPropertyWidget::newProperty(QtnPropertySet *parent, const QVariant &value, const QString &key, int index, VarProperty *map_parent)
{
	return VarProperty::NewExtraProperty(parent, value, key, index, map_parent,
	[this](QtnProperty *property)
	{
		QObject::connect(property, &QtnProperty::propertyValueAccept,
						 this, &CustomPropertyWidget::onPropertyValueAccept);
	});
}

void CustomPropertyWidget::addProperty(QtnPropertyBase *source, const CustomPropertyData &data)
{
	auto var_property = getVarProperty(source);
	Q_ASSERT(nullptr != var_property);
	auto set = dynamic_cast<QtnPropertySet *>(source);
	Q_ASSERT(nullptr != set);
	if (data.index >= 0)
		Q_ASSERT(data.index <= var_property->GetChildrenCount());
	auto new_property = newProperty(nullptr, data.value, data.name, data.index, var_property);

	std::vector<QtnPropertyBase *> children;
	for (auto child : set->childProperties())
	{
		children.push_back(child);
	}

	if (data.index >= 0)
		children.insert(children.begin() + data.index, new_property);
	else
		children.push_back(new_property);

	auto is_list = (VarProperty::List == var_property->GetType());

	if (is_list)
	{
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
			return QString::localeAwareCompare(a->name(), b->name()) < 0;
		});
	}

	auto it = std::find(children.begin(), children.end(), new_property);

	set->addChildProperty(new_property, true, it - children.begin());
	propertyView()->setActiveProperty(new_property);

	if (auto_update)
		updateData();
}

void CustomPropertyWidget::duplicateProperty(QtnPropertyBase *source, const CustomPropertyData &data)
{
	auto var_property = getVarProperty(source);
	Q_ASSERT(nullptr != var_property);

	auto set = dynamic_cast<QtnPropertySet *>(source->parent());
	Q_ASSERT(nullptr != set);

	addProperty(set, { data.index, data.name, var_property->CreateVariant() });
}

void CustomPropertyWidget::updatePropertyOptions(QtnPropertyBase *source, const CustomPropertyData &data)
{
	auto var_property = getVarProperty(source);
	Q_ASSERT(nullptr != var_property);

	auto set = dynamic_cast<QtnPropertySet *>(source->parent());
	Q_ASSERT(nullptr != set);

	bool refresh_siblings = false;

	int old_index = var_property->GetIndex();

	bool top_property = (var_property == var_property->TopParent());

	if (!top_property)
	{
		refresh_siblings = var_property->SetIndex(data.index);
		refresh_siblings = var_property->SetName(data.name) || refresh_siblings;
	}

	auto var_parent = var_property->VarParent();
	if (!refresh_siblings)
	{
		int index = set->childProperties().indexOf(source);

		QString prop_name(top_property ? var_property->GetName() : data.name);

		var_property->RemoveFromParent();
		set->removeChildProperty(source);

		delete source;

		auto new_property = newProperty(nullptr, data.value, prop_name, data.index, var_parent);

		set->addChildProperty(new_property, true, index);
		propertyView()->setActiveProperty(new_property);

		if (auto_update)
			updateData();
	} else
	{
		var_property->SetValue(data.value);

		int index = data.index;

		auto children = var_parent->GetChildren();

		if (index < 0)
		{
			std::sort(children.begin(), children.end(), [](VarProperty *a, VarProperty *b) -> bool
			{
				return QString::localeAwareCompare(a->GetName(), b->GetName()) < 0;
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
