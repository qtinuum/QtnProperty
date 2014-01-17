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

class QTN_PE_CORE_EXPORT QtnPropertySet: public QtnPropertyBase
{
    Q_OBJECT
    Q_DISABLE_COPY(QtnPropertySet)

public:
    explicit QtnPropertySet(QObject* parent);
    virtual ~QtnPropertySet();

    // sub properties
    bool hasChildProperties() const { return !m_childProperties.empty(); }
    const QList<QtnPropertyBase*>& childProperties() const { return m_childProperties; }
    QList<QtnPropertyBase*> findChildProperties(QString name, Qt::FindChildOptions options = Qt::FindChildrenRecursively);
    QList<QtnPropertyBase*> findChildProperties(const QRegularExpression& re, Qt::FindChildOptions options = Qt::FindChildrenRecursively);
    QtnPropertyBase* findChildProperty(QtnPropertyID id);
    void clearChildProperties();
    bool addChildProperty(QtnPropertyBase* childProperty, bool moveOwnership = true);
    bool removeChildProperty(QtnPropertyBase* childProperty);

    // cloning
    QtnPropertySet* createNew(QObject* parentForNew) const;
    QtnPropertySet* createCopy(QObject* parentForCopy) const;

    // copy values
    bool copyValues(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask = QtnPropertyStateNone);

    // casts
    QtnPropertySet* asPropertySet() override { return this; }
    const QtnPropertySet* asPropertySet() const override { return this; }

protected:
    void updateStateInherited(bool force) override;

    // cloning implementation
    virtual QtnPropertySet* createNewImpl(QObject* parentForNew) const { return nullptr; }
    virtual QtnPropertySet* createCopyImpl(QObject* parentForCopy) const { return nullptr; }

    // copy values
    virtual bool copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask) { return false; }

    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    // serialization implementation
    bool loadImpl(QDataStream& stream) override;
    bool saveImpl(QDataStream& stream) const override;

private:
    void findChildPropertiesRecursive(const QString& name, QList<QtnPropertyBase*>& result);
    void findChildPropertiesRecursive(const QRegularExpression& re, QList<QtnPropertyBase*>& result);

    void childPropertyWillChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue);
    void childPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);

private:
    QList<QtnPropertyBase*> m_childProperties;

    bool m_ignoreChildPropertyChanges;

    friend void qtnConnectChildProperty(QtnPropertySet* masterProperty, QtnPropertyBase* childProperty);
    friend void qtnDisconnectChildProperty(QtnPropertySet* masterProperty, QtnPropertyBase* childProperty);
};

#endif // QTN_PROPERTY_SET_H
