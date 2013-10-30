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

#include "PropertyBase.h"
#include <QVariant>

namespace Qtinuum
{

static int PropertyChangeReasonMetaId = qRegisterMetaType<PropertyChangeReason>("PropertyChangeReason");
static int PropertyStateMetaId = qRegisterMetaType<PropertyState>("PropertyState");
static quint16 PropertyMagicNumber = 0x1984;

// extern declaration
void addPropertyAsChild(QObject* parent, PropertyBase* child, bool moveOwnership);
void removePropertyAsChild(QObject* parent, PropertyBase* child);

PropertyBase::PropertyBase(QObject *parent)
    : QObject(parent),
      m_id(PropertyIDInvalid),
      m_stateLocal(PropertyStateNone),
      m_stateInherited(PropertyStateNone)
{
    addPropertyAsChild(parent, this, false);
}

PropertyBase::~PropertyBase()
{
    removePropertyAsChild(parent(), this);
}

void PropertyBase::setName(const QString& name)
{
    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonName, PropertyValuePtr(&name));

    setObjectName(name);

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonName);
}

void PropertyBase::setDescription(const QString& description)
{
    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonDescription, PropertyValuePtr(&description));

    m_description = description;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonDescription);
}

void PropertyBase::setId(PropertyID id)
{
    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonId, PropertyValuePtr(&id));

    m_id = id;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonId);
}

void PropertyBase::setState(PropertyState stateToSet, bool force)
{
    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonStateLocal, PropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonStateLocal);

    updateStateInherited(force);
}

void PropertyBase::addState(PropertyState stateToAdd, bool force)
{
    PropertyState stateToSet = m_stateLocal|stateToAdd;

    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonStateLocal, PropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonStateLocal);

    updateStateInherited(force);
}

void PropertyBase::removeState(PropertyState stateToRemove, bool force)
{
    PropertyState stateToSet = m_stateLocal&~stateToRemove;

    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonStateLocal, PropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonStateLocal);

    updateStateInherited(force);
}

void PropertyBase::switchState(PropertyState stateToSwitch, bool switchOn, bool force)
{
    if (switchOn)
        addState(stateToSwitch, force);
    else
        removeState(stateToSwitch, force);
}

void PropertyBase::switchStateAuto(PropertyState stateToSwitch, bool force)
{
    if (!(stateLocal() & stateToSwitch))
        addState(stateToSwitch, force);
    else
        removeState(stateToSwitch, force);
}

bool PropertyBase::isEditableByUser() const
{
    return !(state()&(PropertyStateImmutable|PropertyStateInvisible));
}

bool PropertyBase::isVisible() const
{
    return !(state()&(PropertyStateInvisible));
}

bool PropertyBase::load(QDataStream &stream)
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

    if (!loadImpl(stream))
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

    return stream.status() == QDataStream::Ok;
}

bool PropertyBase::save(QDataStream &stream) const
{
    if (stream.status() != QDataStream::Ok)
        return false;

    // for better consistency
    stream << PropertyMagicNumber;

    // for compatibility
    quint8 version = 1;
    stream << version;

    QByteArray data;
    QDataStream contentStream(&data, QIODevice::WriteOnly);
    if (!saveImpl(contentStream))
        return false;

    // size of data to save
    stream << (qint32)data.size();

    // save content
    int savedSize = stream.writeRawData(data.constData(), data.size());

    if (savedSize != data.size())
        return false;

    return stream.status() == QDataStream::Ok;
}

bool PropertyBase::skipLoad(QDataStream& stream)
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

    return stream.status() == QDataStream::Ok;
}

bool PropertyBase::loadImpl(QDataStream& stream)
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

bool PropertyBase::saveImpl(QDataStream& stream) const
{
    Q_ASSERT(stream.status() == QDataStream::Ok);

    qint16 version = 1;
    stream << version;

    stream << (PropertyState::Int)m_stateLocal;
    stream << (PropertyState::Int)m_stateInherited;

    return stream.status() == QDataStream::Ok;
}

bool PropertyBase::fromStr(const QString& str)
{
    return fromStrImpl(str);
}

bool PropertyBase::toStr(QString& str) const
{
    return toStrImpl(str);
}

bool PropertyBase::fromVariant(const QVariant& var)
{
    return fromVariantImpl(var);
}

bool PropertyBase::toVariant(QVariant& var)
{
    return toVariantImpl(var);
}

bool PropertyBase::fromVariantImpl(const QVariant& var)
{
    if (var.canConvert<QString>())
        return fromStr(var.value<QString>());
    else
        return false;
}

bool PropertyBase::toVariantImpl(QVariant& var) const
{
    QString str;
    if (!toStr(str))
        return false;

    var.setValue<QString>(str);
    return true;
}

QMetaObject::Connection PropertyBase::connectMasterState(const PropertyBase& masterProperty, PropertyBase& slaveProperty)
{
    slaveProperty.setStateInherited(masterProperty.state());
    return QObject::connect(&masterProperty, &PropertyBase::propertyDidChange, &slaveProperty, &PropertyBase::masterPropertyStateDidChange);
}

bool PropertyBase::disconnectMasterState(const PropertyBase& masterProperty, PropertyBase& slaveProperty)
{
    return QObject::disconnect(&masterProperty, &PropertyBase::propertyDidChange, &slaveProperty, &PropertyBase::masterPropertyStateDidChange);
}

void PropertyBase::setStateInherited(PropertyState stateToSet, bool force)
{
    if (!force && (m_stateInherited == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, PropertyChangeReasonStateInherited, PropertyValuePtr(&stateToSet));

    m_stateInherited = stateToSet;

    Q_EMIT propertyDidChange(this, this, PropertyChangeReasonStateInherited);

    updateStateInherited(force);
}

void PropertyBase::masterPropertyStateDidChange(const PropertyBase *changedProperty, const PropertyBase *firedProperty, PropertyChangeReason reason)
{
    // state has changed and not from child property
    if ((reason | PropertyChangeReasonState) && (changedProperty == firedProperty))
    {
        setStateInherited(changedProperty->state());
    }
}

QDataStream& operator<< (QDataStream& stream, const PropertyBase& property)
{
    property.save(stream);
    return stream;
}

QDataStream& operator>> (QDataStream& stream, PropertyBase& property)
{
    property.load(stream);
    return stream;
}


} // end namespace Qtinuum
