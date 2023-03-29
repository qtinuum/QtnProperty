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

#include "PropertyBase.h"

#include "PropertySet.h"
#include "PropertyConnector.h"

#include <QIODevice>
#include <QCoreApplication>

const qint32 QtnPropertyIDInvalid = -1;
static quint16 qtnPropertyMagicNumber = 0x1984;
const quint8 QtnPropertyBase::STORAGE_VERSION = 2;

class QtnPropertyDelegateInfoGetter
{
	Q_DISABLE_COPY(QtnPropertyDelegateInfoGetter)

public:
	virtual QtnPropertyDelegateInfo *delegateInfo() = 0;

	virtual ~QtnPropertyDelegateInfoGetter() = default;

protected:
	QtnPropertyDelegateInfoGetter() = default;
};

class QtnPropertyDelegateInfoGetterValue : public QtnPropertyDelegateInfoGetter
{
public:
	QtnPropertyDelegateInfoGetterValue(const QtnPropertyDelegateInfo &delegate);

	QtnPropertyDelegateInfo *delegateInfo() override;

private:
	QtnPropertyDelegateInfo m_delegateInfo;
};

class QtnPropertyDelegateInfoGetterCallback
	: public QtnPropertyDelegateInfoGetter
{
public:
	QtnPropertyDelegateInfoGetterCallback(
		const QtnPropertyBase::DelegateInfoCallback &callback);

	QtnPropertyDelegateInfo *delegateInfo() override;

private:
	QtnPropertyBase::DelegateInfoCallback m_callback;
	QScopedPointer<QtnPropertyDelegateInfo> m_delegateInfo;
};

extern bool qtnPropertyRegister();

QtnPropertyBase::QtnPropertyBase(QObject *parent)
	: QObject(parent)
	, mPropertyConnector(nullptr)
	, m_masterProperty(nullptr)
	, m_id(QtnPropertyIDInvalid)
	, m_stateLocal(QtnPropertyStateNone)
	, m_stateInherited(QtnPropertyStateNone)
	, changeReasons(0)
	, timer(0)
	, updateEvent(nullptr)
{
	static const bool static_reg = qtnPropertyRegister();
	Q_UNUSED(static_reg);
}

bool QtnPropertyBase::event(QEvent *e)
{
	if (e == updateEvent)
	{
		updateEvent = nullptr;

		if (changeReasons != 0)
		{
			emit propertyDidChange(QtnPropertyChangeReason(changeReasons));
			changeReasons = 0;
		}

		return true;
	}

	if (e->type() == QEvent::Timer &&
		static_cast<QTimerEvent *>(e)->timerId() == timer)
	{
		killTimer(timer);
		timer = 0;

		if (changeReasons != 0)
		{
			emit propertyDidChange(QtnPropertyChangeReason(changeReasons));
			changeReasons = 0;
		}

		return true;
	}

	return QObject::event(e);
}

QtnPropertyBase::~QtnPropertyBase()
{
	disconnectMasterState();
	qtnRemovePropertyAsChild(parent(), this);
}

const QMetaObject *QtnPropertyBase::propertyMetaObject() const
{
	return metaObject();
}

void QtnPropertyBase::setName(const QString &name)
{
	if (objectName() == name)
		return;

	QtnPropertyChangeReason reason(QtnPropertyChangeReasonName);
	if (m_displayName.isEmpty() && !name.isEmpty())
	{
		m_displayName = name;
		reason |= QtnPropertyChangeReasonDisplayName;
	}

	emit propertyWillChange(
		reason, QtnPropertyValuePtr(&name), qMetaTypeId<QString>());

	setObjectName(name);

	emit propertyDidChange(reason);
}

void QtnPropertyBase::setDisplayName(const QString &displayName)
{
	if (displayName == m_displayName)
		return;

	emit propertyWillChange(QtnPropertyChangeReasonDisplayName,
		QtnPropertyValuePtr(&displayName), qMetaTypeId<QString>());

	m_displayName = displayName;

	emit propertyDidChange(QtnPropertyChangeReasonDisplayName);
}

void QtnPropertyBase::setDescription(const QString &description)
{
	if (m_description == description)
		return;

	emit propertyWillChange(QtnPropertyChangeReasonDescription,
		QtnPropertyValuePtr(&description), qMetaTypeId<QString>());

	m_description = description;

	emit propertyDidChange(QtnPropertyChangeReasonDescription);
}

void QtnPropertyBase::setId(QtnPropertyID id)
{
	if (m_id == id)
		return;

	emit propertyWillChange(QtnPropertyChangeReasonId, QtnPropertyValuePtr(&id),
		qMetaTypeId<QtnPropertyID>());

	m_id = id;

	emit propertyDidChange(QtnPropertyChangeReasonId);
}

bool QtnPropertyBase::isExpanded() const
{
	return (0 == (m_stateLocal & QtnPropertyStateCollapsed));
}

void QtnPropertyBase::setState(QtnPropertyState stateToSet, bool force)
{
	setStateInternal(stateToSet, force);
}

void QtnPropertyBase::updatePropertyState()
{
	auto connector = getConnector();
	if (connector)
	{
		connector->updatePropertyState();
	}
}

void QtnPropertyBase::setStateInternal(
	QtnPropertyState stateToSet, bool force, QtnPropertyChangeReason reason)
{
	if (!force && (m_stateLocal == stateToSet))
		return;

	reason |= QtnPropertyChangeReasonStateLocal;
	reason &= ~QtnPropertyChangeReasonLockToggled;

	if (m_stateLocal.testFlag(QtnPropertyStateUnlockable) &&
		stateToSet.testFlag(QtnPropertyStateUnlockable))
	{
		if (m_stateLocal.testFlag(QtnPropertyStateImmutable) !=
			stateToSet.testFlag(QtnPropertyStateImmutable))
		{
			reason |= QtnPropertyChangeReasonLockToggled;
		}
	}

	emit propertyWillChange(reason, QtnPropertyValuePtr(&stateToSet),
		qMetaTypeId<QtnPropertyState>());

	m_stateLocal = stateToSet;

	updatePropertyState();

	emit propertyDidChange(reason);

	updateStateInherited(force);
}

void QtnPropertyBase::addState(QtnPropertyState stateToAdd, bool force)
{
	setState(m_stateLocal | stateToAdd, force);
}

void QtnPropertyBase::removeState(QtnPropertyState stateToRemove, bool force)
{
	setState(m_stateLocal & ~stateToRemove, force);
}

void QtnPropertyBase::switchState(
	QtnPropertyState stateToSwitch, bool switchOn, bool force)
{
	if (switchOn)
		addState(stateToSwitch, force);
	else
		removeState(stateToSwitch, force);
}

void QtnPropertyBase::toggleState(QtnPropertyState stateToSwitch, bool force)
{
	switchState(stateToSwitch, !(stateLocal() & stateToSwitch), force);
}

bool QtnPropertyBase::isEditableByUser() const
{
	return !(state() & (QtnPropertyStateImmutable | QtnPropertyStateInvisible));
}

bool QtnPropertyBase::isVisible() const
{
	return !(state() & QtnPropertyStateInvisible);
}

bool QtnPropertyBase::isMultiValue() const
{
	return 0 != (m_stateLocal & QtnPropertyStateMultiValue);
}

bool QtnPropertyBase::valueIsDefault() const
{
	return 0 == (m_stateLocal & QtnPropertyStateModifiedValue);
}

bool QtnPropertyBase::isSimple() const
{
	return !m_stateLocal.testFlag(QtnPropertyStateNonSimple);
}

bool QtnPropertyBase::isLocked() const
{
	return m_stateLocal.testFlag(QtnPropertyStateImmutable);
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
	if (version != STORAGE_VERSION)
		return false;

	qint32 contentSize = 0;
	stream >> contentSize;

#ifndef QT_NO_DEBUG
	qint64 posBeforeLoadContent = 0;
	QIODevice *device = stream.device();

	if (device)
		posBeforeLoadContent = device->pos();

#endif

	if (!loadImpl(stream))
		return false;

#ifndef QT_NO_DEBUG
	qint64 posAfterLoadContent = 0;

	if (device)
		posAfterLoadContent = device->pos();

	if (posBeforeLoadContent != 0 && posAfterLoadContent != 0 &&
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
	stream << STORAGE_VERSION;

	QByteArray data;
	QDataStream contentStream(&data, QIODevice::WriteOnly);
	contentStream.setVersion(stream.version());
	contentStream.setByteOrder(stream.byteOrder());
	contentStream.setFloatingPointPrecision(stream.floatingPointPrecision());

	if (!saveImpl(contentStream))
		return false;

	// size of data to save
	stream << (qint32) data.size();

	// save content
	int savedSize = stream.writeRawData(data.constData(), data.size());

	if (savedSize != data.size())
		return false;

	return stream.status() == QDataStream::Ok;
}

bool QtnPropertyBase::skipLoad(QDataStream &stream)
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
	if (version != STORAGE_VERSION)
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

bool QtnPropertyBase::loadImpl(QDataStream &stream)
{
	Q_ASSERT(stream.status() == QDataStream::Ok);

	qint16 version = 0;
	stream >> version;

	// incorrect version
	if (version != STORAGE_VERSION)
		return false;

	QtnPropertyState::Int stateLocal = QtnPropertyStateNone;
	QtnPropertyState::Int stateInherited = QtnPropertyStateNone;
	stream >> stateLocal;
	stream >> stateInherited;
	m_stateLocal = QtnPropertyState(stateLocal);
	m_stateInherited = QtnPropertyState(stateInherited);

	return stream.status() == QDataStream::Ok;
}

bool QtnPropertyBase::saveImpl(QDataStream &stream) const
{
	Q_ASSERT(stream.status() == QDataStream::Ok);

	qint16 version = STORAGE_VERSION;
	stream << version;

	stream << (QtnPropertyState::Int) m_stateLocal;
	stream << (QtnPropertyState::Int) m_stateInherited;

	return stream.status() == QDataStream::Ok;
}

bool QtnPropertyBase::fromStrImpl(const QString &, QtnPropertyChangeReason)
{
	return false;
}

bool QtnPropertyBase::toStrImpl(QString &str) const
{
	Q_UNUSED(str);
	return false;
}

bool QtnPropertyBase::fromStr(
	const QString &str, QtnPropertyChangeReason reason)
{
	if (!isWritable())
		return false;

	QString trimmedStr = str.trimmed();
	return fromStrImpl(trimmedStr, reason);
}

bool QtnPropertyBase::toStr(QString &str) const
{
	return toStrImpl(str);
}

bool QtnPropertyBase::fromVariant(
	const QVariant &var, QtnPropertyChangeReason reason)
{
	if (!isWritable())
		return false;

	return fromVariantImpl(var, reason);
}

bool QtnPropertyBase::toVariant(QVariant &var) const
{
	return toVariantImpl(var);
}

QtnProperty *QtnPropertyBase::asProperty()
{
	return nullptr;
}

const QtnProperty *QtnPropertyBase::asProperty() const
{
	return nullptr;
}

QtnPropertySet *QtnPropertyBase::asPropertySet()
{
	return nullptr;
}

const QtnPropertySet *QtnPropertyBase::asPropertySet() const
{
	return nullptr;
}

QtnPropertyBase *QtnPropertyBase::getRootProperty()
{
	auto result = this;

	do
	{
		auto mp = result->getMasterProperty();

		if (nullptr == mp)
			break;

		result = mp;
	} while (true);

	return result;
}

QtnPropertySet *QtnPropertyBase::getRootPropertySet()
{
	auto p = this;

	while (p != nullptr)
	{
		auto set = p->asPropertySet();

		auto mp = p->getRootProperty();

		if (set && mp == p &&
			qobject_cast<QtnPropertySet *>(set->parent()) == nullptr)
		{
			return set;
		}

		if (mp != p)
		{
			p = mp;
		} else
		{
			p = qobject_cast<QtnPropertyBase *>(p->parent());
		}
	}

	return nullptr;
}

bool QtnPropertyBase::fromVariantImpl(
	const QVariant &var, QtnPropertyChangeReason reason)
{
	if (var.canConvert<QString>())
		return fromStr(var.value<QString>(), reason);
	else
		return false;
}

bool QtnPropertyBase::toVariantImpl(QVariant &var) const
{
	QString str;

	if (!toStr(str))
		return false;

    var.setValue<QString>(std::move(str));
	return true;
}

void QtnPropertyBase::updateStateInherited(bool force)
{
	Q_UNUSED(force);
	/* does nothing by default */
}

void QtnPropertyBase::connectMasterState(QtnPropertyBase *masterProperty)
{
	Q_ASSERT(nullptr != masterProperty);

	disconnectMasterState();

	m_masterProperty = masterProperty;

	beforeUpdateStateFromMasterProperty();
	doUpdateStateFromMasterProperty();

	QObject::connect(masterProperty, &QObject::destroyed, this,
		&QtnPropertyBase::onMasterPropertyDestroyed);
	QObject::connect(masterProperty, &QtnPropertyBase::propertyWillChange, this,
		&QtnPropertyBase::masterPropertyWillChange);
	QObject::connect(masterProperty, &QtnPropertyBase::propertyDidChange, this,
		&QtnPropertyBase::masterPropertyDidChange);
}

void QtnPropertyBase::disconnectMasterState()
{
	if (nullptr != m_masterProperty)
	{
		QObject::disconnect(m_masterProperty, &QObject::destroyed, this,
			&QtnPropertyBase::onMasterPropertyDestroyed);
		QObject::disconnect(m_masterProperty,
			&QtnPropertyBase::propertyWillChange, this,
			&QtnPropertyBase::masterPropertyWillChange);
		QObject::disconnect(m_masterProperty,
			&QtnPropertyBase::propertyDidChange, this,
			&QtnPropertyBase::masterPropertyDidChange);

		m_masterProperty = nullptr;
	}
}

void QtnPropertyBase::postUpdateEvent(
	QtnPropertyChangeReason reason, int afterMS)
{
	changeReasons |= reason;

	if (afterMS > 0)
	{
		if (timer == 0)
		{
			timer = startTimer(afterMS);
		}
	} else if (nullptr == updateEvent)
	{
		updateEvent = new QEvent(QEvent::User);
		QCoreApplication::postEvent(this, updateEvent);
	}
}

void QtnPropertyBase::setStateInherited(QtnPropertyState stateToSet, bool force)
{
	if (!force && (m_stateInherited == stateToSet))
		return;

	emit propertyWillChange(QtnPropertyChangeReasonStateInherited,
		QtnPropertyValuePtr(&stateToSet), qMetaTypeId<QtnPropertyState>());

	m_stateInherited = stateToSet;

	emit propertyDidChange(QtnPropertyChangeReasonStateInherited);

	updateStateInherited(force);
}

QtnPropertyState QtnPropertyBase::masterPropertyState() const
{
	return !(stateLocal() & QtnPropertyStateIgnoreDirectParentState)
		? m_masterProperty->state()
		: m_masterProperty->stateInherited();
}

void QtnPropertyBase::masterPropertyWillChange(QtnPropertyChangeReason reason)
{
	if (reason & QtnPropertyChangeReasonState)
	{
		Q_ASSERT(sender() == m_masterProperty);
		beforeUpdateStateFromMasterProperty();
	}
}

void QtnPropertyBase::masterPropertyDidChange(QtnPropertyChangeReason reason)
{
	if (reason & QtnPropertyChangeReasonState)
	{
		Q_ASSERT(sender() == m_masterProperty);
		doUpdateStateFromMasterProperty();
	}
}

void QtnPropertyBase::beforeUpdateStateFromMasterProperty()
{
	auto newState = masterPropertyState();
	if (m_stateInherited == newState)
		return;

	emit propertyWillChange(QtnPropertyChangeReasonStateInherited,
		QtnPropertyValuePtr(&newState), qMetaTypeId<QtnPropertyState>());
}

void QtnPropertyBase::doUpdateStateFromMasterProperty()
{
	auto newState = masterPropertyState();
	if (m_stateInherited == newState)
		return;

	m_stateInherited = newState;
	emit propertyDidChange(QtnPropertyChangeReasonStateInherited);

	updateStateInherited(false);
}

QVariant QtnPropertyBase::valueAsVariant() const
{
	QVariant result;
	toVariant(result);
	return result;
}

void QtnPropertyBase::onMasterPropertyDestroyed(QObject *object)
{
	Q_ASSERT(object == m_masterProperty);
	Q_UNUSED(object);
	m_masterProperty = nullptr;
}

QDataStream &operator<<(QDataStream &stream, const QtnPropertyBase &property)
{
	property.save(stream);
	return stream;
}

QDataStream &operator>>(QDataStream &stream, QtnPropertyBase &property)
{
	property.load(stream);
	return stream;
}

void QtnPropertyBase::setExpanded(bool expanded)
{
	if (expanded)
		expand();
	else
		collapse();
}

bool QtnPropertyBase::isResettable() const
{
	return isWritable() && 0 != (stateLocal() & QtnPropertyStateResettable);
}

void QtnPropertyBase::reset(QtnPropertyChangeReason reason)
{
	if (!isResettable())
		return;

	reason |= QtnPropertyChangeReasonResetValue;
	doReset(reason);
}

void QtnPropertyBase::doReset(QtnPropertyChangeReason reason)
{
	auto connector = getConnector();
	if (connector)
	{
		connector->resetPropertyValue(reason);
	}
}

bool QtnPropertyBase::isWritable() const
{
	return (0 == (state() & QtnPropertyStateImmutable));
}

bool QtnPropertyBase::isUnlockable() const
{
	return !m_stateInherited.testFlag(QtnPropertyStateImmutable) &&
		m_stateLocal.testFlag(QtnPropertyStateUnlockable);
}

void QtnPropertyBase::setLocked(bool locked, QtnPropertyChangeReason reason)
{
	Q_ASSERT(m_stateLocal & QtnPropertyStateUnlockable);

	auto state = m_stateLocal;
	state.setFlag(QtnPropertyStateImmutable, locked);
	setStateInternal(state, false, reason);
}

void QtnPropertyBase::toggleLock(QtnPropertyChangeReason reason)
{
	setLocked(!isLocked(), reason);
}

bool QtnPropertyBase::isCollapsed() const
{
	return (0 != (m_stateLocal & QtnPropertyStateCollapsed));
}

void QtnPropertyBase::setCollapsed(bool collapsed)
{
	if (collapsed)
		collapse();
	else
		expand();
}

QtnPropertyState QtnPropertyBase::state() const
{
	return m_stateLocal | m_stateInherited;
}

const QtnPropertyDelegateInfo *QtnPropertyBase::delegateInfo() const
{
	if (m_delegateInfoGetter.isNull())
		return 0;

	return m_delegateInfoGetter->delegateInfo();
}

void QtnPropertyBase::setDelegateInfo(const QtnPropertyDelegateInfo &delegate)
{
	m_delegateInfoGetter.reset(
		new QtnPropertyDelegateInfoGetterValue(delegate));
}

void QtnPropertyBase::setDelegateInfoCallback(
	const DelegateInfoCallback &callback)
{
	m_delegateInfoGetter.reset((callback != nullptr)
			? new QtnPropertyDelegateInfoGetterCallback(callback)
			: nullptr);
}

void QtnPropertyBase::setDelegateAttribute(
    const QByteArray &attributeName, const QVariant &attributeValue, QtnPropertyChangeReason reason)
{
	if (m_delegateInfoGetter.isNull())
	{
		setDelegateInfo(QtnPropertyDelegateInfo());
	}
	Q_ASSERT(!m_delegateInfoGetter.isNull());

	auto delegate = m_delegateInfoGetter->delegateInfo();
	Q_ASSERT(delegate);
	delegate->attributes[attributeName] = attributeValue;

    if (reason)
        emit propertyDidChange(reason);
}

QtnPropertyDelegateInfoGetterValue::QtnPropertyDelegateInfoGetterValue(
	const QtnPropertyDelegateInfo &delegate)
	: m_delegateInfo(delegate)
{
}

QtnPropertyDelegateInfo *QtnPropertyDelegateInfoGetterValue::delegateInfo()
{
	return &m_delegateInfo;
}

QtnPropertyDelegateInfoGetterCallback::QtnPropertyDelegateInfoGetterCallback(
	const QtnPropertyBase::DelegateInfoCallback &callback)
	: m_callback(callback)
{
	Q_ASSERT(callback != nullptr);
}

QtnPropertyDelegateInfo *QtnPropertyDelegateInfoGetterCallback::delegateInfo()
{
	if (m_delegateInfo.isNull())
	{
		m_delegateInfo.reset(new QtnPropertyDelegateInfo(m_callback()));
	}

	return m_delegateInfo.data();
}
