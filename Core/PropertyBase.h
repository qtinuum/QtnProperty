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

#ifndef QTN_PROPERTY_BASE_H
#define QTN_PROPERTY_BASE_H

#include "Auxiliary/PropertyAux.h"
#include <QDataStream>
#include <QVariant>

class QScriptEngine;
class QtnPropertySet;
class QtnProperty;

class QTN_PE_CORE_EXPORT QtnPropertyBase: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QtnPropertyBase)

public:
    virtual ~QtnPropertyBase();

    QString name() const { return objectName(); }
    void setName(const QString& name);

    QString description() const { return m_description; }
    void setDescription(const QString& description);

    QtnPropertyID id() const { return m_id; }
    void setId(QtnPropertyID id);

    // states
    QtnPropertyState state() const { return m_stateLocal|m_stateInherited; }
    QtnPropertyState stateLocal() const { return m_stateLocal; }
    QtnPropertyState stateInherited() const { return m_stateInherited; }

    void setState(QtnPropertyState stateToSet, bool force = false);
    void addState(QtnPropertyState stateToAdd, bool force = false);
    void removeState(QtnPropertyState stateToRemove, bool force = false);
    void switchState(QtnPropertyState stateToSwitch, bool switchOn, bool force = false);
    void switchStateAuto(QtnPropertyState stateToSwitch, bool force = false);

    bool isEditableByUser() const;
    bool isVisible() const;
    bool isSimple() const { return !m_stateLocal.testFlag(QtnPropertyStateNonSimple); }

    // serialization
    bool load(QDataStream& stream);
    bool save(QDataStream& stream) const;
    static bool skipLoad(QDataStream& stream);

    // string conversion
    bool fromStr(const QString& str);
    bool toStr(QString& str) const;

    // variant conversion
    bool fromVariant(const QVariant& var);
    bool toVariant(QVariant& var) const;

    // casts
    virtual QtnProperty* asProperty() { return nullptr; }
    virtual const QtnProperty* asProperty() const { return nullptr; }
    virtual QtnPropertySet* asPropertySet() { return nullptr; }
    virtual const QtnPropertySet* asPropertySet() const { return nullptr; }

    static QMetaObject::Connection connectMasterState(const QtnPropertyBase& masterProperty, QtnPropertyBase& slaveProperty);
    static bool disconnectMasterState(const QtnPropertyBase& masterProperty, QtnPropertyBase& slaveProperty);

public: // properties for scripting
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(qint32 id READ id)
    Q_PROPERTY(bool isEditable READ isEditableByUser)
    Q_PROPERTY(quint32 state READ state)
    Q_PROPERTY(QVariant value READ valueAsVariant WRITE setValueAsVariant)

Q_SIGNALS:
    void propertyWillChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue);
    void propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);

protected:
    QtnPropertyBase(QObject* parent);

    // serialization implementation
    virtual bool loadImpl(QDataStream& stream);
    virtual bool saveImpl(QDataStream& stream) const;

    // string conversion implementation
    virtual bool fromStrImpl(const QString& str) { return false; }
    virtual bool toStrImpl(QString& str) const { return false; }

    // variant conversion implementation
    virtual bool fromVariantImpl(const QVariant& var);
    virtual bool toVariantImpl(QVariant& var) const;

    // inherited states support
    virtual void updateStateInherited(bool force) { /* does nothing by default */ }
    void setStateInherited(QtnPropertyState stateToSet, bool force = false);

private:
    void masterPropertyStateDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);

    // getter/setter for "value" property
    QVariant valueAsVariant() const;
    void setValueAsVariant(const QVariant& value);

    QString m_description;
    QtnPropertyID m_id;

    QtnPropertyState m_stateLocal;
    QtnPropertyState m_stateInherited;

    friend class QtnPropertySet;
};

QTN_PE_CORE_EXPORT QDataStream& operator<< (QDataStream& stream, const QtnPropertyBase& property);
QTN_PE_CORE_EXPORT QDataStream& operator>> (QDataStream& stream, QtnPropertyBase& property);

QTN_PE_CORE_EXPORT void qtnScriptRegisterPropertyTypes(QScriptEngine* engine);

Q_DECLARE_METATYPE(const QtnPropertyBase*)
Q_DECLARE_METATYPE(QtnPropertyBase*)

#endif // QTN_PROPERTY_BASE_H
