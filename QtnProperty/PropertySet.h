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

#ifndef QTN_PROPERTY_SET_H
#define QTN_PROPERTY_SET_H

#include "Property.h"
#include <QJsonObject>

class QTN_PE_CORE_EXPORT QtnPropertySet: public QtnPropertyBase
{
    Q_OBJECT
    Q_DISABLE_COPY(QtnPropertySet)

public:
    explicit QtnPropertySet(QObject* parent);
    ~QtnPropertySet() override;

public slots:
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

    // JSON support
    bool fromJson(const QJsonObject& jsonObject);
    bool toJson(QJsonObject& jsonObject) const;

protected:
    void updateStateInherited(bool force) override;

    // cloning implementation
    virtual QtnPropertySet* createNewImpl(QObject* parentForNew) const { Q_UNUSED(parentForNew); return nullptr; }
    virtual QtnPropertySet* createCopyImpl(QObject* parentForCopy) const { Q_UNUSED(parentForCopy); return nullptr; }

    // copy values
    virtual bool copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask) { Q_UNUSED(propertySetCopyFrom); Q_UNUSED(ignoreMask); return false; }

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

    bool toStrWithPrefix(QString& str, const QString& prefix) const;

private:
    QList<QtnPropertyBase*> m_childProperties;

    bool m_ignoreChildPropertyChanges = false;

    friend void qtnConnectChildProperty(QtnPropertySet* masterProperty, QtnPropertyBase* childProperty);
    friend void qtnDisconnectChildProperty(QtnPropertySet* masterProperty, QtnPropertyBase* childProperty);
};

Q_DECLARE_METATYPE(QtnPropertySet*)

template <typename T>
T* qtnCreateProperty(QtnPropertySet* parent, QString name)
{
    auto property = new T(parent);
    property->setName(name);
    parent->addChildProperty(property);
    return property;
}

template <typename T>
T* qtnCreateProperty(QtnPropertySet* parent)
{
    auto property = new T(parent);
    parent->addChildProperty(property);
    return property;
}

#endif // QTN_PROPERTY_SET_H
