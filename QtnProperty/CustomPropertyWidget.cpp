#include "CustomPropertyWidget.h"

#include "VarProperty.h"

#include "Utils/InplaceEditing.h"
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
	, lastAddType(QVariant::Invalid)
	, dataPtr(nullptr)
	, rootSet(nullptr)
	, readOnly(false)
	, autoUpdate(false)
{
}

void CustomPropertyWidget::setReadOnly(bool value)
{
	if (!readOnly)
	{
		readOnly = value;
		if (nullptr != rootSet)
		{
			if (value)
				rootSet->addState(QtnPropertyStateImmutable, true);
			else
				rootSet->removeState(QtnPropertyStateImmutable, true);
		}
	}
}

void CustomPropertyWidget::setData(QVariant *data_ptr, const QString &title, bool force)
{
	if (force || this->dataPtr != data_ptr)
	{
		this->dataPtr = data_ptr;
		setPropertySet(nullptr);
		delete rootSet;

		if (nullptr != data_ptr)
		{
			rootSet = new QtnPropertySet(this);
			if (readOnly)
				rootSet->setState(QtnPropertyStateImmutable, true);

			newProperty(rootSet, *data_ptr, title, -1, nullptr);

			setPropertySet(rootSet);
		} else
			rootSet = nullptr;
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

		dialog.setType(lastAddType);

		if (dialog.execute(result_data))
		{
			lastAddType = result_data.value.type();
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
	if (autoUpdate != yes)
	{
		autoUpdate = yes;

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

		if (autoUpdate)
			updateData();
	}
}

void CustomPropertyWidget::editData()
{
	emit dataEdited();
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
	auto varProperty = getVarProperty(destination);
	if (nullptr != varProperty)
	{
		CustomPropertyData customData;
		if (data->hasFormat(kCustomPropertyData))
		{
			auto doc = QJsonDocument::fromBinaryData(data->data(kCustomPropertyData));

			if (doc.isObject())
			{
				auto obj = doc.object();

				if (QtnApplyPosition::Over == position
				&&	varProperty->GetType() == VarProperty::Value)
				{
					position = QtnApplyPosition::After;
				}

				bool ok = false;

				int insertIndex = varProperty->GetIndex();

				for (auto it = obj.begin(); it != obj.end(); ++it)
				{
					customData.value = it.value().toVariant();

					switch (position)
					{
						case QtnApplyPosition::Before:
						case QtnApplyPosition::After:
						{
							if (varProperty != varProperty->TopParent())
							{
								auto parent_prop = dynamic_cast<QtnPropertyBase *>(destination->parent());
								switch (getVarProperty(parent_prop)->GetType())
								{
									case VarProperty::Map:
									{
										customData.index = -1;
										customData.name = it.key();
										addProperty(parent_prop, customData);
										ok = true;
									}	break;

									case VarProperty::List:
									{
										customData.name.clear();

										customData.index = insertIndex;

										if (QtnApplyPosition::After == position)
											customData.index++;

										addProperty(parent_prop, customData);
										ok = true;
									}	break;

									default:
										break;
								}
							}
						}	break;

						case QtnApplyPosition::Over:
						{
							switch (varProperty->GetType())
							{
								case VarProperty::Map:
								{
									customData.index = -1;
									customData.name = it.key();
									addProperty(destination, customData);
									ok = true;
								}	break;

								case VarProperty::List:
								{
									customData.name.clear();
									customData.index = varProperty->GetChildrenCount();
									addProperty(destination, customData);
									ok = true;
								}	break;

								default:
									break;
							}

						}	break;
					}

					insertIndex++;
				}

				if (ok)
					return true;
			}

		} else
		{
			if (data->hasColor())
			{
				CustomPropertyData customData;
				customData.index = varProperty->GetIndex();
				customData.name = varProperty->GetName();
				customData.value = data->colorData();
				updatePropertyOptions(destination, customData);

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

				CustomPropertyData customData;
				customData.index = varProperty->GetIndex();
				customData.name = varProperty->GetName();

				if (list.size() > 1)
				{
					customData.value = list;
				} else
				{
					customData.value = list.at(0);
				}

				updatePropertyOptions(destination, customData);

				return true;
			}

			if (data->hasText())
			{
				auto text = data->text().toUtf8();
				text.prepend('{');
				text.append('}');
				QJsonParseError parseResult;
				auto doc = QJsonDocument::fromJson(text, &parseResult);

				if (QJsonParseError::NoError == parseResult.error)
				{
					auto object = doc.object();
					for (auto it = object.begin(); it != object.end(); ++it)
					{
						customData.index = varProperty->GetIndex();
						customData.name = it.key();
						customData.value = it.value().toVariant();

						QMessageBox::StandardButton choice = QMessageBox::No;

						auto insertDestination = dynamic_cast<QtnPropertySet *>(destination);

						if (nullptr == insertDestination)
						{
							if (varProperty != varProperty->TopParent())
								insertDestination = dynamic_cast<QtnPropertySet *>(destination->parent());
						}

						if (nullptr != insertDestination)
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
								if (insertDestination == destination)
								{
									switch (varProperty->GetType())
									{
										case VarProperty::List:
											customData.index = varProperty->GetChildrenCount();
											break;

										case VarProperty::Map:
											customData.index = -1;
											break;

										default:
											break;
									}
								}
								addProperty(insertDestination, customData);
							}	break;

							case QMessageBox::No:
							{
								updatePropertyOptions(destination, customData);
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

					doc = QJsonDocument::fromJson(text, &parseResult);

					if (QJsonParseError::NoError == parseResult.error)
					{
						auto array = doc.array();

						customData.index = varProperty->GetIndex();
						customData.name = varProperty->GetName();

						if (array.size() == 1)
						{
							customData.value = array.at(0).toVariant();
							updatePropertyOptions(destination, customData);

							return true;
						}

						if (array.size() > 1)
						{
							customData.value = array.toVariantList();
							updatePropertyOptions(destination, customData);
							return true;
						}
					}
				}

				customData.index = varProperty->GetIndex();
				customData.name = varProperty->GetName();
				customData.value = data->text();
				updatePropertyOptions(destination, customData);
				return true;
			}
		}
	}

	return false;
}

void CustomPropertyWidget::updateData()
{
	auto propertySet = propertySet();
	if (!readOnly && nullptr != dataPtr && nullptr != propertySet)
	{
		auto varProperty = getVarProperty(propertySet->childProperties().at(0));
		*dataPtr = varProperty->CreateVariant();
		editData();
	}
}

void CustomPropertyWidget::updateSet(QtnPropertyBase *setProperty, int childIndex)
{
	Q_ASSERT(nullptr != setProperty);

	auto varProperty = getVarProperty(setProperty);
	Q_ASSERT(nullptr != varProperty);

	auto data = varProperty->CreateVariant();

	auto var_parent = varProperty->VarParent();

	varProperty->RemoveFromParent();

	auto new_set = dynamic_cast<QtnPropertySet *>(newProperty(nullptr, data, varProperty->GetName(),
															  varProperty->GetIndex(), var_parent));

	Q_ASSERT(nullptr != new_set);

	auto property_parent = dynamic_cast<QtnPropertySet *>(setProperty->parent());
	Q_ASSERT(nullptr != property_parent);

	int property_index = property_parent->childProperties().indexOf(setProperty);
	property_parent->removeChildProperty(setProperty);

	delete setProperty;

	property_parent->addChildProperty(new_set, true, property_index);
	auto view = propertyView();
	auto &child_properties = new_set->childProperties();
	if (child_properties.isEmpty())
		view->setActiveProperty(new_set);
	else
	{
		int count = child_properties.count();
		if (childIndex >= count)
			childIndex = count - 1;
		view->setActiveProperty(child_properties.at(childIndex));
	}

	if (autoUpdate)
		updateData();
}

bool CustomPropertyWidget::getActiveVarProperty(QtnPropertyBase *&property, VarProperty *&varProperty)
{
	property = propertyView()->activeProperty();

	varProperty = getVarProperty(property);

	return varProperty != nullptr;
}

VarProperty *CustomPropertyWidget::getVarProperty(QtnPropertyBase *source)
{
	VarProperty *var_property = nullptr;
	if (nullptr != source)
		var_property = source->findChild<VarProperty *>(QString(), Qt::FindDirectChildrenOnly);

	return var_property;
}

QtnPropertyBase *CustomPropertyWidget::newProperty(QtnPropertySet *parent,
												   const QVariant &value,
												   const QString &key,
												   int index,
												   VarProperty *mapParent)
{
	return VarProperty::NewExtraProperty(parent, value, key, index, mapParent,
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

	if (autoUpdate)
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

		if (autoUpdate)
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
