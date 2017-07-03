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

#include <QObject>
#include <QVariant>

#include <functional>
#include <vector>

class QtnPropertyBase;
class QtnProperty;
class QtnPropertySet;

class QTN_IMPORT_EXPORT VarProperty : public QObject
{
	Q_OBJECT

public:
	enum Type
	{
		Value,
		List,
		Map
	};

	typedef std::function<void (QtnProperty *)> RegisterPropertyCallback;
	typedef std::vector<VarProperty *> VarChildren;

	VarProperty(
		QObject *parent,
		Type type,
		const QString &name,
		int index,
		const QVariant &value);

	void ChangePropertyValue(const QVariant &value, QVariant *dest = nullptr);
	void RemoveFromParent();
	VarProperty *Duplicate(VarProperty *child, int newIndex);
	VarProperty *Duplicate(VarProperty *child, const QString &newName);

	VarProperty *Duplicate(int new_index);
	VarProperty *Duplicate(const QString &new_name);

	VarProperty *AddChild(VarProperty *child, int index = -1);
	VarProperty *AddChild(int index, const QVariant &value);
	VarProperty *AddChild(const QString &name, const QVariant &value);

	static Type GetTypeFromValue(const QVariant &value);
	Type GetType() const;
	QVariant::Type GetVariantType() const;

	int GetIndex() const;
	bool SetIndex(int newIndex);

	const QString &GetName() const;
	bool SetName(const QString &newName);

	VarProperty *TopParent();
	VarProperty *VarParent();
	VarChildren &GetChildren();
	int GetChildrenCount() const;

	void SetValue(const QVariant &value);
	QVariant CreateVariant() const;
	bool IsChildNameAvailable(const QString &name, VarProperty *skip) const;

	static QtnPropertyBase *NewExtraProperty(
		QtnPropertySet *set, const QVariant &value,
		const QString &key, int index, VarProperty *mapParent,
		const RegisterPropertyCallback &registerProperty);

	static bool PropertyValueAccept(
		const QtnProperty *property,
		void *valueToAccept, QVariant *dest = nullptr);

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
	static QtnPropertySet *NewExtraPropertySet(
		QObject *parent, const QVariantMap &map,
		VarProperty *mapParent, const QString &name, int index,
		const RegisterPropertyCallback &registerProperty);
	static QtnPropertySet *NewExtraPropertyList(
		QObject *parent, const QVariantList &list, VarProperty *mapParent,
		const QString &name, int index,
		const RegisterPropertyCallback &registerProperty);

	VarProperty *varParent;
	VarChildren varChildren;
	QVariant value;
	QString name;
	int index;
	Type type;
};
