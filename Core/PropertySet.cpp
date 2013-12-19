/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
 */

#include "PropertySet.h"
#include <QRegularExpression>

namespace Qtinuum
{

void addPropertyAsChild(QObject *parent, PropertyBase* child, bool moveOwnership)
{
    PropertySet* propertySet = qobject_cast<PropertySet*>(parent);
    if (propertySet)
        propertySet->addChildProperty(child, moveOwnership);
}

void removePropertyAsChild(QObject* parent, PropertyBase* child)
{
    PropertySet* propertySet = qobject_cast<PropertySet*>(parent);
    if (propertySet)
        propertySet->removeChildProperty(child);
}

void connectChildProperty(PropertySet* masterProperty, PropertyBase* childProperty)
{
    QObject::connect(childProperty, &PropertyBase::propertyWillChange, masterProperty, &PropertySet::childPropertyWillChange);
    QObject::connect(childProperty, &PropertyBase::propertyDidChange, masterProperty, &PropertySet::childPropertyDidChange);
}

void disconnectChildProperty(PropertySet* masterProperty, PropertyBase* childProperty)
{
    QObject::disconnect(childProperty, &PropertyBase::propertyWillChange, masterProperty, &PropertySet::childPropertyWillChange);
    QObject::disconnect(childProperty, &PropertyBase::propertyDidChange, masterProperty, &PropertySet::childPropertyDidChange);
}

PropertySet::PropertySet(QObject* parent)
    : PropertyBase(parent)
{
}

PropertySet::~PropertySet()
{
    clearChildProperties();
}

QList<PropertyBase*> PropertySet::findChildProperties(QString name, Qt::FindChildOptions options)
{
    QList<PropertyBase*> result;

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

        QList<PropertyBase*> headResult = findChildProperties(nameHead, options);
        foreach (PropertyBase* headProperty, headResult)
        {
            PropertySet* headPropertySet = headProperty->asPropertySet();
            if (!headPropertySet)
                continue;

            result.append(headPropertySet->findChildProperties(nameTail, options));
        }
    }
    else
    {
        foreach(PropertyBase* childProperty, m_childProperties)
        {
            if (childProperty->name() == name)
                result.append(childProperty);
        }

        if (options & Qt::FindChildrenRecursively)
        {
            foreach(PropertyBase* childProperty, m_childProperties)
            {
                PropertySet* propertySet = childProperty->asPropertySet();
                if (propertySet)
                    propertySet->findChildPropertiesRecursive(name, result);
            }
        }
    }

    return result;
}

QList<PropertyBase*> PropertySet::findChildProperties(const QRegularExpression& re, Qt::FindChildOptions options)
{
    QList<PropertyBase*> result;

    foreach(PropertyBase* childProperty, m_childProperties)
    {
        if (re.match(childProperty->name()).isValid())
            result.append(childProperty);
    }

    if (options & Qt::FindChildrenRecursively)
    {
        foreach(PropertyBase* childProperty, m_childProperties)
        {
            PropertySet* propertySet = childProperty->asPropertySet();
            if (propertySet)
                propertySet->findChildPropertiesRecursive(re, result);
        }
    }

    return result;
}

PropertyBase* PropertySet::findChildProperty(PropertyID id)
{
    foreach(PropertyBase* childProperty, m_childProperties)
    {
        if (childProperty->id() == id)
            return childProperty;
    }

    return nullptr;
}

void PropertySet::clearChildProperties()
{
    if (m_childProperties.isEmpty())
        return;

    foreach(PropertyBase* childProperty, m_childProperties)
    {
        disconnectChildProperty(this, childProperty);
    }

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonChildPropertyRemove, PropertyValuePtr(0));

    m_childProperties.clear();

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonChildPropertyRemove);
}

bool PropertySet::addChildProperty(PropertyBase* childProperty, bool moveOwnership)
{
    Q_CHECK_PTR(childProperty);

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonChildPropertyAdd, PropertyValuePtr(childProperty));

    m_childProperties.append(childProperty);
    connectChildProperty(this, childProperty);
    if (moveOwnership)
        childProperty->setParent(this);

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonChildPropertyAdd);

    m_ignoreChildPropertyChanges = true;
    childProperty->setStateInherited(state());
    m_ignoreChildPropertyChanges = false;

    return true;
}

bool PropertySet::removeChildProperty(PropertyBase* childProperty)
{
    Q_CHECK_PTR(childProperty);

    int childPropertyIndex = m_childProperties.indexOf(childProperty);
    if (childPropertyIndex < 0)
        return false;

    Q_ASSERT(childPropertyIndex < m_childProperties.size());

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonChildPropertyRemove, PropertyValuePtr(childProperty));

    disconnectChildProperty(this, childProperty);
    m_childProperties.erase(m_childProperties.begin()+childPropertyIndex);
    if (childProperty->parent() == this)
        childProperty->setParent(nullptr);

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonChildPropertyRemove);

    return true;
}

PropertySet* PropertySet::createNew(QObject* parentForNew) const
{
    return createNewImpl(parentForNew);
}

PropertySet* PropertySet::createCopy(QObject* parentForCopy) const
{
    return createCopyImpl(parentForCopy);
}

bool PropertySet::copyValues(PropertySet* propertySetCopyFrom, PropertyState ignoreMask)
{
    return copyValuesImpl(propertySetCopyFrom, ignoreMask);
}

void PropertySet::updateStateInherited(bool force)
{
    m_ignoreChildPropertyChanges = true;
    foreach(PropertyBase* childProperty, m_childProperties)
    {
        childProperty->setStateInherited(state(), force);
    }
    m_ignoreChildPropertyChanges = false;
}

bool PropertySet::fromStrImpl(const QString& str)
{
    static QRegExp parserLine("^\\s*([^=]+)=(.*)$");

    QStringList lines = str.split(QChar::LineFeed, QString::SkipEmptyParts);
    if (lines.isEmpty())
        return true;

    bool anySuccess = false;

    foreach (QString line, lines)
    {
        if (!parserLine.exactMatch(line))
            continue;

        QStringList params = parserLine.capturedTexts();
        if (params.size() != 3)
            continue;

        QString propertyPath = params[1];
        QString propertyStrValue = params[2];

        QList<PropertyBase*> subProperties = findChildProperties(propertyPath, Qt::FindChildrenRecursively);
        if (subProperties.size() != 1)
            continue;

        if (subProperties[0]->fromStr(propertyStrValue))
            anySuccess = true;
    }

    return anySuccess;
}

bool PropertySet::toStrImpl(QString& str) const
{
    return false;
}

bool PropertySet::loadImpl(QDataStream& stream)
{
    if (!PropertyBase::loadImpl(stream))
        return false;

    if (stream.status() != QDataStream::Ok)
        return false;

    quint8 version = 0;
    stream >> version;
    // version incorrect
    if (version != 1)
        return false;

    forever
    {
        PropertyID id = PropertyIDInvalid;
        stream >> id;

        // no child properties any more
        if (id == PropertyIDInvalid)
            break;

        PropertyBase *childProperty = findChildProperty(id);
        if (!childProperty)
        {
            // cannot find subproperty -> skip
            if (!skipLoad(stream))
                return false;
            continue;
        }

        if (childProperty->state() & PropertyStateNonSerialized)
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

bool PropertySet::saveImpl(QDataStream& stream) const
{
    if (!PropertyBase::saveImpl(stream))
        return false;

    if (stream.status() != QDataStream::Ok)
        return false;

    // for compatibility
    quint8 version = 1;
    stream << version;

    foreach (PropertyBase* childProperty, m_childProperties)
    {
        if (childProperty->state() & PropertyStateNonSerialized)
            continue;

        if (childProperty->id() == PropertyIDInvalid)
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
    stream << PropertyIDInvalid;

    return stream.status() == QDataStream::Ok;
}

void PropertySet::findChildPropertiesRecursive(const QString& name, QList<PropertyBase*>& result)
{
    foreach(PropertyBase* childProperty, m_childProperties)
    {
        if (childProperty->name() == name)
            result.append(childProperty);

        PropertySet* propertySet = childProperty->asPropertySet();
        if (propertySet)
            propertySet->findChildPropertiesRecursive(name, result);
    }
}

void PropertySet::findChildPropertiesRecursive(const QRegularExpression& re, QList<PropertyBase*>& result)
{
    foreach(PropertyBase* childProperty, m_childProperties)
    {
        if (re.match(childProperty->name()).isValid())
            result.append(childProperty);

        PropertySet* propertySet = childProperty->asPropertySet();
        if (propertySet)
            propertySet->findChildPropertiesRecursive(re, result);
    }
}

void PropertySet::childPropertyWillChange(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason, PropertyValuePtr newValue)
{
    if (m_ignoreChildPropertyChanges)
        return;

    Q_EMIT propertyWillChange(changedProperty, this, reason, newValue);
}

void PropertySet::childPropertyDidChange(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason)
{
    if (m_ignoreChildPropertyChanges)
        return;

    Q_EMIT propertyDidChange(changedProperty, this, reason);
}

} // end namespace Qtinuum
