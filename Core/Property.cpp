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

#include "Property.h"
#include <QRegularExpression>

namespace Qtinuum
{

static int PropertyChangeReasonMetaId = qRegisterMetaType<PropertyChangeReason>("PropertyChangeReason");
static int PropertyStateMetaId = qRegisterMetaType<PropertyState>("PropertyState");
static quint16 PropertyMagicNumber = 0x1984;

class PropertyDelegateInfoGetter
{
    Q_DISABLE_COPY(PropertyDelegateInfoGetter)

public:
    virtual const PropertyDelegateInfo *delegate() const = 0;

    virtual ~PropertyDelegateInfoGetter() {}

protected:
    PropertyDelegateInfoGetter() {}
};

class PropertyDelegateInfoGetterValue: public PropertyDelegateInfoGetter
{
public:
    PropertyDelegateInfoGetterValue(const PropertyDelegateInfo &delegate)
        : m_delegate(delegate)
    {
    }

    const PropertyDelegateInfo *delegate() const override
    {
        return &m_delegate;
    }

private:
    PropertyDelegateInfo m_delegate;
};

class PropertyDelegateInfoGetterCallback: public PropertyDelegateInfoGetter
{
public:
    PropertyDelegateInfoGetterCallback(const std::function<const PropertyDelegateInfo *()> &callback)
        : m_callback(callback)
    {
    }

    const PropertyDelegateInfo *delegate() const override
    {
        if (m_delegate.isNull())
        {
            m_delegate.reset(m_callback());
        }

        return m_delegate.data();
    }

private:
    std::function<const PropertyDelegateInfo *()> m_callback;
    mutable QScopedPointer<const PropertyDelegateInfo> m_delegate;
};

void connectChildProperty(Property *masterProperty, Property *childProperty)
{
    QObject::connect(childProperty, &Property::propertyWillChange, masterProperty, &Property::childPropertyWillChange);
    QObject::connect(childProperty, &Property::propertyDidChange, masterProperty, &Property::childPropertyDidChange);
}

void disconnectChildProperty(Property *masterProperty, Property *childProperty)
{
    QObject::disconnect(childProperty, &Property::propertyWillChange, masterProperty, &Property::childPropertyWillChange);
    QObject::disconnect(childProperty, &Property::propertyDidChange, masterProperty, &Property::childPropertyDidChange);
}

Property::Property(QObject *parent)
    : QObject(parent),
      m_id(PropertyIDInvalid),
      m_stateLocal(PropertyStateNone),
      m_stateInherited(PropertyStateNone),
      m_ignoreChildPropertyChanges(false)
{
    Property *parentProperty = qobject_cast<Property*>(parent);
    if (parentProperty)
        parentProperty->addChildProperty(this, false);
}

Property::Property(QObject *parent, const Property &other)
    : QObject(parent),
      m_description(other.m_description),
      m_id(other.m_id),
      m_stateLocal(other.m_stateLocal),
      m_stateInherited(other.m_stateInherited),
      m_ignoreChildPropertyChanges(false)
{
    Property *parentProperty = qobject_cast<Property*>(parent);
    if (parentProperty)
        parentProperty->addChildProperty(this, false);
}

Property::~Property()
{
    clearChildProperties();

    Property *parentProperty = qobject_cast<Property*>(parent());
    if (parentProperty)
        parentProperty->removeChildProperty(this);
}

/*!
 * \brief Property::setName
 * Sets property name (calls QObject::setObjectName).
 *
 * Property::propertyWillChange and Property::propertyDidChange signals
 * will be emitted with PropertyChangeReasonName reason flag.
 *
 * \param name new name of the property.
 */
void Property::setName(const QString &name)
{
    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonName, PropertyValuePtr(&name));

    setObjectName(name);

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonName);
}

void Property::setDescription(const QString &description)
{
    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonDescription, PropertyValuePtr(&description));

    m_description = description;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonDescription);
}

void Property::setId(PropertyID id)
{
    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonId, PropertyValuePtr(&id));

    m_id = id;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonId);
}

void Property::setState(PropertyState stateToSet, bool force)
{
    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonStateLocal, PropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonStateLocal);

    m_ignoreChildPropertyChanges = true;
    // update state in properties hierarchy
    foreach(Property *childProperty, m_childProperties)
    {
        childProperty->setStateInherited(state(), force);
    }
    m_ignoreChildPropertyChanges = false;
}

void Property::addState(PropertyState stateToAdd, bool force)
{
    PropertyState stateToSet = m_stateLocal|stateToAdd;

    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonStateLocal, PropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonStateLocal);

    m_ignoreChildPropertyChanges = true;
    // update state in properties hierarchy
    foreach(Property *childProperty, m_childProperties)
    {
        childProperty->setStateInherited(state(), force);
    }
    m_ignoreChildPropertyChanges = false;
}

void Property::removeState(PropertyState stateToRemove, bool force)
{
    PropertyState stateToSet = m_stateLocal&~stateToRemove;

    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonStateLocal, PropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonStateLocal);

    m_ignoreChildPropertyChanges = true;
    // update state in properties hierarchy
    foreach(Property *childProperty, m_childProperties)
    {
        childProperty->setStateInherited(state(), force);
    }
    m_ignoreChildPropertyChanges = false;
}

void Property::switchState(PropertyState stateToSwitch, bool switchOn, bool force)
{
    if (switchOn)
        addState(stateToSwitch, force);
    else
        removeState(stateToSwitch, force);
}

void Property::switchStateAuto(PropertyState stateToSwitch, bool force)
{
    if (!(stateLocal() & stateToSwitch))
        addState(stateToSwitch, force);
    else
        removeState(stateToSwitch, force);
}

QMetaObject::Connection Property::connectMasterState(const Property &masterProperty)
{
    setStateInherited(masterProperty.state());
    return QObject::connect(&masterProperty, &Property::propertyDidChange, this, &Property::masterPropertyStateDidChange);
}

bool Property::disconnectMasterState(const Property &masterProperty)
{
    return QObject::disconnect(&masterProperty, &Property::propertyDidChange, this, &Property::masterPropertyStateDidChange);
}

bool Property::isEditableByUser() const
{
    return !(state()&(PropertyStateImmutable|PropertyStateInvisible));
}

bool Property::isVisible() const
{
    return !(state()&(PropertyStateInvisible));
}

QList<Property*> Property::findChildProperties(const QString &name, Qt::FindChildOptions options)
{
    QList<Property*> result;

    foreach(Property *childProperty, m_childProperties)
    {
        if (childProperty->name() == name)
            result.append(childProperty);
    }

    if (options & Qt::FindChildrenRecursively)
    {
        foreach(Property *childProperty, m_childProperties)
        {
            childProperty->findChildPropertiesRecursive(name, result);
        }
    }

    return result;
}

QList<Property*> Property::findChildProperties(const QRegularExpression &re, Qt::FindChildOptions options)
{
    QList<Property*> result;

    foreach(Property *childProperty, m_childProperties)
    {
        if (re.match(childProperty->name()).isValid())
            result.append(childProperty);
    }

    if (options & Qt::FindChildrenRecursively)
    {
        foreach(Property *childProperty, m_childProperties)
        {
            childProperty->findChildPropertiesRecursive(re, result);
        }
    }

    return result;
}

Property *Property::findChildProperty(PropertyID id)
{
    foreach(Property *childProperty, m_childProperties)
    {
        if (childProperty->id() == id)
            return childProperty;
    }

    return 0;
}

void Property::clearChildProperties()
{
    if (m_childProperties.isEmpty())
        return;

    foreach(Property *childProperty, m_childProperties)
    {
        disconnectChildProperty(this, childProperty);
    }

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonChildPropertyRemove, PropertyValuePtr(0));

    m_childProperties.clear();

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonChildPropertyRemove);
}

bool Property::addChildProperty(Property *childProperty, bool moveOwnership)
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

bool Property::removeChildProperty(Property *childProperty)
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
        childProperty->setParent(0);

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonChildPropertyRemove);

    return true;
}

const PropertyDelegateInfo *Property::delegate() const
{
    if (m_delegateInfoGetter.isNull())
        return 0;

    return m_delegateInfoGetter->delegate();
}

void Property::setDelegate(const PropertyDelegateInfo &delegate)
{
    m_delegateInfoGetter.reset(new PropertyDelegateInfoGetterValue(delegate));
}

void Property::setDelegateCallback(const std::function<const PropertyDelegateInfo *()> &callback)
{
    m_delegateInfoGetter.reset(new PropertyDelegateInfoGetterCallback(callback));
}

Property* Property::createNew(QObject* parentForNew) const
{
    return createNewImpl(parentForNew);
}

Property* Property::createCopy(QObject* parentForCopy) const
{
    return createCopyImpl(parentForCopy);
}

Property* Property::createNewImpl(QObject* parentForNew) const
{
    return new Property(parentForNew);
}

Property* Property::createCopyImpl(QObject* parentForCopy) const
{
    return new Property(parentForCopy, *this);
}

bool Property::load(QDataStream &stream)
{
    if (stream.status() != QDataStream::Ok)
        return false;

    quint16 magicNumber = 0;
    stream >> magicNumber;
    // consistency corrupted
    if (magicNumber != PropertyMagicNumber)
        return false;

    quint8 version = 0;
    stream >> version;
    // version incorrect
    if (version != 1)
        return false;

    qint32 contentSize = 0;
    stream >> contentSize;

#ifndef QT_NO_DEBUG
    qint64 posBeforeLoadContent = 0;
    QIODevice* device = stream.device();
    if (device)
        posBeforeLoadContent = device->pos();
#endif

    if (!loadContentImpl(stream))
        return false;

#ifndef QT_NO_DEBUG
    qint64 posAfterLoadContent = 0;
    if (device)
        posAfterLoadContent = device->pos();

    if (posBeforeLoadContent != 0 &&
        posAfterLoadContent != 0 &&
        (qint32(posAfterLoadContent - posBeforeLoadContent) != contentSize))
    {
        Q_ASSERT(false && "contentSize and loadContentImpl inconsistency.");
    }
#endif

    forever
    {
        PropertyID id = PropertyIDInvalid;
        stream >> id;

        // no child properties any more
        if (id == PropertyIDInvalid)
            break;

        Property *childProperty = findChildProperty(id);
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

bool Property::skipLoad(QDataStream &stream)
{
    if (stream.status() != QDataStream::Ok)
        return false;

    quint16 magicNumber = 0;
    stream >> magicNumber;
    // consistency corrupted
    if (magicNumber != PropertyMagicNumber)
        return false;

    quint8 version = 0;
    stream >> version;
    // version incorrect
    if (version != 1)
        return false;

    qint32 contentSize = 0;
    stream >> contentSize;

    {
        int read = stream.skipRawData(contentSize);
        // corrupted data
        if (read != contentSize)
            return false;
    }

    forever
    {
        PropertyID id = PropertyIDInvalid;
        stream >> id;

        // no child properties any more
        if (id == PropertyIDInvalid)
            break;

        // skip all children properties
        if (!skipLoad(stream))
            return false;
    }

    return stream.status() == QDataStream::Ok;
}

bool Property::save(QDataStream &stream) const
{
    if (stream.status() != QDataStream::Ok)
        return false;

    // for better consistency
    stream << PropertyMagicNumber;

    // for compatibility
    quint8 version = 1;
    stream << version;

    QByteArray contentData;
    {
        QDataStream contentStream(&contentData, QIODevice::WriteOnly);
        if (!saveContentImpl(contentStream))
            return false;
    }

    qint32 contentSize = contentData.size();
    stream << contentSize;

    int result = stream.writeRawData(contentData.data(), contentData.size());
    if (result == -1)
        return false;

    foreach (Property *childProperty, m_childProperties)
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

bool Property::loadContentImpl(QDataStream &stream)
{
    Q_ASSERT(stream.status() == QDataStream::Ok);

    qint16 version = 0;
    stream >> version;
    // incorrect version
    if (version != 1)
        return false;

    PropertyState::Int stateLocal = PropertyStateNone;
    PropertyState::Int stateInherited = PropertyStateNone;
    stream >> stateLocal;
    stream >> stateInherited;
    m_stateLocal = PropertyState(stateLocal);
    m_stateInherited = PropertyState(stateInherited);

    return stream.status() == QDataStream::Ok;
}

bool Property::saveContentImpl(QDataStream &stream) const
{
    Q_ASSERT(stream.status() == QDataStream::Ok);

    qint16 version = 1;
    stream << version;

    stream << (PropertyState::Int)m_stateLocal;
    stream << (PropertyState::Int)m_stateInherited;

    return stream.status() == QDataStream::Ok;
}

void Property::setStateInherited(PropertyState stateToSet, bool force)
{
    if (!force && (m_stateInherited == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonStateInherited, PropertyValuePtr(&stateToSet));

    m_stateInherited = stateToSet;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonStateInherited);

    // update state in properties hierarchy
    foreach(Property *childProperty, m_childProperties)
    {
        childProperty->setStateInherited(state(), force);
    }
}

void Property::findChildPropertiesRecursive(const QString &name, QList<Property*> &result)
{
    foreach(Property *childProperty, m_childProperties)
    {
        if (childProperty->name() == name)
            result.append(childProperty);

        childProperty->findChildPropertiesRecursive(name, result);
    }
}

void Property::findChildPropertiesRecursive(const QRegularExpression &re, QList<Property*> &result)
{
    foreach(Property *childProperty, m_childProperties)
    {
        if (re.match(childProperty->name()).isValid())
            result.append(childProperty);

        childProperty->findChildPropertiesRecursive(re, result);
    }
}

void Property::childPropertyWillChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason, PropertyValuePtr newValue)
{
    if (m_ignoreChildPropertyChanges)
        return;

    Q_EMIT propertyWillChange(changedProperty, this, reason, newValue);
}

void Property::childPropertyDidChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason)
{
    if (m_ignoreChildPropertyChanges)
        return;

    Q_EMIT propertyDidChange(changedProperty, this, reason);
}

void Property::masterPropertyStateDidChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason)
{
    // state has changed and not from child property
    if ((reason | PropertyChangeReasonState) && (changedProperty == firedProperty))
    {
        setStateInherited(changedProperty->state());
    }
}

QDataStream& operator<< (QDataStream& stream, const Property& property)
{
    property.save(stream);
    return stream;
}

QDataStream& operator>> (QDataStream& stream, Property& property)
{
    property.load(stream);
    return stream;
}


} // end namespace Qtinuum
