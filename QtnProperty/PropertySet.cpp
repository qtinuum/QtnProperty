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

#include "PropertySet.h"

#include <QRegularExpression>
#include <QJsonObject>
#include <QDebug>

void qtnAddPropertyAsChild(
	QObject *parent, QtnPropertyBase *child, bool moveOwnership)
{
	QtnPropertySet *propertySet = qobject_cast<QtnPropertySet *>(parent);

	if (propertySet)
		propertySet->addChildProperty(child, moveOwnership);
}

void qtnRemovePropertyAsChild(QObject *parent, QtnPropertyBase *child)
{
	QtnPropertySet *propertySet = qobject_cast<QtnPropertySet *>(parent);

	if (propertySet)
		propertySet->removeChildProperty(child);
}

QtnPropertySet::QtnPropertySet(QObject *parent)
	: QtnPropertyBase(parent)
	, m_childrenOrder(NoSort)
{
}

QtnPropertySet::QtnPropertySet(
	SortOrder childrenOrder, const CompareFunc &compareFunc)
	: QtnPropertyBase(nullptr)
	, m_compareFunc(compareFunc)
	, m_childrenOrder(childrenOrder)
{
}

QtnPropertySet::~QtnPropertySet()
{
	clearChildProperties();
}

int QtnPropertySet::compareByName(
	const QtnPropertyBase *a, const QtnPropertyBase *b)
{
	return QString::localeAwareCompare(a->name(), b->name());
}

QList<QtnPropertyBase *> QtnPropertySet::findChildProperties(
	QString name, Qt::FindChildOptions options)
{
	QList<QtnPropertyBase *> result;

	// normilize name
	name = name.trimmed();

	// if name is dot separated property path
	if (name.contains('.'))
	{
		QString nameHead = name.section('.', 0, 0);

		if (nameHead.isEmpty())
			return result;

		QString nameTail = name.section('.', 1);

		if (nameTail.isEmpty())
			return result;

		QList<QtnPropertyBase *> headResult =
			findChildProperties(nameHead, options);

		for (auto headProperty : headResult)
		{
			QtnPropertySet *headPropertySet = headProperty->asPropertySet();

			if (!headPropertySet)
				continue;

			result.append(
				headPropertySet->findChildProperties(nameTail, options));
		}
	} else
	{
		for (auto childProperty : m_childProperties)
		{
			if (childProperty->name() == name)
				result.append(childProperty);
		}

		if (options & Qt::FindChildrenRecursively)
		{
			for (auto childProperty : m_childProperties)
			{
				QtnPropertySet *propertySet = childProperty->asPropertySet();

				if (propertySet)
					propertySet->findChildPropertiesRecursive(name, result);
			}
		}
	}

	return result;
}

QList<QtnPropertyBase *> QtnPropertySet::findChildProperties(
	const QRegularExpression &re, Qt::FindChildOptions options)
{
	QList<QtnPropertyBase *> result;

	for (auto childProperty : m_childProperties)
	{
		if (re.match(childProperty->name()).isValid())
			result.append(childProperty);
	}

	if (options & Qt::FindChildrenRecursively)
	{
		for (auto childProperty : m_childProperties)
		{
			QtnPropertySet *propertySet = childProperty->asPropertySet();

			if (propertySet)
				propertySet->findChildPropertiesRecursive(re, result);
		}
	}

	return result;
}

QtnPropertyBase *QtnPropertySet::findChildProperty(QtnPropertyID id)
{
	for (auto childProperty : m_childProperties)
	{
		if (childProperty->id() == id)
			return childProperty;
	}

	return nullptr;
}

void QtnPropertySet::clearChildProperties()
{
	if (m_childProperties.isEmpty())
		return;

	emit propertyWillChange(
		QtnPropertyChangeReasonChildPropertyRemove, nullptr, 0);

	// Original list is cleared to avoid interference with property destructors,
	// where properties are removed from the parent's list.
	auto childProperties = std::move(m_childProperties);
	for (auto p : childProperties)
	{
		if (p->parent() == this)
			delete p;
	}

	emit propertyDidChange(QtnPropertyChangeReasonChildPropertyRemove);
}

bool QtnPropertySet::addChildProperty(
	QtnPropertyBase *childProperty, bool moveOwnership, int index)
{
	Q_CHECK_PTR(childProperty);

	emit propertyWillChange(QtnPropertyChangeReasonChildPropertyAdd,
		QtnPropertyValuePtr(childProperty), qMetaTypeId<QtnPropertyBase *>());

	switch (m_childrenOrder)
	{
		case NoSort:
			break;

		case Ascend:
		case Descend:
			index = 0;
			for (int i = m_childProperties.size() - 1; i >= 0; --i)
			{
				auto p = m_childProperties.at(i);
				int compare = m_compareFunc(childProperty, p);
				if (compare == 0 ||
					(m_childrenOrder == Ascend && compare > 0) ||
					(m_childrenOrder == Descend && compare < 0))
				{
					index = i + 1;
					break;
				}
			}
			break;
	}

	if (index < 0)
		m_childProperties.append(childProperty);
	else
		m_childProperties.insert(index, childProperty);

	if (moveOwnership)
		childProperty->setParent(this);

	emit propertyDidChange(QtnPropertyChangeReasonChildPropertyAdd);

	childProperty->setStateInherited(state());
	return true;
}

bool QtnPropertySet::removeChildProperty(QtnPropertyBase *childProperty)
{
	Q_CHECK_PTR(childProperty);

	int childPropertyIndex = m_childProperties.indexOf(childProperty);

	if (childPropertyIndex < 0)
		return false;

	emit propertyWillChange(QtnPropertyChangeReasonChildPropertyRemove,
		QtnPropertyValuePtr(childProperty), qMetaTypeId<QtnPropertyBase *>());

	m_childProperties.erase(m_childProperties.begin() + childPropertyIndex);

	if (childProperty->parent() == this)
		childProperty->setParent(nullptr);

	emit propertyDidChange(QtnPropertyChangeReasonChildPropertyRemove);

	return true;
}

QtnPropertySet *QtnPropertySet::createNew(QObject *parentForNew) const
{
	return createNewImpl(parentForNew);
}

QtnPropertySet *QtnPropertySet::createCopy(QObject *parentForCopy) const
{
	return createCopyImpl(parentForCopy);
}

bool QtnPropertySet::copyValues(
	QtnPropertySet *propertySetCopyFrom, QtnPropertyState ignoreMask)
{
	return copyValuesImpl(propertySetCopyFrom, ignoreMask);
}

QtnPropertySet *QtnPropertySet::asPropertySet()
{
	return this;
}

const QtnPropertySet *QtnPropertySet::asPropertySet() const
{
	return this;
}

void QtnPropertySet::doReset(QtnPropertyChangeReason reason)
{
	Q_ASSERT(reason & QtnPropertyChangeReasonResetValue);
	for (auto &p : childProperties())
	{
		p->reset(reason);
	}
}

void QtnPropertySet::updateStateInherited(bool force)
{
	for (auto childProperty : m_childProperties)
	{
		childProperty->setStateInherited(state(), force);
	}
}

QtnPropertySet *QtnPropertySet::createNewImpl(QObject *parentForNew) const
{
	Q_UNUSED(parentForNew);
	return nullptr;
}

QtnPropertySet *QtnPropertySet::createCopyImpl(QObject *parentForCopy) const
{
	Q_UNUSED(parentForCopy);
	return nullptr;
}

bool QtnPropertySet::copyValuesImpl(
	QtnPropertySet *propertySetCopyFrom, QtnPropertyState ignoreMask)
{
	Q_UNUSED(propertySetCopyFrom);
	Q_UNUSED(ignoreMask);
	return false;
}

bool QtnPropertySet::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
    static QRegularExpression parserLine(QStringLiteral("^\\s*([^=]+)=(.*)$"));

    QStringList lines = str.split(QChar::LineFeed, Qt::SkipEmptyParts);

	if (lines.isEmpty())
		return true;

	bool ok = true;

	for (const auto &line : lines)
	{
        QRegularExpressionMatch match = parserLine.match(line);
        if (!match.hasMatch())
		{
			qDebug() << "Cannot parse string: " << line;
			ok = false;
			continue;
		}

        QStringList params = match.capturedTexts();
		if (params.size() != 3)
		{
			qDebug() << "Cannot parse string: " << line;
			ok = false;
			continue;
		}

		QString propertyPath = params[1];
		QString propertyStrValue = params[2];

		QList<QtnPropertyBase *> subProperties =
			findChildProperties(propertyPath, Qt::FindChildrenRecursively);
		if (subProperties.size() != 1)
		{
			qDebug() << "Ambiguous property path: " << propertyPath;
			ok = false;
			continue;
		}

		if (subProperties[0]->state() & QtnPropertyStateNonSerialized)
			continue;

		propertyStrValue = propertyStrValue.trimmed();
		if (propertyStrValue.startsWith('"') && propertyStrValue.endsWith('"'))
		{
			propertyStrValue =
				propertyStrValue.mid(1, propertyStrValue.length() - 2);
		}

		if (!subProperties[0]->fromStr(propertyStrValue, reason))
		{
			qDebug() << QString(
				"Cannot convert property %1<%2> from string \"%3\"")
							.arg(subProperties[0]->name(),
								subProperties[0]->metaObject()->className(),
								propertyStrValue);
			ok = false;
			continue;
		}
	}

	return ok;
}

bool QtnPropertySet::fromJson(
	const QJsonObject &jsonObject, QtnPropertyChangeReason reason)
{
	bool ok = true;

	for (auto it = jsonObject.begin(), end = jsonObject.end(); it != end; ++it)
	{
		if (it.value().type() != QJsonValue::Object)
		{
			qDebug() << "Json object expected";
			ok = false;
			continue;
		}

		QString cppName = it.key();
		auto childProperties =
			findChildProperties(cppName, Qt::FindDirectChildrenOnly);
		if (childProperties.isEmpty())
		{
			qDebug() << "Cannot find property " << cppName;
			ok = false;
			continue;
		} else if (childProperties.size() > 1)
		{
			qDebug() << "Ambiguous property " << cppName;
			ok = false;
			continue;
		}

		if (childProperties[0]->state() & QtnPropertyStateNonSerialized)
			continue;

		auto childPropertySet = childProperties[0]->asPropertySet();
		if (childPropertySet)
		{
			if (!childPropertySet->fromJson(it.value().toObject(), reason))
			{
				qDebug() << "Cannot load \"" << childPropertySet->name()
						 << "\" from JSON";
				ok = false;
			}
		} else
		{
			auto childProperty = childProperties[0]->asProperty();
			if (childProperty)
			{
				auto jsonProperty = it.value().toObject();
				if (!jsonProperty.contains("value"))
				{
					qDebug() << "Cannot parse \"value\" attribute";
					ok = false;
					continue;
				}

				QString propertyValue = jsonProperty.value("value").toString();
				if (!childProperty->fromStr(propertyValue, reason))
				{
					qDebug() << "Cannot convert value" << propertyValue
							 << "to property" << childProperty->name();
					ok = false;
				}
			} else
			{
				Q_ASSERT(false && "Cannot recognize property type");
				ok = false;
			}
		}
	}

	return ok;
}

bool QtnPropertySet::toJson(QJsonObject &jsonObject) const
{
	bool ok = true;

	for (auto childPropertyBase : childProperties())
	{
		if (childPropertyBase->state() & QtnPropertyStateNonSerialized)
			continue;

		QJsonObject jsonSubObject;

		auto childPropertySet = childPropertyBase->asPropertySet();
		if (childPropertySet)
		{
			if (!childPropertySet->toJson(jsonSubObject))
			{
				qDebug() << "Cannot save \"" << childPropertySet->name()
						 << "\" to JSON";
				ok = false;
				continue;
			}
		} else
		{
			auto childProperty = childPropertyBase->asProperty();
			if (childProperty)
			{
				QString value;
				if (!childProperty->toStr(value))
				{
					qDebug() << "Cannot convert property \""
							 << childProperty->name() << "\" to QString";
					ok = false;
					continue;
				}

				jsonSubObject.insert("value", value);
			} else
			{
				Q_ASSERT(false && "Cannot recognize property type");
				ok = false;
				continue;
			}
		}

		jsonObject.insert(childPropertyBase->name(), jsonSubObject);
	}

	return ok;
}

bool QtnPropertySet::toStrImpl(QString &str) const
{
	return toStrWithPrefix(str, QString());
}

bool QtnPropertySet::fromVariantImpl(
	const QVariant &v, QtnPropertyChangeReason reason)
{
	if (!v.isValid() || v.type() == QVariant::Map)
	{
		return fromJson(QJsonObject::fromVariantMap(v.toMap()), reason);
	}
	return false;
}

bool QtnPropertySet::toVariantImpl(QVariant &v) const
{
	QJsonObject json;
	bool ok = toJson(json);
	if (ok)
		v = json.toVariantMap();
	return ok;
}

bool QtnPropertySet::loadImpl(QDataStream &stream)
{
	if (!QtnPropertyBase::loadImpl(stream))
		return false;

	if (stream.status() != QDataStream::Ok)
		return false;

	quint8 version = 0;
	stream >> version;
	// version incorrect
	if (version != STORAGE_VERSION)
		return false;

	forever
	{
		QtnPropertyID id = QtnPropertyIDInvalid;
		stream >> id;

		// no child properties any more
		if (id == QtnPropertyIDInvalid)
			break;

		QtnPropertyBase *childProperty = findChildProperty(id);
		if (!childProperty)
		{
			// cannot find subproperty -> skip
			if (!skipLoad(stream))
				return false;
			continue;
		}

		if (childProperty->state() & QtnPropertyStateNonSerialized)
		{
			// should not load such subproperty
			if (!skipLoad(stream))
				return false;
			continue;
		}

		if (!childProperty->load(stream))
			return false;
	}

	return stream.status() == QDataStream::Ok;
}

bool QtnPropertySet::saveImpl(QDataStream &stream) const
{
	if (!QtnPropertyBase::saveImpl(stream))
		return false;

	if (stream.status() != QDataStream::Ok)
		return false;

	// for compatibility
	stream << STORAGE_VERSION;

	for (auto childProperty : m_childProperties)
	{
		if (childProperty->state() & QtnPropertyStateNonSerialized)
			continue;

		if (childProperty->id() == QtnPropertyIDInvalid)
		{
			// serializable properties should have unique ids
			Q_ASSERT(false && "serializable properties should have unique ids");
			continue;
		}

		// save child property id
		stream << childProperty->id();
		// save child property
		if (!childProperty->save(stream))
			return false;
	}

	// mark end of children list
	stream << QtnPropertyIDInvalid;

	return stream.status() == QDataStream::Ok;
}

void QtnPropertySet::findChildPropertiesRecursive(
	const QString &name, QList<QtnPropertyBase *> &result)
{
	for (auto childProperty : m_childProperties)
	{
		if (childProperty->name() == name)
			result.append(childProperty);

		QtnPropertySet *propertySet = childProperty->asPropertySet();

		if (propertySet)
			propertySet->findChildPropertiesRecursive(name, result);
	}
}

void QtnPropertySet::findChildPropertiesRecursive(
	const QRegularExpression &re, QList<QtnPropertyBase *> &result)
{
	for (auto childProperty : m_childProperties)
	{
		if (re.match(childProperty->name()).isValid())
			result.append(childProperty);

		QtnPropertySet *propertySet = childProperty->asPropertySet();

		if (propertySet)
			propertySet->findChildPropertiesRecursive(re, result);
	}
}

bool QtnPropertySet::toStrWithPrefix(QString &str, const QString &prefix) const
{
	for (auto childPropertyBase : m_childProperties)
	{
		if (childPropertyBase->state() & QtnPropertyStateNonSerialized)
			continue;
		QtnProperty *childProperty = childPropertyBase->asProperty();

		if (childProperty)
		{
			QString strValue;

			if (!childProperty->toStr(strValue))
				return false;

			str.append(QStringLiteral("%1%2 = %3\n")
						   .arg(prefix, childProperty->name(), strValue));
		} else
		{
			auto childPropertySet = childPropertyBase->asPropertySet();

			if (childPropertySet)
			{
				if (!childPropertySet->toStrWithPrefix(str,
						QStringLiteral("%1%2.").arg(
							prefix, childPropertySet->name())))
					return false;
			} else
			{
				// neither property no propertyset
				Q_UNREACHABLE();
			}
		}
	}

	return true;
}
