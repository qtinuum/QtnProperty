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

#ifndef QTN_PROPERTY_BASE_H
#define QTN_PROPERTY_BASE_H

#include "Auxiliary/PropertyAux.h"
#include "Auxiliary/PropertyDelegateInfo.h"
#include <QDataStream>
#include <QVariant>
#include <functional>

class QScriptEngine;
class QtnPropertySet;
class QtnProperty;
class QtnPropertyConnector;
class QtnPropertyDelegateInfoGetter;

class QTN_IMPORT_EXPORT QtnPropertyBase : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnPropertyBase)

	Q_PROPERTY(QString name READ name)
	Q_PROPERTY(QString displayName READ displayName)
	Q_PROPERTY(QString description READ description)
	Q_PROPERTY(qint32 id READ id)
	Q_PROPERTY(bool isEditable READ isWritable)
	Q_PROPERTY(bool isEditableByUser READ isEditableByUser)
	Q_PROPERTY(quint32 state READ state)
	Q_PROPERTY(QVariant value READ valueAsVariant WRITE fromVariant)

	friend class QtnPropertyConnector;
	friend class QtnPropertySet;

	inline void setConnector(QtnPropertyConnector *connector);

public:
	using DelegateInfoCallback = std::function<QtnPropertyDelegateInfo()>;

	virtual ~QtnPropertyBase() override;

	virtual const QMetaObject *propertyMetaObject() const;

	inline QString name() const;
	void setName(const QString &name);

	inline QString displayName() const;
	void setDisplayName(const QString &displayName);

	inline QString description() const;
	void setDescription(const QString &description);

	inline QtnPropertyID id() const;
	void setId(QtnPropertyID id);

	bool isExpanded() const;
	void setExpanded(bool expanded);

	bool isCollapsed() const;
	void setCollapsed(bool collapsed);

	bool isResettable() const;
	void reset(QtnPropertyChangeReason reason = QtnPropertyChangeReason());

	bool isWritable() const;

	inline void expand();
	inline void collapse();

	inline QtnPropertyConnector *getConnector() const;
	inline bool isQObjectProperty() const;

	// states
	QtnPropertyState state() const;
	inline QtnPropertyState stateLocal() const;
	inline QtnPropertyState stateInherited() const;

	void setState(QtnPropertyState stateToSet, bool force = false);
	void addState(QtnPropertyState stateToAdd, bool force = false);
	void removeState(QtnPropertyState stateToRemove, bool force = false);
	void switchState(
		QtnPropertyState stateToSwitch, bool switchOn, bool force = false);
	void toggleState(QtnPropertyState stateToSwitch, bool force = false);

	bool isEditableByUser() const;
	bool isVisible() const;
	bool isMultiValue() const;
	bool valueIsDefault() const;
	bool isSimple() const;

	// serialization
	bool load(QDataStream &stream);
	bool save(QDataStream &stream) const;
	static bool skipLoad(QDataStream &stream);

	// string conversion
	bool fromStr(const QString &str,
		QtnPropertyChangeReason reason = QtnPropertyChangeReasonNewValue);
	bool toStr(QString &str) const;

	// variant conversion
	bool fromVariant(const QVariant &var,
		QtnPropertyChangeReason reason = QtnPropertyChangeReasonNewValue);
	bool toVariant(QVariant &var) const;

	// casts
	virtual QtnProperty *asProperty();
	virtual const QtnProperty *asProperty() const;
	virtual QtnPropertySet *asPropertySet();
	virtual const QtnPropertySet *asPropertySet() const;

	inline QtnPropertyBase *getMasterProperty() const;
	QtnPropertyBase *getRootProperty();
	QtnPropertySet *getRootPropertySet();
	void connectMasterState(QtnPropertyBase *masterProperty);
	void disconnectMasterState();
	void postUpdateEvent(QtnPropertyChangeReason reason, int afterMS = 0);

	// getter/setter for "value" property
	QVariant valueAsVariant() const;
	// delegates
	const QtnPropertyDelegateInfo *delegateInfo() const;
	void setDelegateInfo(const QtnPropertyDelegateInfo &delegateInfo);
	void setDelegateInfoCallback(const DelegateInfoCallback &callback);

	void setDelegateAttribute(
		const QByteArray &attributeName, const QVariant &attributeValue);

Q_SIGNALS:
	void propertyWillChange(QtnPropertyChangeReason reason,
		QtnPropertyValuePtr newValue, int typeId);
	void propertyDidChange(QtnPropertyChangeReason reason);

protected:
	QtnPropertyBase(QObject *parent);

	virtual void doReset(QtnPropertyChangeReason reason);
	virtual bool event(QEvent *e) override;

	// serialization implementation
	virtual bool loadImpl(QDataStream &stream);
	virtual bool saveImpl(QDataStream &stream) const;

	// string conversion implementation
	virtual bool fromStrImpl(const QString &, QtnPropertyChangeReason reason);
	virtual bool toStrImpl(QString &str) const;

	// variant conversion implementation
	virtual bool fromVariantImpl(
		const QVariant &var, QtnPropertyChangeReason reason);
	virtual bool toVariantImpl(QVariant &var) const;

	// inherited states support
	virtual void updateStateInherited(bool force);
	void setStateInherited(QtnPropertyState stateToSet, bool force = false);

private:
	QtnPropertyState masterPropertyState() const;
	void onMasterPropertyDestroyed(QObject *object);
	void masterPropertyStateWillChange(QtnPropertyChangeReason reason);
	void masterPropertyStateDidChange(QtnPropertyChangeReason reason);
	void beforeUpdateStateFromMasterProperty();
	void doUpdateStateFromMasterProperty();

private:
	QtnPropertyConnector *mPropertyConnector;
	QtnPropertyBase *m_masterProperty;

	QString m_displayName;
	QString m_description;
	QtnPropertyID m_id;

	QtnPropertyState m_stateLocal;
	QtnPropertyState m_stateInherited;

	int changeReasons;
	int timer;
	QEvent *updateEvent;
	QScopedPointer<QtnPropertyDelegateInfoGetter> m_delegateInfoGetter;
};

QString QtnPropertyBase::name() const
{
	return objectName();
}

QString QtnPropertyBase::displayName() const
{
	return m_displayName;
}

QString QtnPropertyBase::description() const
{
	return m_description;
}

QtnPropertyID QtnPropertyBase::id() const
{
	return m_id;
}

void QtnPropertyBase::expand()
{
	removeState(QtnPropertyStateCollapsed);
}

void QtnPropertyBase::collapse()
{
	addState(QtnPropertyStateCollapsed);
}

void QtnPropertyBase::setConnector(QtnPropertyConnector *connector)
{
	mPropertyConnector = connector;
}

QtnPropertyConnector *QtnPropertyBase::getConnector() const
{
	return mPropertyConnector;
}

bool QtnPropertyBase::isQObjectProperty() const
{
	return (nullptr != getConnector());
}

QtnPropertyState QtnPropertyBase::stateLocal() const
{
	return m_stateLocal;
}

QtnPropertyState QtnPropertyBase::stateInherited() const
{
	return m_stateInherited;
}

QtnPropertyBase *QtnPropertyBase::getMasterProperty() const
{
	return m_masterProperty;
}

QTN_IMPORT_EXPORT QDataStream &operator<<(
	QDataStream &stream, const QtnPropertyBase &property);
QTN_IMPORT_EXPORT QDataStream &operator>>(
	QDataStream &stream, QtnPropertyBase &property);

QTN_IMPORT_EXPORT void qtnScriptRegisterPropertyTypes(QScriptEngine *engine);

Q_DECLARE_METATYPE(const QtnPropertyBase *)
Q_DECLARE_METATYPE(QtnPropertyBase *)

#endif // QTN_PROPERTY_BASE_H
