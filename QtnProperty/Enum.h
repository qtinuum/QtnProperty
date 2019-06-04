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

#ifndef QTN_ENUM_H
#define QTN_ENUM_H

#include "CoreAPI.h"
#include <QVector>
#include <QSharedPointer>
#include <QMetaEnum>

typedef qint32 QtnEnumValueType;

enum QtnEnumValueStateFlag
{
	QtnEnumValueStateNone = 0x0000,
	QtnEnumValueStateHidden = 0x0001,
	QtnEnumValueStateObsolete = 0x0002,
	QtnEnumValueStateInvalid = 0x0004
};

Q_DECLARE_FLAGS(QtnEnumValueState, QtnEnumValueStateFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtnEnumValueState)

class QTN_IMPORT_EXPORT QtnEnumValueInfo
{
public:
	QtnEnumValueInfo();

	QtnEnumValueInfo(QtnEnumValueType value, const QString &name,
		QtnEnumValueState state = QtnEnumValueStateNone);

	QtnEnumValueInfo(QtnEnumValueType value, const QString &name,
		const QString &displayName,
		QtnEnumValueState state = QtnEnumValueStateNone);

	inline QtnEnumValueType value() const;
	inline void setValue(QtnEnumValueType value);
	inline const QString &name() const;
	inline const QString &displayName() const;
	inline QtnEnumValueState state() const;

private:
	QtnEnumValueType m_value;
	QString m_name;
	QString m_displayName;
	QtnEnumValueState m_state;
};

QtnEnumValueType QtnEnumValueInfo::value() const
{
	return m_value;
}

void QtnEnumValueInfo::setValue(QtnEnumValueType value)
{
	m_value = value;
}

const QString &QtnEnumValueInfo::name() const
{
	return m_name;
}

const QString &QtnEnumValueInfo::displayName() const
{
	return m_displayName;
}

QtnEnumValueState QtnEnumValueInfo::state() const
{
	return m_state;
}

class QTN_IMPORT_EXPORT QtnEnumInfo
{
public:
	QtnEnumInfo();
	QtnEnumInfo(const QString &name);
	QtnEnumInfo(const QString &name, QVector<QtnEnumValueInfo> &staticValues);
	QtnEnumInfo(
		const QString &name, const QVector<QtnEnumValueInfo> &staticValues);

	inline const QString &name() const;

	static QtnEnumInfo withMetaEnum(
		const QMetaEnum &metaEnum, bool translate = false);
	template <typename T>
	static inline QtnEnumInfo withEnum(bool translate = false)
	{
		return withMetaEnum(QMetaEnum::fromType<T>(), translate);
	}

	template <typename Pred>
	bool forEachEnumValue(Pred pred) const
	{
		for (const auto &value : m_values)
		{
			if (!pred(value))
				return false;
		}

		return true;
	}

	const QtnEnumValueInfo *findByValue(QtnEnumValueType value) const;
	const QtnEnumValueInfo *findByName(const QString &name) const;
	const QtnEnumValueInfo *findByDisplayName(const QString &displayName,
		Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

	const QtnEnumValueInfo *fromStr(const QString &str) const;
	bool toStr(QString &str, const QtnEnumValueInfo *value) const;
	bool toStr(QString &str, QtnEnumValueType value) const;

	Qt::CaseSensitivity getCaseSensitivity() const;
	void setCaseSensitivity(Qt::CaseSensitivity value);

	inline QVector<QtnEnumValueInfo> &getVector();
	inline const QVector<QtnEnumValueInfo> &getVector() const;

private:
	Qt::CaseSensitivity m_case_sensitivity;
	QString m_name;
	QVector<QtnEnumValueInfo> m_values;
};

const QString &QtnEnumInfo::name() const
{
	return m_name;
}

QVector<QtnEnumValueInfo> &QtnEnumInfo::getVector()
{
	return m_values;
}

const QVector<QtnEnumValueInfo> &QtnEnumInfo::getVector() const
{
	return m_values;
}

#endif // QTN_ENUM_H
