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
#include <QPushButton>

static const QString kDragDropMimeType = "CustomPropertyDragDrop";
static const QString kCustomPropertyData = "QtnCustomPropertyData";

CustomPropertyWidget::CustomPropertyWidget(QWidget *parent)
	: QtnPropertyWidgetEx(parent)
	, dataPtr(nullptr)
	, rootSet(nullptr)
	, lastAddType(QVariant::Invalid)
	, readOnly(false)
	, autoUpdate(false)
	, backupAutoUpdate(false)
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

void CustomPropertyWidget::setData(QVariant *dataPtr, const QString &title, bool force)
{
	if (force || this->dataPtr != dataPtr)
	{
		this->dataPtr = dataPtr;
		setPropertySet(nullptr);
		delete rootSet;

		if (nullptr != dataPtr)
		{
			rootSet = new QtnPropertySet(this);
			if (readOnly)
				rootSet->setState(QtnPropertyStateImmutable, true);

			newProperty(rootSet, *dataPtr, title, -1, nullptr);

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

void CustomPropertyWidget::onPropertyValueAccept(void *valueToAccept, bool *accept)
{
	if (nullptr != accept)
	{
		bool ok = VarProperty::PropertyValueAccept(dynamic_cast<QtnProperty *>(sender()), valueToAccept);
		Q_ASSERT(ok);

		*accept = ok;

		if (autoUpdate)
			updateData();
	}
}

void CustomPropertyWidget::editData(const QVariant &oldValue)
{
	emit dataEdited(oldValue);
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
	auto varProperty = getVarProperty(property);
	if (nullptr != varProperty)
	{
		switch (action)
		{
			case Qt::MoveAction:
			case Qt::CopyAction:
			case Qt::IgnoreAction:
			{
				auto mime = new QMimeData;

				auto variant = varProperty->CreateVariant();

				QJsonObject jobj;
				jobj.insert(varProperty->GetName(), QJsonValue::fromVariant(variant));

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

bool CustomPropertyWidget::insertReplaceOrCancel(QtnPropertyBase *destination, CustomPropertyData &customData)
{
	auto varProperty = getVarProperty(destination);
	Q_ASSERT(nullptr != varProperty);

	enum
	{
		CANCEL,
		INSERT,
		REPLACE
	};

	int choice = REPLACE;

	auto insertDestination = dynamic_cast<QtnPropertySet *>(destination);

	if (nullptr == insertDestination)
	{
		if (varProperty != varProperty->TopParent())
			insertDestination = dynamic_cast<QtnPropertySet *>(destination->parent());
	}

	if (nullptr != insertDestination)
	{
		QMessageBox mb(QMessageBox::Question, QCoreApplication::applicationName(),
					   tr("Do you want to insert new property "
						  "from clipboard or to replace the selected one?"),
					   QMessageBox::Cancel, this);

		auto insertButton = mb.addButton(tr("Insert", "Paste"), QMessageBox::AcceptRole);
		auto replaceButton = mb.addButton(tr("Replace", "Paste"), QMessageBox::AcceptRole);
		mb.setDefaultButton(QMessageBox::Cancel);

		mb.show();
		mb.raise();
		mb.exec();

		if (mb.clickedButton() == insertButton)
			choice = INSERT;
		else
		if (mb.clickedButton() == replaceButton)
			choice = REPLACE;
		else
			choice = CANCEL;
	}

	switch (choice)
	{
		case INSERT:
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

		case REPLACE:
		{
			customData.name = varProperty->GetName();
			updatePropertyOptions(destination, customData);
		}	break;

		default:
			return false;
	}

	return true;
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
			customData.index = varProperty->GetIndex();
			customData.name = "";

			if (data->hasColor())
			{
				customData.value = data->colorData();

				return insertReplaceOrCancel(destination, customData);
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

				if (list.size() > 1)
				{
					customData.value = list;
				} else
				{
					customData.value = list.at(0);
				}

				return insertReplaceOrCancel(destination, customData);
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

					if (object.size() == 1)
					{
						auto it = object.begin();
						customData.name = it.key();
						customData.value = it.value().toVariant();

						return insertReplaceOrCancel(destination, customData);
					}

					if (object.size() > 1)
					{
						customData.value = object.toVariantMap();

						return insertReplaceOrCancel(destination, customData);
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

						if (array.size() == 1)
						{
							customData.value = array.at(0).toVariant();

							return insertReplaceOrCancel(destination, customData);
						}

						if (array.size() > 1)
						{
							customData.value = array.toVariantList();

							return insertReplaceOrCancel(destination, customData);
						}
					}
				}

				doc = QJsonDocument::fromJson(data->text().toUtf8(), &parseResult);
				if (QJsonParseError::NoError == parseResult.error)
				{
					customData.value = doc.toVariant();
				} else
				{
					customData.value = data->text();
				}

				return insertReplaceOrCancel(destination, customData);
			}
		}
	}

	return false;
}

bool CustomPropertyWidget::drop(const QMimeData *data, QtnPropertyBase *property, QtnApplyPosition applyPosition)
{
	backupAutoUpdate = autoUpdate;
	autoUpdate = false;
	return QtnPropertyWidgetEx::drop(data, property, applyPosition);
}

void CustomPropertyWidget::dropEnd()
{
	QtnPropertyWidgetEx::dropEnd();
	autoUpdate = backupAutoUpdate;
	if (autoUpdate)
		updateData();
}

void CustomPropertyWidget::updateData()
{
	auto propertySet = this->propertySet();
	if (!readOnly && nullptr != dataPtr && nullptr != propertySet)
	{
		auto varProperty = getVarProperty(propertySet->childProperties().at(0));
		auto oldValue = *dataPtr;
		*dataPtr = varProperty->CreateVariant();
		editData(oldValue);
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
	VarProperty *varProperty = nullptr;
	if (nullptr != source)
		varProperty = source->findChild<VarProperty *>(QString(), Qt::FindDirectChildrenOnly);

	return varProperty;
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
	auto varProperty = getVarProperty(source);
	Q_ASSERT(nullptr != varProperty);
	auto set = dynamic_cast<QtnPropertySet *>(source);
	Q_ASSERT(nullptr != set);
	if (data.index >= 0)
		Q_ASSERT(data.index <= varProperty->GetChildrenCount());
	auto new_property = newProperty(nullptr, data.value, data.name, data.index, varProperty);

	std::vector<QtnPropertyBase *> children;
	for (auto child : set->childProperties())
	{
		children.push_back(child);
	}

	if (data.index >= 0)
		children.insert(children.begin() + data.index, new_property);
	else
		children.push_back(new_property);

	auto isList = (VarProperty::List == varProperty->GetType());

	if (isList)
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
	auto varProperty = getVarProperty(source);
	Q_ASSERT(nullptr != varProperty);

	auto set = dynamic_cast<QtnPropertySet *>(source->parent());
	Q_ASSERT(nullptr != set);

	addProperty(set, { data.index, data.name, varProperty->CreateVariant() });
}

void CustomPropertyWidget::updatePropertyOptions(QtnPropertyBase *source, const CustomPropertyData &data)
{
	auto varProperty = getVarProperty(source);
	Q_ASSERT(nullptr != varProperty);

	auto set = dynamic_cast<QtnPropertySet *>(source->parent());
	Q_ASSERT(nullptr != set);

	bool refresh_siblings = false;

	int old_index = varProperty->GetIndex();

	bool top_property = (varProperty == varProperty->TopParent());

	if (!top_property)
	{
		refresh_siblings = varProperty->SetIndex(data.index);
		refresh_siblings = varProperty->SetName(data.name) || refresh_siblings;
	}

	auto var_parent = varProperty->VarParent();
	if (!refresh_siblings)
	{
		int index = set->childProperties().indexOf(source);

		QString prop_name(top_property ? varProperty->GetName() : data.name);

		varProperty->RemoveFromParent();
		set->removeChildProperty(source);

		delete source;

		auto new_property = newProperty(nullptr, data.value, prop_name, data.index, var_parent);

		set->addChildProperty(new_property, true, index);
		propertyView()->setActiveProperty(new_property);

		if (autoUpdate)
			updateData();
	} else
	{
		varProperty->SetValue(data.value);

		int index = data.index;

		auto children = var_parent->GetChildren();

		if (index < 0)
		{
			std::sort(children.begin(), children.end(), [](VarProperty *a, VarProperty *b) -> bool
			{
				return QString::localeAwareCompare(a->GetName(), b->GetName()) < 0;
			});

			auto it = std::find(children.begin(), children.end(), varProperty);

			index = it - children.begin();
		} else
		if (old_index != index)
		{
			children.at(index)->SetIndex(old_index);
		}

		updateSet(set, index);
	}
}
