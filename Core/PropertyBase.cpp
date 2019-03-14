/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

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

#include "PropertyBase.h"
#include <QDebug>
#include <QtScript/QScriptEngine>

class QtnPropertyDelegateInfoGetter
{
    Q_DISABLE_COPY(QtnPropertyDelegateInfoGetter)

public:
    virtual QtnPropertyDelegateInfo* delegate() = 0;

    virtual ~QtnPropertyDelegateInfoGetter() {}

protected:
    QtnPropertyDelegateInfoGetter() {}
};

class QtnPropertyDelegateInfoGetterValue: public QtnPropertyDelegateInfoGetter
{
public:
    QtnPropertyDelegateInfoGetterValue(const QtnPropertyDelegateInfo& delegate)
        : m_delegate(delegate)
    {
    }

    QtnPropertyDelegateInfo* delegate() override
    {
        return &m_delegate;
    }

private:
    QtnPropertyDelegateInfo m_delegate;
};

class QtnPropertyDelegateInfoGetterCallback: public QtnPropertyDelegateInfoGetter
{
public:
    QtnPropertyDelegateInfoGetterCallback(const std::function<QtnPropertyDelegateInfo* ()>& callback)
        : m_callback(callback)
    {
    }

    QtnPropertyDelegateInfo* delegate() override
    {
        if (m_delegate.isNull())
        {
            m_delegate.reset(m_callback());
        }

        return m_delegate.data();
    }

private:
    std::function<QtnPropertyDelegateInfo* ()> m_callback;
    mutable QScopedPointer<QtnPropertyDelegateInfo> m_delegate;
};


static int qtnPropertyChangeReasonMetaId = qRegisterMetaType<QtnPropertyChangeReason>("QtnPropertyChangeReason");
static int qtnPropertyPtrId = qRegisterMetaType<QtnPropertyBase*>("QtnPropertyBase*");
static quint16 qtnPropertyMagicNumber = 0x1984;

// extern declaration
void qtnAddPropertyAsChild(QObject* parent, QtnPropertyBase* child, bool moveOwnership);
void qtnRemovePropertyAsChild(QObject* parent, QtnPropertyBase* child);

static QScriptValue qtnPropertyChangeReasonToScriptValue(QScriptEngine* engine, const QtnPropertyChangeReason& val)
{
    Q_UNUSED(engine);
    QScriptValue obj((QtnPropertyChangeReason::Int)val);
    return obj;
}

static void qtnPropertyChangeReasonFromScriptValue(const QScriptValue& obj, QtnPropertyChangeReason& val)
{
    val = (QtnPropertyChangeReason::enum_type)obj.toInt32();
}

static QScriptValue qtnPropertyValuePtrToScriptValue(QScriptEngine* engine, const QtnPropertyValuePtr& val)
{
    Q_UNUSED(engine);
    Q_UNUSED(val);
    // no sutable conversion
    return QScriptValue();
}

static void qtnPropertyValuePtrFromScriptValue(const QScriptValue& obj, QtnPropertyValuePtr& val)
{
    Q_UNUSED(obj);
    Q_UNUSED(val);
    // no sutable conversion
}

typedef const QtnPropertyBase* QtnPropertyBasePtr_t;

static QScriptValue qtnPropertyBasePtrToScriptValue(QScriptEngine* engine, const QtnPropertyBasePtr_t& val)
{
    QtnPropertyBasePtr_t value = val;
    QScriptValue obj = engine->newQObject(const_cast<QtnPropertyBase*>(value));
    return obj;
}

static void qtnPropertyBasePtrFromScriptValue(const QScriptValue& obj, QtnPropertyBasePtr_t& val)
{
    val = qobject_cast<const QtnPropertyBase*>(obj.toQObject());
}

void qtnScriptRegisterPropertyTypes(QScriptEngine* engine)
{
    qScriptRegisterMetaType(engine, qtnPropertyChangeReasonToScriptValue, qtnPropertyChangeReasonFromScriptValue);
    qScriptRegisterMetaType(engine, qtnPropertyValuePtrToScriptValue, qtnPropertyValuePtrFromScriptValue);
    qScriptRegisterMetaType(engine, qtnPropertyBasePtrToScriptValue, qtnPropertyBasePtrFromScriptValue);

    QScriptValue obj = engine->globalObject();

    // QScriptValue obj = engine->newObject();

    obj.setProperty("QtnPropertyStateNone", QtnPropertyStateNone, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyStateNonSimple", QtnPropertyStateNonSimple, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyStateInvisible", QtnPropertyStateInvisible, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyStateImmutable", QtnPropertyStateImmutable, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyStateCollapsed", QtnPropertyStateCollapsed, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyStateNonSerialized", QtnPropertyStateNonSerialized, QScriptValue::ReadOnly|QScriptValue::Undeletable);

    obj.setProperty("QtnPropertyChangeReasonNewValue", QtnPropertyChangeReasonNewValue, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonLoadedValue", QtnPropertyChangeReasonLoadedValue, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonValue", QtnPropertyChangeReasonValue, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonName", QtnPropertyChangeReasonName, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonDescription", QtnPropertyChangeReasonDescription, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonId", QtnPropertyChangeReasonId, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonStateLocal", QtnPropertyChangeReasonStateLocal, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonStateInherited", QtnPropertyChangeReasonStateInherited, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonState", QtnPropertyChangeReasonState, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonChildPropertyAdd", QtnPropertyChangeReasonChildPropertyAdd, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonChildPropertyRemove", QtnPropertyChangeReasonChildPropertyRemove, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("QtnPropertyChangeReasonChildren", QtnPropertyChangeReasonChildren, QScriptValue::ReadOnly|QScriptValue::Undeletable);

//    engine->globalObject().setProperty("Qtinuum", obj);
}

QtnPropertyBase::QtnPropertyBase(QObject *parent)
    : QObject(parent),
      m_id(QtnPropertyIDInvalid),
      m_stateLocal(QtnPropertyStateNone),
      m_stateInherited(QtnPropertyStateNone)
{
//    qtnAddPropertyAsChild(parent, this, false);
}

QtnPropertyBase::~QtnPropertyBase()
{
    qtnRemovePropertyAsChild(parent(), this);
}

void QtnPropertyBase::setName(const QString& name)
{
    if (name.contains(' ') || name.contains('.'))
    {
        // cannot set non-identifier name
        // use displayName instead
        Q_ASSERT(false);
        return;
    }

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonDisplayName|QtnPropertyChangeReasonName, QtnPropertyValuePtr(&name));

    setObjectName(name);
    m_displayName = name;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonDisplayName|QtnPropertyChangeReasonName);
}

void QtnPropertyBase::setDisplayName(const QString& displayName)
{
    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonDisplayName, QtnPropertyValuePtr(&displayName));

    m_displayName = displayName;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonDisplayName);
}

void QtnPropertyBase::setDescription(const QString& description)
{
    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonDescription, QtnPropertyValuePtr(&description));

    m_description = description;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonDescription);
}

void QtnPropertyBase::setId(QtnPropertyID id)
{
    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonId, QtnPropertyValuePtr(&id));

    m_id = id;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonId);
}

void QtnPropertyBase::setState(QtnPropertyState stateToSet, bool force)
{
    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonStateLocal, QtnPropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonStateLocal);

    updateStateInherited(force);
}

void QtnPropertyBase::addState(QtnPropertyState stateToAdd, bool force)
{
    QtnPropertyState stateToSet = m_stateLocal|stateToAdd;

    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonStateLocal, QtnPropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonStateLocal);

    updateStateInherited(force);
}

void QtnPropertyBase::removeState(QtnPropertyState stateToRemove, bool force)
{
    QtnPropertyState stateToSet = m_stateLocal&~stateToRemove;

    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonStateLocal, QtnPropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonStateLocal);

    updateStateInherited(force);
}

void QtnPropertyBase::switchState(QtnPropertyState stateToSwitch, bool switchOn, bool force)
{
    if (switchOn)
        addState(stateToSwitch, force);
    else
        removeState(stateToSwitch, force);
}

void QtnPropertyBase::switchStateAuto(QtnPropertyState stateToSwitch, bool force)
{
    if (!(stateLocal() & stateToSwitch))
        addState(stateToSwitch, force);
    else
        removeState(stateToSwitch, force);
}

bool QtnPropertyBase::isEditable() const
{
    return !(state()&(QtnPropertyStateImmutable));
}

bool QtnPropertyBase::isEditableByUser() const
{
    return !(state()&(QtnPropertyStateImmutable|QtnPropertyStateInvisible));
}

bool QtnPropertyBase::isVisible() const
{
    return !(state()&(QtnPropertyStateInvisible));
}

bool QtnPropertyBase::load(QDataStream &stream)
{
    if (stream.status() != QDataStream::Ok)
        return false;

    quint16 magicNumber = 0;
    stream >> magicNumber;
    // consistency corrupted
    if (magicNumber != qtnPropertyMagicNumber)
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

bool QtnPropertyBase::save(QDataStream &stream) const
{
    if (stream.status() != QDataStream::Ok)
        return false;

    // for better consistency
    stream << qtnPropertyMagicNumber;

    // for compatibility
    quint8 version = 1;
    stream << version;

    QByteArray data;
    QDataStream contentStream(&data, QIODevice::WriteOnly);
    contentStream.setVersion(stream.version());
    contentStream.setByteOrder(stream.byteOrder());
    contentStream.setFloatingPointPrecision(stream.floatingPointPrecision());

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

bool QtnPropertyBase::skipLoad(QDataStream& stream)
{
    if (stream.status() != QDataStream::Ok)
        return false;

    quint16 magicNumber = 0;
    stream >> magicNumber;
    // consistency corrupted
    if (magicNumber != qtnPropertyMagicNumber)
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

bool QtnPropertyBase::loadImpl(QDataStream& stream)
{
    Q_ASSERT(stream.status() == QDataStream::Ok);

    qint16 version = 0;
    stream >> version;
    // incorrect version
    if (version != 1)
        return false;

    QtnPropertyState::Int stateLocal = QtnPropertyStateNone;
    QtnPropertyState::Int stateInherited = QtnPropertyStateNone;
    stream >> stateLocal;
    stream >> stateInherited;
    m_stateLocal = QtnPropertyState(stateLocal);
    m_stateInherited = QtnPropertyState(stateInherited);

    return stream.status() == QDataStream::Ok;
}

bool QtnPropertyBase::saveImpl(QDataStream& stream) const
{
    Q_ASSERT(stream.status() == QDataStream::Ok);

    qint16 version = 1;
    stream << version;

    stream << (QtnPropertyState::Int)m_stateLocal;
    stream << (QtnPropertyState::Int)m_stateInherited;

    return stream.status() == QDataStream::Ok;
}

bool QtnPropertyBase::fromStr(const QString& str)
{
    if (!isEditable())
        return true;

    QString trimmedStr = str.trimmed();
    return fromStrImpl(trimmedStr);
}

bool QtnPropertyBase::toStr(QString& str) const
{
    return toStrImpl(str);
}

bool QtnPropertyBase::fromVariant(const QVariant& var)
{
    if (!isEditable())
        return false;

    return fromVariantImpl(var);
}

bool QtnPropertyBase::toVariant(QVariant& var) const
{
    return toVariantImpl(var);
}

bool QtnPropertyBase::fromVariantImpl(const QVariant& var)
{
    if (var.canConvert<QString>())
        return fromStr(var.value<QString>());
    else
        return false;
}

bool QtnPropertyBase::toVariantImpl(QVariant& var) const
{
    QString str;
    if (!toStr(str))
        return false;

    var.setValue<QString>(str);
    return true;
}

const QtnPropertyDelegateInfo* QtnPropertyBase::delegate() const
{
    if (m_delegateInfoGetter.isNull())
        return 0;

    return m_delegateInfoGetter->delegate();
}

void QtnPropertyBase::setDelegate(const QtnPropertyDelegateInfo& delegate)
{
    m_delegateInfoGetter.reset(new QtnPropertyDelegateInfoGetterValue(delegate));
}

void QtnPropertyBase::setDelegateCallback(const std::function<QtnPropertyDelegateInfo*()>& callback)
{
    m_delegateInfoGetter.reset(new QtnPropertyDelegateInfoGetterCallback(callback));
}

void QtnPropertyBase::setDelegateAttribute(const QByteArray& attributeName, const QVariant& attributeValue)
{
    if (m_delegateInfoGetter.isNull())
    {
        Q_ASSERT(false);
        return;
    }

    auto delegate = m_delegateInfoGetter->delegate();
    delegate->attributes[attributeName] = attributeValue;
}

void QtnPropertyBase::setResetCallback(const std::function<void(QtnPropertyBase&)>& resetCallback)
{
    m_resetCallback = resetCallback;
}

bool QtnPropertyBase::reset()
{
    if (!m_resetCallback)
        return false;

    m_resetCallback(*this);
    return true;
}


void QtnPropertyBase::connectMasterSignals(const QtnPropertyBase& masterProperty, QtnPropertyBase& slaveProperty)
{
    slaveProperty.setStateInherited(masterProperty.state());
    QObject::connect(&masterProperty, &QtnPropertyBase::propertyWillChange, &slaveProperty, &QtnPropertyBase::masterPropertyWillChange);
    QObject::connect(&masterProperty, &QtnPropertyBase::propertyDidChange, &slaveProperty, &QtnPropertyBase::masterPropertyDidChange);
}

void QtnPropertyBase::disconnectMasterSignals(const QtnPropertyBase& masterProperty, QtnPropertyBase& slaveProperty)
{
    QObject::disconnect(&masterProperty, &QtnPropertyBase::propertyWillChange, &slaveProperty, &QtnPropertyBase::masterPropertyWillChange);
    QObject::disconnect(&masterProperty, &QtnPropertyBase::propertyDidChange, &slaveProperty, &QtnPropertyBase::masterPropertyDidChange);
}

void QtnPropertyBase::setStateInherited(QtnPropertyState stateToSet, bool force)
{
    if (!force && (m_stateInherited == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonStateInherited, QtnPropertyValuePtr(&stateToSet));

    m_stateInherited = stateToSet;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonStateInherited);

    updateStateInherited(force);
}

void QtnPropertyBase::masterPropertyWillChange(const QtnPropertyBase*, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr)
{
    if (reason & QtnPropertyChangeReasonValue)
        Q_EMIT propertyWillChange(this, firedProperty, reason, nullptr);
}

void QtnPropertyBase::masterPropertyDidChange(const QtnPropertyBase *changedProperty, const QtnPropertyBase *firedProperty, QtnPropertyChangeReason reason)
{
    // state has changed and not from child property
    if ((reason & QtnPropertyChangeReasonState) && (changedProperty == firedProperty))
    {
        setStateInherited(changedProperty->state());
    }

    if (reason & QtnPropertyChangeReasonValue)
        Q_EMIT propertyDidChange(this, firedProperty, reason);
}

QVariant QtnPropertyBase::valueAsVariant() const
{
    QVariant result;
    toVariant(result);
    return result;
}

void QtnPropertyBase::setValueAsVariant(const QVariant& value)
{
    fromVariant(value);
}

QDataStream& operator<< (QDataStream& stream, const QtnPropertyBase& property)
{
    property.save(stream);
    return stream;
}

QDataStream& operator>> (QDataStream& stream, QtnPropertyBase& property)
{
    property.load(stream);
    return stream;
}
