#pragma once

#include <QObject>
#include <QVariant>

#include <functional>
#include <vector>

class QtnPropertyBase;
class QtnProperty;
class QtnPropertySet;

class VarProperty : public QObject
{
public:
	enum Type
	{
		Value,
		List,
		Map
	};

	typedef std::function<void (QtnProperty *)> RegisterPropertyCallback;
	typedef std::vector<VarProperty *> VarChildren;

	VarProperty(QObject *parent,
				Type type,
				const QString &name,
				int index,
				const QVariant &value);

	void ChangePropertyValue(const QVariant &value, QVariant *dest = nullptr);
	void RemoveFromParent();
	VarProperty *Duplicate(VarProperty *child, int new_index);
	VarProperty *Duplicate(VarProperty *child, const QString &new_name);

	VarProperty *Duplicate(int new_index);
	VarProperty *Duplicate(const QString &new_name);

	VarProperty *AddChild(VarProperty *child, int index = -1);
	VarProperty *AddChild(int index, const QVariant &value);
	VarProperty *AddChild(const QString &name, const QVariant &value);

	static Type GetTypeFromValue(const QVariant &value);
	Type GetType() const;
	QVariant::Type GetVariantType() const;

	int GetIndex() const;
	bool SetIndex(int new_index);

	const QString &GetName() const;
	bool SetName(const QString &new_name);

	VarProperty *TopParent();
	VarProperty *VarParent();
	VarChildren &GetChildren();
	int GetChildrenCount() const;

	QVariant CreateVariant() const;
	bool IsChildNameAvailable(const QString &name, VarProperty *skip) const;

	static QtnPropertyBase *NewExtraProperty(QtnPropertySet *set, const QVariant &value,
											 const QString &key, int index, VarProperty *map_parent,
											 const RegisterPropertyCallback &register_property);

	static bool PropertyValueAccept(const QtnProperty *property, void *valueToAccept, QVariant *dest = nullptr);

	enum
	{
		PID_EXTRA = 1,

		PID_EXTRA_STRING,
		PID_EXTRA_INT,
		PID_EXTRA_UINT,
		PID_EXTRA_FLOAT,
		PID_EXTRA_BOOL,

		PID_EXTRA_TOTAL
	};

private:
	static QtnPropertySet *NewExtraPropertySet(QObject *parent, const QVariantMap &map,
											   VarProperty *map_parent, const QString &name, int index,
									const RegisterPropertyCallback &register_property);
	static QtnPropertySet *NewExtraPropertyList(QObject *parent, const QVariantList &list, VarProperty *map_parent,
												const QString &name, int index,
									 const RegisterPropertyCallback &register_property);

	VarProperty *var_parent;
	VarChildren var_children;
	QVariant value;
	QString name;
	int index;
	Type type;
};
