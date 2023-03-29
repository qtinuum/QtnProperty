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

#include "Enum.h"

#include <QStringList>
#include <QCoreApplication>

QtnEnumInfo::QtnEnumInfo()
	: m_case_sensitivity(Qt::CaseInsensitive)
{
}

QtnEnumInfo::QtnEnumInfo(const QString &name)
	: m_case_sensitivity(Qt::CaseInsensitive)
	, m_name(name)
{
}

QtnEnumInfo::QtnEnumInfo(
	const QString &name, QVector<QtnEnumValueInfo> &staticValues)
	: m_case_sensitivity(Qt::CaseInsensitive)
	, m_name(name)
{
	m_values.swap(staticValues);
}

QtnEnumInfo::QtnEnumInfo(
	const QString &name, const QVector<QtnEnumValueInfo> &staticValues)
	: m_case_sensitivity(Qt::CaseSensitive)
	, m_name(name)
	, m_values(staticValues)
{
}

QtnEnumInfo QtnEnumInfo::withMetaEnum(const QMetaEnum &metaEnum, bool translate)
{
	QtnEnumInfo enumInfo(QLatin1String(metaEnum.scope()) +
		QStringLiteral("::") + QLatin1String(metaEnum.name()));
	auto &vec = enumInfo.getVector();
	int count = metaEnum.keyCount();
	vec.reserve(count);
	for (int i = 0; i < count; i++)
	{
		const char *key = metaEnum.key(i);
		QString keyStr = QLatin1String(key);
		vec.append(QtnEnumValueInfo(metaEnum.value(i), keyStr,
			translate ? QCoreApplication::translate(
							metaEnum.scope(), key, metaEnum.name())
					  : keyStr));
	}

	return enumInfo;
}

const QtnEnumValueInfo *QtnEnumInfo::findByValue(QtnEnumValueType value) const
{
	const QtnEnumValueInfo *result = nullptr;

	forEachEnumValue(
		[&result, value](const QtnEnumValueInfo &enumValue) -> bool {
			if (enumValue.value() == value)
			{
				result = &enumValue;
				return false;
			}

			return true;
		});

	return result;
}

const QtnEnumValueInfo *QtnEnumInfo::findByName(const QString &name) const
{
	const QtnEnumValueInfo *result = nullptr;

	forEachEnumValue([&result, &name, this](
						 const QtnEnumValueInfo &enumValue) -> bool {
		if (QString::compare(enumValue.name(), name, m_case_sensitivity) == 0)
		{
			result = &enumValue;
			return false;
		}

		return true;
	});

	return result;
}

const QtnEnumValueInfo *QtnEnumInfo::findByDisplayName(
	const QString &displayName, Qt::CaseSensitivity cs) const
{
	const QtnEnumValueInfo *result = nullptr;

	forEachEnumValue(
		[&result, &displayName, cs](const QtnEnumValueInfo &enumValue) -> bool {
			if (QString::compare(enumValue.displayName(), displayName, cs) == 0)
			{
				result = &enumValue;
				return false;
			}

			return true;
		});

	return result;
}

const QtnEnumValueInfo *QtnEnumInfo::fromStr(const QString &str) const
{
	return findByName(str.trimmed());
}

bool QtnEnumInfo::toStr(QString &str, const QtnEnumValueInfo *value) const
{
	if (!value)
		return false;

	str = value->name();
	return true;
}

QtnEnumValueInfo::QtnEnumValueInfo()
	: m_value(0)
	, m_state(QtnEnumValueStateInvalid)
{
}

QtnEnumValueInfo::QtnEnumValueInfo(
	QtnEnumValueType value, const QString &name, QtnEnumValueState state)
	: QtnEnumValueInfo(value, name, name, state)
{
}

QtnEnumValueInfo::QtnEnumValueInfo(QtnEnumValueType value, const QString &name,
	const QString &displayName, QtnEnumValueState state)
	: m_value(value)
	, m_name(name)
	, m_displayName(displayName)
	, m_state(state)
{
	if (displayName.isEmpty())
		m_displayName = name;
}

bool QtnEnumInfo::toStr(QString &str, QtnEnumValueType value) const
{
	return toStr(str, findByValue(value));
}
