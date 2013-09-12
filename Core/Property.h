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

#ifndef QTN_PROPERTY_H
#define QTN_PROPERTY_H

#include "PropertyAux.h"
#include <QDataStream>
#include <functional>

namespace Qtinuum
{

class PropertyDelegateInfoGetter;

class QTN_PE_CORE_EXPORT Property: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Property)

public:
    explicit Property(QObject *parent);
    virtual ~Property();

    QString name() const { return objectName(); }
    void setName(const QString &name);

    QString description() const { return m_description; }
    void setDescription(const QString &description);

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

    QMetaObject::Connection connectMasterState(const Property &masterProperty);
    bool disconnectMasterState(const Property &masterProperty);

    bool isEditableByUser() const;
    bool isVisible() const;
    bool isSimple() const { return !m_stateLocal.testFlag(PropertyStateNonSimple); }

    // sub properties
    bool hasChildProperties() const { return !m_childProperties.empty(); }
    const QList<Property*> &childProperties() const { return m_childProperties; }
    QList<Property*> findChildProperties(const QString &name, Qt::FindChildOptions options = Qt::FindChildrenRecursively);
    QList<Property*> findChildProperties(const QRegularExpression &re, Qt::FindChildOptions options = Qt::FindChildrenRecursively);
    Property *findChildProperty(PropertyID id);
    void clearChildProperties();
    bool addChildProperty(Property *childProperty, bool moveOwnership = true);
    bool removeChildProperty(Property *childProperty);

    // delegates
    const PropertyDelegateInfo *delegate() const;
    void setDelegate(const PropertyDelegateInfo &delegate);
    void setDelegateCallback(const std::function<const PropertyDelegateInfo *()> &callback);

    // cloning
    Property* createNew(QObject* parentForNew) const;
    Property* createCopy(QObject* parentForCopy) const;

    // serialization
    bool load(QDataStream &stream);
    bool save(QDataStream &stream) const;

Q_SIGNALS:
    void propertyWillChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason, PropertyValuePtr newValue);
    void propertyDidChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason);
    void propertyValueAccept(const Property *property, PropertyValuePtr valueToAccept, bool* accept);

protected:
    // cloning implementation
    Property(QObject *parent, const Property &other);
    virtual Property* createNewImpl(QObject* parentForNew) const;
    virtual Property* createCopyImpl(QObject* parentForCopy) const;

    // serialization implementation
    virtual bool loadContentImpl(QDataStream &stream);
    virtual bool saveContentImpl(QDataStream &stream) const;

    static bool skipLoad(QDataStream &stream);

private:
    void setStateInherited(PropertyState stateToSet, bool force = false);

    void findChildPropertiesRecursive(const QString &name, QList<Property*> &result);
    void findChildPropertiesRecursive(const QRegularExpression &re, QList<Property*> &result);

    void childPropertyWillChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason, PropertyValuePtr newValue);
    void childPropertyDidChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason);
    void masterPropertyStateDidChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason);

private:
    QString m_description;
    PropertyID m_id;

    PropertyState m_stateLocal;
    PropertyState m_stateInherited;

    QList<Property*> m_childProperties;

    QScopedPointer<PropertyDelegateInfoGetter> m_delegateInfoGetter;

    bool m_ignoreChildPropertyChanges;

    friend void connectChildProperty(Property *masterProperty, Property *childProperty);
    friend void disconnectChildProperty(Property *masterProperty, Property *childProperty);
};

QTN_PE_CORE_EXPORT QDataStream& operator<< (QDataStream& stream, const Property& property);
QTN_PE_CORE_EXPORT QDataStream& operator>> (QDataStream& stream, Property& property);

} // end namespace Qtinuum

#endif // QTN_PROPERTY_H
