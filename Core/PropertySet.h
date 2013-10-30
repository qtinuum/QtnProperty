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

#ifndef QTN_PROPERTY_SET_H
#define QTN_PROPERTY_SET_H

#include "Property.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertySet: public PropertyBase
{
    Q_OBJECT
    Q_DISABLE_COPY(PropertySet)

public:
    explicit PropertySet(QObject* parent);
    virtual ~PropertySet();

    // sub properties
    bool hasChildProperties() const { return !m_childProperties.empty(); }
    const QList<PropertyBase*>& childProperties() const { return m_childProperties; }
    QList<PropertyBase*> findChildProperties(const QString& name, Qt::FindChildOptions options = Qt::FindChildrenRecursively);
    QList<PropertyBase*> findChildProperties(const QRegularExpression& re, Qt::FindChildOptions options = Qt::FindChildrenRecursively);
    PropertyBase* findChildProperty(PropertyID id);
    void clearChildProperties();
    bool addChildProperty(PropertyBase* childProperty, bool moveOwnership = true);
    bool removeChildProperty(PropertyBase* childProperty);

    // cloning
    PropertySet* createNew(QObject* parentForNew) const;
    PropertySet* createCopy(QObject* parentForCopy) const;

    // copy values
    bool copyValues(PropertySet* propertySetCopyFrom, PropertyState ignoreMask);

    // casts
    PropertySet* asPropertySet() override { return this; }
    const PropertySet* asPropertySet() const override { return this; }

protected:
    void updateStateInherited(bool force) override;

    // cloning implementation
    virtual PropertySet* createNewImpl(QObject* parentForNew) const { return nullptr; }
    virtual PropertySet* createCopyImpl(QObject* parentForCopy) const { return nullptr; }

    // copy values
    virtual bool copyValuesImpl(PropertySet* propertySetCopyFrom, PropertyState ignoreMask) { return false; }

    // serialization implementation
    bool loadImpl(QDataStream& stream) override;
    bool saveImpl(QDataStream& stream) const override;

private:
    void findChildPropertiesRecursive(const QString& name, QList<PropertyBase*>& result);
    void findChildPropertiesRecursive(const QRegularExpression& re, QList<PropertyBase*>& result);

    void childPropertyWillChange(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason, PropertyValuePtr newValue);
    void childPropertyDidChange(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason);

private:
    QList<PropertyBase*> m_childProperties;

    bool m_ignoreChildPropertyChanges;

    friend void connectChildProperty(PropertySet* masterProperty, PropertyBase* childProperty);
    friend void disconnectChildProperty(PropertySet* masterProperty, PropertyBase* childProperty);
};

} // end namespace Qtinuum

#endif // QTN_PROPERTY_SET_H
