/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#ifndef QTN_PROPERTY_BASE_H
#define QTN_PROPERTY_BASE_H

#include "Auxiliary/PropertyAux.h"
#include <QDataStream>
#include <QVariant>

#include <atomic>

class QScriptEngine;
class QtnPropertySet;
class QtnProperty;
class QtnPropertyConnector;

class QTN_IMPORT_EXPORT QtnPropertyBase
	: public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnPropertyBase)

public:
	virtual ~QtnPropertyBase();

	inline QString name() const;
	void setName(const QString &name);

	inline QString description() const;
	void setDescription(const QString &description);

	inline QtnPropertyID id() const;
	void setId(QtnPropertyID id);

	inline bool isExpanded() const;
	void setExpanded(bool expanded);

	inline bool isCollapsed() const;
	void setCollapsed(bool collapsed);

	inline void expand();
	inline void collapse();

	QtnPropertyConnector *getConnector() const;
	inline bool isQObjectProperty() const;

	// states
	inline QtnPropertyState state() const;
	inline QtnPropertyState stateLocal() const;
	inline QtnPropertyState stateInherited() const;

	void setState(QtnPropertyState stateToSet, bool force = false);
	void addState(QtnPropertyState stateToAdd, bool force = false);
	void removeState(QtnPropertyState stateToRemove, bool force = false);
	void switchState(
		QtnPropertyState stateToSwitch, bool switchOn, bool force = false);
	void switchStateAuto(QtnPropertyState stateToSwitch, bool force = false);

	bool isEditableByUser() const;
	bool isVisible() const;
	bool valueIsHidden() const;
	bool valueIsDefault() const;
	bool isSimple() const;

	// serialization
	bool load(QDataStream &stream);
	bool save(QDataStream &stream) const;
	static bool skipLoad(QDataStream &stream);

	// string conversion
	bool fromStr(const QString &str, bool edit);
	bool toStr(QString &str) const;

	// variant conversion
	bool fromVariant(const QVariant &var, bool edit);
	bool toVariant(QVariant &var) const;

	// casts
	virtual QtnProperty *asProperty();
	virtual const QtnProperty *asProperty() const;
	virtual QtnPropertySet *asPropertySet();
	virtual const QtnPropertySet *asPropertySet() const;

	inline QtnPropertyBase *getMasterProperty() const;
	QtnPropertyBase *getRootProperty();
	QtnPropertySet *getRootPropertySet();
	virtual void connectMasterState(QtnPropertyBase *masterProperty);
	virtual void disconnectMasterState();

	void postUpdateEvent(QtnPropertyChangeReason reason);

public:
	// properties for scripting
	Q_PROPERTY(QString name READ name)
	Q_PROPERTY(QString description READ description)
	Q_PROPERTY(qint32 id READ id)
	Q_PROPERTY(bool isEditable READ isEditableByUser)
	Q_PROPERTY(quint32 state READ state)
	Q_PROPERTY(QVariant value READ valueAsVariant WRITE setValueAsVariant)

	// getter/setter for "value" property
	QVariant valueAsVariant() const;
	void setValueAsVariant(const QVariant &value);

Q_SIGNALS:
	void propertyWillChange(
		QtnPropertyChangeReason reason,
		QtnPropertyValuePtr newValue, int typeId);
	void propertyDidChange(QtnPropertyChangeReason reason);

private slots:
	void onMasterPropertyDestroyed(QObject *object);

protected:
	QtnPropertyBase(QObject *parent);

	virtual void masterPropertyStateDidChange(QtnPropertyChangeReason reason);

	virtual bool event(QEvent *e) override;

	// serialization implementation
	virtual bool loadImpl(QDataStream &stream);
	virtual bool saveImpl(QDataStream &stream) const;

	// string conversion implementation
	virtual bool fromStrImpl(const QString &, bool);
	virtual bool toStrImpl(QString &str) const;

	// variant conversion implementation
	virtual bool fromVariantImpl(const QVariant &var, bool edit);
	virtual bool toVariantImpl(QVariant &var) const;

	// inherited states support
	virtual void updateStateInherited(bool force);
	void setStateInherited(QtnPropertyState stateToSet, bool force = false);

private:
	QtnPropertyBase *m_masterProperty;

	QString m_description;
	QtnPropertyID m_id;

	QtnPropertyState m_stateLocal;
	QtnPropertyState m_stateInherited;

	int changeReasons;
	QEvent *updateEvent;

	friend class QtnPropertySet;
};

QString QtnPropertyBase::name() const
{
	return objectName();
}

QString QtnPropertyBase::description() const
{
	return m_description;
}

QtnPropertyID QtnPropertyBase::id() const
{
	return m_id;
}

bool QtnPropertyBase::isExpanded() const
{
	return (0 == (m_stateLocal & QtnPropertyStateCollapsed));
}

bool QtnPropertyBase::isCollapsed() const
{
	return (0 != (m_stateLocal & QtnPropertyStateCollapsed));
}

void QtnPropertyBase::expand()
{
	removeState(QtnPropertyStateCollapsed);
}

void QtnPropertyBase::collapse()
{
	addState(QtnPropertyStateCollapsed);
}

bool QtnPropertyBase::isQObjectProperty() const
{
	return (nullptr != getConnector());
}

QtnPropertyState QtnPropertyBase::state() const
{
	return m_stateLocal | m_stateInherited;
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

#endif	// QTN_PROPERTY_BASE_H
