#include "VarProperty.h"

#include "QtnProperty/Core/PropertyBase.h"
#include "QtnProperty/Core/PropertySet.h"
#include "QtnProperty/Core/Core/PropertyBool.h"
#include "QtnProperty/Core/Core/PropertyInt.h"
#include "QtnProperty/Core/Core/PropertyUInt.h"
#include "QtnProperty/Core/Core/PropertyDouble.h"
#include "QtnProperty/Core/Core/PropertyQString.h"

VarProperty::VarProperty(QObject *parent, VarProperty::Type type, const QString &name, int index, const QVariant &value)
	: QObject(parent)
	, varParent(nullptr)
	, value(type == Value ? value : QVariant())
	, name(name)
	, index(index)
	, type(type)
{

}

void VarProperty::ChangePropertyValue(const QVariant &value, QVariant *dest)
{
	if (this->value != value)
	{
		this->value = value;

		if (nullptr != dest)
		{
			auto top = TopParent();

			Q_ASSERT(nullptr != top);

			*dest = top->CreateVariant();
		}
	}
}

void VarProperty::RemoveFromParent()
{
	if (nullptr != varParent)
	{
		auto &siblings = varParent->varChildren;

		auto it = std::find(siblings.begin(), siblings.end(), this);

		if (it != siblings.end())
		{
			siblings.erase(it);
		}
	}
}

VarProperty *VarProperty::Duplicate(VarProperty *child, int newIndex)
{
	Q_ASSERT(nullptr != child);

	return AddChild(new VarProperty(nullptr, VarProperty::Value, "", newIndex,
									child->CreateVariant()), newIndex);
}

VarProperty *VarProperty::Duplicate(VarProperty *child, const QString &newName)
{
	Q_ASSERT(nullptr != child);

	return AddChild(new VarProperty(nullptr, VarProperty::Value, newName,
									-1, child->CreateVariant()));
}

VarProperty *VarProperty::Duplicate(int new_index)
{
	Q_ASSERT(nullptr != varParent);
	return varParent->Duplicate(this, new_index);
}

VarProperty *VarProperty::Duplicate(const QString &new_name)
{
	Q_ASSERT(nullptr != varParent);
	return varParent->Duplicate(this, new_name);
}

VarProperty *VarProperty::AddChild(VarProperty *child, int index)
{
	if (index < 0)
		index = varChildren.size();

	child->varParent = this;
	varChildren.insert(varChildren.begin() + index, child);

	return child;
}

VarProperty *VarProperty::AddChild(int index, const QVariant &value)
{
	return AddChild(new VarProperty(nullptr, GetTypeFromValue(value), "", index, value), index);
}

VarProperty *VarProperty::AddChild(const QString &name, const QVariant &value)
{
	return AddChild(new VarProperty(nullptr, GetTypeFromValue(value), name, -1, value));
}

VarProperty::Type VarProperty::GetTypeFromValue(const QVariant &value)
{
	Type type;
	switch (value.type())
	{
		case QVariant::Map:
			type = Map;
			break;

		case QVariant::List:
			type = List;
			break;

		default:
			type = Value;
			break;
	}

	return type;
}

VarProperty::Type VarProperty::GetType() const
{
	return type;
}

QVariant::Type VarProperty::GetVariantType() const
{
	switch (type)
	{
		case Value:
			return value.type();

		case List:
			return QVariant::List;

		case Map:
			return QVariant::Map;
	}

	return QVariant::Invalid;
}

int VarProperty::GetIndex() const
{
	return index;
}

bool VarProperty::SetIndex(int newIndex)
{
	if (index != newIndex)
	{
		index = newIndex;
		if (newIndex >= 0)
			name = QString("[%1]").arg(QString::number(newIndex));

		return true;
	}

	return false;
}

const QString &VarProperty::GetName() const
{
	return name;
}

bool VarProperty::SetName(const QString &newName)
{
	if (index < 0 && name != newName)
	{
		name = newName;

		return true;
	}

	return false;
}

VarProperty *VarProperty::TopParent()
{
	auto p = varParent;
	if (nullptr == p)
		return this;

	while (nullptr != p)
	{
		auto next_p = p->varParent;
		if (nullptr == next_p)
			return p;

		p = next_p;
	}

	return nullptr;
}

VarProperty *VarProperty::VarParent()
{
	return varParent;
}

VarProperty::VarChildren &VarProperty::GetChildren()
{
	return varChildren;
}

int VarProperty::GetChildrenCount() const
{
	return static_cast<int>(varChildren.size());
}

void VarProperty::SetValue(const QVariant &value)
{
	this->value = value;
	switch (type)
	{
		case Map:
		case List:
			for (auto child : varChildren)
			{
				child->SetValue(QVariant());
				child->setParent(nullptr);
				delete child;
			}
			varChildren.clear();
			break;

		case Value:
			break;
	}

	type = Value;
}

QVariant VarProperty::CreateVariant() const
{
	switch (type)
	{
		case List:
		{
			QVariantList list;

			for (auto child : varChildren)
			{
				list.append(child->CreateVariant());
			}

			return QVariant(list);
		}

		case Map:
		{
			QVariantMap map;

			for (auto child : varChildren)
			{
				map.insert(child->name, child->CreateVariant());
			}

			return QVariant(map);
		}

		default:
			break;
	}

	return value;
}

bool VarProperty::IsChildNameAvailable(const QString &name, VarProperty *skip) const
{
	for (auto prop : varChildren)
	{
		if (skip == prop)
			continue;

		if (prop->name == name)
			return true;
	}

	return false;
}

QtnPropertyBase *VarProperty::NewExtraProperty(QtnPropertySet *set, const QVariant &value,
											   const QString &key, int index, VarProperty *mapParent,
											   const RegisterPropertyCallback &registerProperty)
{
	QtnProperty *prop = nullptr;

	QString name(key);
	if (index >= 0)
		name = QString("[%1]").arg(QString::number(index));

	auto type = value.type();

	switch (type)
	{
		case QVariant::Map:
		{
			return NewExtraPropertySet(set, value.toMap(), mapParent, name, index, registerProperty);
		} break;

		case QVariant::List:
		{
			return NewExtraPropertyList(set, value.toList(), mapParent, name, index, registerProperty);
		} break;

		case QVariant::Int:
		{
			auto p = new QtnPropertyInt(set);

			p->setId(PID_EXTRA_INT);
			p->setValue(value.toInt());

			prop = p;
		} break;

		case QVariant::UInt:
		{
			auto p = new QtnPropertyUInt(set);

			p->setId(PID_EXTRA_UINT);
			p->setValue(value.toUInt());

			prop = p;
		} break;

		case QVariant::LongLong:
		case QVariant::ULongLong:
		case QVariant::Double:
		{
			auto p = new QtnPropertyDouble(set);

			p->setId(PID_EXTRA_FLOAT);
			p->setValue(value.toDouble());

			prop = p;
		} break;

		case QVariant::Bool:
		{
			auto p = new QtnPropertyBool(set);

			p->setId(PID_EXTRA_BOOL);
			p->setValue(value.toBool());

			prop = p;
		} break;

		default:
		{
			auto p = new QtnPropertyQString(set);

			p->setId(PID_EXTRA_STRING);
			p->setValue(value.toString());

			prop = p;
		} break;
	}

	if (nullptr != prop)
	{
		auto varprop = new VarProperty(prop, VarProperty::Value, name, index, value);
		if (nullptr != mapParent)
			mapParent->AddChild(varprop, index);

		prop->setName(name);

		registerProperty(prop);
	}

	return prop;
}

bool VarProperty::PropertyValueAccept(const QtnProperty *property, void *valueToAccept, QVariant *dest)
{
	switch (property->id())
	{
		case VarProperty::PID_EXTRA:
		case VarProperty::PID_EXTRA_STRING:
		case VarProperty::PID_EXTRA_INT:
		case VarProperty::PID_EXTRA_UINT:
		case VarProperty::PID_EXTRA_FLOAT:
		case VarProperty::PID_EXTRA_BOOL:
		{
			auto var_property = property->findChild<VarProperty *>(QString(), Qt::FindDirectChildrenOnly);

			QVariant value;

			if (nullptr != valueToAccept)
			{
				switch (property->id())
				{
					case PID_EXTRA_STRING:
						value = QVariant(*(QtnPropertyQString::ValueType *) valueToAccept);
						break;

					case PID_EXTRA_INT:
						value = QVariant(*(QtnPropertyInt::ValueType *) valueToAccept);
						break;

					case PID_EXTRA_UINT:
						value = QVariant(*(QtnPropertyUInt::ValueType *) valueToAccept);
						break;

					case PID_EXTRA_FLOAT:
						value = QVariant(*(QtnPropertyDouble::ValueType *) valueToAccept);
						break;

					case PID_EXTRA_BOOL:
						value = QVariant(*(QtnPropertyBool::ValueType *) valueToAccept);
						break;

					default:
						value = *(QVariant *) valueToAccept;
						break;
				}
			}

			var_property->ChangePropertyValue(value, dest);

			return true;
		}
	}

	return false;
}

QtnPropertySet *VarProperty::NewExtraPropertySet(QObject *parent, const QVariantMap &map,
												 VarProperty *mapParent, const QString &name,
												 int index, const RegisterPropertyCallback &registerProperty)
{
	auto set = new QtnPropertySet(parent);

	auto varprop = new VarProperty(set, VarProperty::Map, name, index, QVariant());
	if (nullptr != mapParent)
		mapParent->AddChild(varprop, index);
	mapParent = varprop;

	set->setId(PID_EXTRA);
	set->setName(name);

	for (auto it = map.begin(); it != map.end(); ++it)
	{
		auto &key = it.key();
		auto &value = it.value();

		NewExtraProperty(set, value, key, -1, mapParent, registerProperty);
	}

	return set;
}

QtnPropertySet *VarProperty::NewExtraPropertyList(QObject *parent, const QVariantList &list,
												  VarProperty *mapParent, const QString &name,
												  int index, const RegisterPropertyCallback &registerProperty)
{
	auto set = new QtnPropertySet(parent);

	auto varprop = new VarProperty(set, VarProperty::List, name, index, QVariant());
	if (nullptr != mapParent)
		mapParent->AddChild(varprop, index);
	mapParent = varprop;

	set->setId(PID_EXTRA);
	set->setName(name);

	int len = list.size();

	for (int i = 0; i < len; i++)
	{
		auto &value = list.at(i);

		NewExtraProperty(set, value, QString(), i, mapParent, registerProperty);
	}

	return set;
}
