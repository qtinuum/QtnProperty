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

#ifndef QTN_PROPERTY_BASE_H
#define QTN_PROPERTY_BASE_H

#include "Auxiliary/PropertyAux.h"
#include <QDataStream>
#include <QVariant>

namespace Qtinuum
{

class PropertySet;
class Property;
class PropertyVisitor;

class QTN_PE_CORE_EXPORT PropertyBase: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(PropertyBase)

public:
    virtual ~PropertyBase();

    QString name() const { return objectName(); }
    void setName(const QString& name);

    QString description() const { return m_description; }
    void setDescription(const QString& description);

    PropertyID id() const { return m_id; }
    void setId(PropertyID id);

    // states
    PropertyState state() const { return m_stateLocal|m_stateInherited; }
    PropertyState stateLocal() const { return m_stateLocal; }
    PropertyState stateInherited() const { return m_stateInherited; }

    void setState(PropertyState stateToSet, bool force = false);
    void addState(PropertyState stateToAdd, bool force = false);
    void removeState(PropertyState stateToRemove, bool force = false);
    void switchState(PropertyState stateToSwitch, bool switchOn, bool force = false);
    void switchStateAuto(PropertyState stateToSwitch, bool force = false);

    bool isEditableByUser() const;
    bool isVisible() const;
    bool isSimple() const { return !m_stateLocal.testFlag(PropertyStateNonSimple); }

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
    virtual Property* asProperty() { return nullptr; }
    virtual const Property* asProperty() const { return nullptr; }
    virtual PropertySet* asPropertySet() { return nullptr; }
    virtual const PropertySet* asPropertySet() const { return nullptr; }

    static QMetaObject::Connection connectMasterState(const PropertyBase& masterProperty, PropertyBase& slaveProperty);
    static bool disconnectMasterState(const PropertyBase& masterProperty, PropertyBase& slaveProperty);

public: // properties
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(PropertyID id READ id)
    Q_PROPERTY(PropertyState state READ state)
    Q_PROPERTY(QVariant value READ valueAsVariant WRITE setValueAsVariant)

Q_SIGNALS:
    void propertyWillChange(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason, PropertyValuePtr newValue);
    void propertyDidChange(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason);

protected:
    PropertyBase(QObject* parent);

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
    void setStateInherited(PropertyState stateToSet, bool force = false);

private:
    void masterPropertyStateDidChange(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason);
    QVariant valueAsVariant() const;
    void setValueAsVariant(const QVariant& value);

    QString m_description;
    PropertyID m_id;

    PropertyState m_stateLocal;
    PropertyState m_stateInherited;

    friend class PropertySet;
};

QTN_PE_CORE_EXPORT QDataStream& operator<< (QDataStream& stream, const PropertyBase& property);
QTN_PE_CORE_EXPORT QDataStream& operator>> (QDataStream& stream, PropertyBase& property);

} // end namespace Qtinuum

#endif // QTN_PROPERTY_BASE_H
