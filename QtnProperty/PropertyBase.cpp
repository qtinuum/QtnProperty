/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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

#include "PropertyConnector.h"

#include <QScriptEngine>
#include <QCoreApplication>

static int qtnPropertyChangeReasonMetaId = qRegisterMetaType<QtnPropertyChangeReason>("QtnPropertyChangeReason");
static int qtnPropertyPtrId = qRegisterMetaType<QtnPropertyBase*>("QtnPropertyBase*");
static quint16 qtnPropertyMagicNumber = 0x1984;

// extern declaration
void qtnAddPropertyAsChild(QObject* parent, QtnPropertyBase* child, bool moveOwnership);
void qtnRemovePropertyAsChild(QObject* parent, QtnPropertyBase* child);

static QScriptValue qtnPropertyChangeReasonToScriptValue(QScriptEngine* engine, const QtnPropertyChangeReason& val)
{
	QScriptValue obj((QtnPropertyChangeReason::Int)val);
	return obj;
}

static void qtnPropertyChangeReasonFromScriptValue(const QScriptValue& obj, QtnPropertyChangeReason& val)
{
	val = (QtnPropertyChangeReason::enum_type)obj.toInt32();
}

static QScriptValue qtnPropertyValuePtrToScriptValue(QScriptEngine* engine, const QtnPropertyValuePtr& val)
{
	// no sutable conversion
	return QScriptValue();
}

static void qtnPropertyValuePtrFromScriptValue(const QScriptValue& obj, QtnPropertyValuePtr& val)
{
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

//    QScriptValue obj = engine->newObject();

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
	: QObject(parent)
	, m_id(QtnPropertyIDInvalid)
	, m_stateLocal(QtnPropertyStateNone)
	, m_stateInherited(QtnPropertyStateNone)
	, changeReasons(0)
	, timer(0)
{
	qtnAddPropertyAsChild(parent, this, false);
}

bool QtnPropertyBase::event(QEvent *e)
{
	switch (e->type())
	{
		case QEvent::Timer:
		{
			int reasons = changeReasons;
			if (reasons != 0)
			{
				emit propertyDidChange(QtnPropertyChangeReason(reasons));
				changeReasons = 0;
			} else
			{
				killTimer(timer);
				timer = 0;
			}
		}	break;

		default:
			break;
	}

	return QObject::event(e);
}

QtnPropertyBase::~QtnPropertyBase()
{
	qtnRemovePropertyAsChild(parent(), this);
}

void QtnPropertyBase::setName(const QString& name)
{
	emit propertyWillChange(QtnPropertyChangeReasonName,
							QtnPropertyValuePtr(&name),
							qMetaTypeId<QString>());

	setObjectName(name);

	emit propertyDidChange(QtnPropertyChangeReasonName);
}

void QtnPropertyBase::setDescription(const QString& description)
{
	emit propertyWillChange(QtnPropertyChangeReasonDescription,
							QtnPropertyValuePtr(&description),
							qMetaTypeId<QString>());

	m_description = description;

	emit propertyDidChange(QtnPropertyChangeReasonDescription);
}

void QtnPropertyBase::setId(QtnPropertyID id)
{
	emit propertyWillChange(QtnPropertyChangeReasonId,
							QtnPropertyValuePtr(&id),
							qMetaTypeId<QtnPropertyID>());

	m_id = id;

	emit propertyDidChange(QtnPropertyChangeReasonId);
}

void QtnPropertyBase::setState(QtnPropertyState stateToSet, bool force)
{
	if (!force && (m_stateLocal == stateToSet))
		return;

	emit propertyWillChange(QtnPropertyChangeReasonStateLocal,
							QtnPropertyValuePtr(&stateToSet),
							qMetaTypeId<QtnPropertyState>());

	m_stateLocal = stateToSet;

	emit propertyDidChange(QtnPropertyChangeReasonStateLocal);

	updateStateInherited(force);
}

void QtnPropertyBase::addState(QtnPropertyState stateToAdd, bool force)
{
	QtnPropertyState stateToSet = m_stateLocal|stateToAdd;

	if (!force && (m_stateLocal == stateToSet))
		return;

	emit propertyWillChange(QtnPropertyChangeReasonStateLocal,
							QtnPropertyValuePtr(&stateToSet),
							qMetaTypeId<QtnPropertyState>());

	m_stateLocal = stateToSet;

	emit propertyDidChange(QtnPropertyChangeReasonStateLocal);

	updateStateInherited(force);
}

void QtnPropertyBase::removeState(QtnPropertyState stateToRemove, bool force)
{
	QtnPropertyState stateToSet = m_stateLocal&~stateToRemove;

	if (!force && (m_stateLocal == stateToSet))
		return;

	emit propertyWillChange(QtnPropertyChangeReasonStateLocal,
							QtnPropertyValuePtr(&stateToSet),
							qMetaTypeId<QtnPropertyState>());

	m_stateLocal = stateToSet;

	emit propertyDidChange(QtnPropertyChangeReasonStateLocal);

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

bool QtnPropertyBase::isEditableByUser() const
{
	return !(state()&(QtnPropertyStateImmutable|QtnPropertyStateInvisible));
}

bool QtnPropertyBase::isVisible() const
{
	return !(state()&(QtnPropertyStateInvisible));
}

bool QtnPropertyBase::valueIsHidden() const
{
	return 0 != (m_stateLocal & QtnPropertyStateHiddenValue);
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

bool QtnPropertyBase::fromStr(const QString& str, bool edit)
{
	if (!isEditableByUser())
		return false;

	return fromStrImpl(str, edit);
}

bool QtnPropertyBase::toStr(QString& str) const
{
	return toStrImpl(str);
}

bool QtnPropertyBase::fromVariant(const QVariant& var, bool edit)
{
	if (!isEditableByUser())
		return false;

	return fromVariantImpl(var, edit);
}

bool QtnPropertyBase::toVariant(QVariant& var) const
{
	return toVariantImpl(var);
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

bool QtnPropertyBase::fromVariantImpl(const QVariant& var, bool edit)
{
	if (var.canConvert<QString>())
		return fromStr(var.value<QString>(), edit);
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

void QtnPropertyBase::connectMasterState(QtnPropertyBase *masterProperty)
{
	Q_ASSERT(nullptr != masterProperty);

	disconnectMasterState();

	m_masterProperty = masterProperty;

	setStateInherited(masterProperty->state());

	QObject::connect(masterProperty, &QObject::destroyed,
					 this, &QtnPropertyBase::onMasterPropertyDestroyed);
	QObject::connect(masterProperty, &QtnPropertyBase::propertyDidChange,
					 this, &QtnPropertyBase::masterPropertyStateDidChange);
}

void QtnPropertyBase::disconnectMasterState()
{
	if (nullptr != m_masterProperty)
	{
		QObject::disconnect(m_masterProperty, &QObject::destroyed,
							this, &QtnPropertyBase::onMasterPropertyDestroyed);
		QObject::disconnect(m_masterProperty, &QtnPropertyBase::propertyDidChange,
							this, &QtnPropertyBase::masterPropertyStateDidChange);

		m_masterProperty = nullptr;
	}
}

void QtnPropertyBase::postUpdateEvent(QtnPropertyChangeReason reason)
{
	changeReasons.store(changeReasons | reason);
	if (timer == 0)
		timer = startTimer(20);
}

void QtnPropertyBase::setStateInherited(QtnPropertyState stateToSet, bool force)
{
	if (!force && (m_stateInherited == stateToSet))
		return;

	emit propertyWillChange(QtnPropertyChangeReasonStateInherited,
							QtnPropertyValuePtr(&stateToSet),
							qMetaTypeId<QtnPropertyState>());

	m_stateInherited = stateToSet;

	emit propertyDidChange(QtnPropertyChangeReasonStateInherited);

	updateStateInherited(force);
}

void QtnPropertyBase::masterPropertyStateDidChange(QtnPropertyChangeReason reason)
{
	if (reason & QtnPropertyChangeReasonState)
	{
		auto changedProperty = dynamic_cast<QtnPropertyBase *>(sender());
		Q_ASSERT(nullptr != changedProperty);
		setStateInherited(changedProperty->state());
	}
}

QVariant QtnPropertyBase::valueAsVariant() const
{
	QVariant result;
	toVariant(result);
	return result;
}

void QtnPropertyBase::setValueAsVariant(const QVariant& value)
{
	fromVariant(value, false);
}

void QtnPropertyBase::onMasterPropertyDestroyed(QObject *object)
{
	Q_ASSERT(object == m_masterProperty);
	m_masterProperty = nullptr;
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

void QtnPropertyBase::setExpanded(bool expanded)
{
	if (expanded)
		expand();
	else
		collapse();
}

void QtnPropertyBase::setCollapsed(bool collapsed)
{
	if (collapsed)
		collapse();
	else
		expand();
}

QtnPropertyConnector *QtnPropertyBase::getConnector() const
{
	return findChild<QtnPropertyConnector *>(QString(), Qt::FindDirectChildrenOnly);
}

