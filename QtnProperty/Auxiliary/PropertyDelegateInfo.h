/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#ifndef QTN_PROPERTY_DELEGATE_INFO_H
#define QTN_PROPERTY_DELEGATE_INFO_H

#include "QtnProperty/CoreAPI.h"
#include <QMap>
#include <QVariant>

struct QTN_IMPORT_EXPORT QtnPropertyDelegateInfo
{
	QByteArray name;
	using Attributes = QMap<QByteArray, QVariant>;
	Attributes attributes;

	QtnPropertyDelegateInfo() = default;
	QtnPropertyDelegateInfo(const QtnPropertyDelegateInfo &other);
	QtnPropertyDelegateInfo(
		const QByteArray &name, const Attributes &attributes = Attributes());

	template <typename T>
	inline T getAttribute(
		const QByteArray &attributeName, const T &defaultValue = T()) const
	{
		auto it = attributes.find(attributeName);

		if (it == attributes.end())
			return defaultValue;

		return it.value().value<T>();
	}

	template <typename T>
	inline bool loadAttribute(const QByteArray &name, T &to) const
	{
		auto it = attributes.find(name);

		if (it == attributes.end())
			return false;

		to = it.value().value<T>();
		return true;
	}

	template <typename OBJ_T, typename ATTR_T_RET, typename ATTR_T_ARG>
	inline void storeAttributeValue(const QByteArray &name, OBJ_T *to,
		ATTR_T_RET (OBJ_T::*get)() const, void (OBJ_T::*set)(ATTR_T_ARG)) const
	{
		Q_ASSERT(to);
		(to->*set)(getAttribute(name, (to->*get)()));
	}
};

struct QTN_IMPORT_EXPORT QtnSubPropertyInfo
{
	int id;
	QString key;
	QByteArray displayNameAttr;
	QByteArray descriptionAttr;
};

#endif // QTN_PROPERTY_DELEGATE_INFO_H
