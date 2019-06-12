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

#ifndef QTN_PROPERTY_SET_H
#define QTN_PROPERTY_SET_H

#include "Property.h"

class QJsonObject;

class QTN_IMPORT_EXPORT QtnPropertySet : public QtnPropertyBase
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnPropertySet)

public:
	enum SortOrder
	{
		NoSort,
		Ascend,
		Descend,
	};

	static int compareByName(
		const QtnPropertyBase *a, const QtnPropertyBase *b);

	using CompareFunc =
		std::function<int(const QtnPropertyBase *, const QtnPropertyBase *)>;

	explicit QtnPropertySet(QObject *parent = nullptr);
	explicit QtnPropertySet(SortOrder childrenOrder,
		const CompareFunc &compareFunc = compareByName);
	virtual ~QtnPropertySet() override;

	inline SortOrder childrenOrder() const;
	inline const CompareFunc &compareFunc() const;

public slots:
	// sub properties
	inline bool hasChildProperties() const;
	inline const QList<QtnPropertyBase *> &childProperties() const;

	QList<QtnPropertyBase *> findChildProperties(QString name,
		Qt::FindChildOptions options = Qt::FindChildrenRecursively);

	QList<QtnPropertyBase *> findChildProperties(const QRegularExpression &re,
		Qt::FindChildOptions options = Qt::FindChildrenRecursively);

	QtnPropertyBase *findChildProperty(QtnPropertyID id);
	void clearChildProperties();
	bool addChildProperty(QtnPropertyBase *childProperty,
		bool moveOwnership = true, int index = -1);
	bool removeChildProperty(QtnPropertyBase *childProperty);

	// cloning
	QtnPropertySet *createNew(QObject *parentForNew) const;
	QtnPropertySet *createCopy(QObject *parentForCopy) const;

	// copy values
	bool copyValues(QtnPropertySet *propertySetCopyFrom,
		QtnPropertyState ignoreMask = QtnPropertyStateNone);

	// JSON support
	bool fromJson(const QJsonObject &jsonObject,
		QtnPropertyChangeReason reason = QtnPropertyChangeReasonChildren);
	bool toJson(QJsonObject &jsonObject) const;

public:
	// casts
	virtual QtnPropertySet *asPropertySet() override;
	virtual const QtnPropertySet *asPropertySet() const override;

protected:
	virtual void doReset(QtnPropertyChangeReason reason) override;
	virtual void updateStateInherited(bool force) override;

	// cloning implementation
	virtual QtnPropertySet *createNewImpl(QObject *parentForNew) const;
	virtual QtnPropertySet *createCopyImpl(QObject *parentForCopy) const;

	// copy values
	virtual bool copyValuesImpl(
		QtnPropertySet *propertySetCopyFrom, QtnPropertyState ignoreMask);

	// string conversion implementation
	virtual bool fromStrImpl(
		const QString &str, QtnPropertyChangeReason reason) override;
	virtual bool toStrImpl(QString &str) const override;

	virtual bool fromVariantImpl(
		const QVariant &v, QtnPropertyChangeReason reason) override;
	virtual bool toVariantImpl(QVariant &v) const override;

	// serialization implementation
	virtual bool loadImpl(QDataStream &stream) override;
	virtual bool saveImpl(QDataStream &stream) const override;

private:
	void findChildPropertiesRecursive(
		const QString &name, QList<QtnPropertyBase *> &result);
	void findChildPropertiesRecursive(
		const QRegularExpression &re, QList<QtnPropertyBase *> &result);

	bool toStrWithPrefix(QString &str, const QString &prefix) const;

private:
	CompareFunc m_compareFunc;
	QList<QtnPropertyBase *> m_childProperties;

	SortOrder m_childrenOrder;
};

QtnPropertySet::SortOrder QtnPropertySet::childrenOrder() const
{
	return m_childrenOrder;
}

const QtnPropertySet::CompareFunc &QtnPropertySet::compareFunc() const
{
	return m_compareFunc;
}

bool QtnPropertySet::hasChildProperties() const
{
	return !m_childProperties.empty();
}

const QList<QtnPropertyBase *> &QtnPropertySet::childProperties() const
{
	return m_childProperties;
}

Q_DECLARE_METATYPE(QtnPropertySet *)

template <typename T>
T *qtnCreateProperty(QtnPropertySet *parent, QString name)
{
	auto property = new T(parent);
	property->setName(name);
	parent->addChildProperty(property);
	return property;
}

template <typename T>
T *qtnCreateProperty(QtnPropertySet *parent)
{
	auto property = new T(parent);
	parent->addChildProperty(property);
	return property;
}

QTN_IMPORT_EXPORT void qtnAddPropertyAsChild(
	QObject *parent, QtnPropertyBase *child, bool moveOwnership);
QTN_IMPORT_EXPORT void qtnRemovePropertyAsChild(
	QObject *parent, QtnPropertyBase *child);

#endif // QTN_PROPERTY_SET_H
