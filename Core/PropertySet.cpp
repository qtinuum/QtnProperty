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

#include "PropertySet.h"
#include <QRegularExpression>

static int qtnPropertySetPtrId = qRegisterMetaType<QtnPropertySet*>("QtnPropertySet*");


void qtnAddPropertyAsChild(QObject *parent, QtnPropertyBase* child, bool moveOwnership)
{
    QtnPropertySet* propertySet = qobject_cast<QtnPropertySet*>(parent);
    if (propertySet)
        propertySet->addChildProperty(child, moveOwnership);
}

void qtnRemovePropertyAsChild(QObject* parent, QtnPropertyBase* child)
{
    QtnPropertySet* propertySet = qobject_cast<QtnPropertySet*>(parent);
    if (propertySet)
        propertySet->removeChildProperty(child);
}

void qtnConnectChildProperty(QtnPropertySet* masterProperty, QtnPropertyBase* childProperty)
{
    QObject::connect(childProperty, &QtnPropertyBase::propertyWillChange, masterProperty, &QtnPropertySet::childPropertyWillChange);
    QObject::connect(childProperty, &QtnPropertyBase::propertyDidChange, masterProperty, &QtnPropertySet::childPropertyDidChange);
}

void qtnDisconnectChildProperty(QtnPropertySet* masterProperty, QtnPropertyBase* childProperty)
{
    QObject::disconnect(childProperty, &QtnPropertyBase::propertyWillChange, masterProperty, &QtnPropertySet::childPropertyWillChange);
    QObject::disconnect(childProperty, &QtnPropertyBase::propertyDidChange, masterProperty, &QtnPropertySet::childPropertyDidChange);
}

QtnPropertySet::QtnPropertySet(QObject* parent)
    : QtnPropertyBase(parent)
{
}

QtnPropertySet::~QtnPropertySet()
{
    clearChildProperties();
}

QList<QtnPropertyBase*> QtnPropertySet::findChildProperties(QString name, Qt::FindChildOptions options)
{
    QList<QtnPropertyBase*> result;

    // normilize name
    name = name.trimmed();

    // if name is dot separated property path
    if (name.contains('.'))
    {
        QString nameHead = name.section('.', 0, 0);
        if (nameHead.isEmpty())
            return result;

        QString nameTail = name.section('.', 1);
        if (nameTail.isEmpty())
            return result;

        QList<QtnPropertyBase*> headResult = findChildProperties(nameHead, options);
        foreach (QtnPropertyBase* headProperty, headResult)
        {
            QtnPropertySet* headPropertySet = headProperty->asPropertySet();
            if (!headPropertySet)
                continue;

            result.append(headPropertySet->findChildProperties(nameTail, options));
        }
    }
    else
    {
        foreach(QtnPropertyBase* childProperty, m_childProperties)
        {
            if (childProperty->name() == name)
                result.append(childProperty);
        }

        if (options & Qt::FindChildrenRecursively)
        {
            foreach(QtnPropertyBase* childProperty, m_childProperties)
            {
                QtnPropertySet* propertySet = childProperty->asPropertySet();
                if (propertySet)
                    propertySet->findChildPropertiesRecursive(name, result);
            }
        }
    }

    return result;
}

QList<QtnPropertyBase*> QtnPropertySet::findChildProperties(const QRegularExpression& re, Qt::FindChildOptions options)
{
    QList<QtnPropertyBase*> result;

    foreach(QtnPropertyBase* childProperty, m_childProperties)
    {
        if (re.match(childProperty->name()).isValid())
            result.append(childProperty);
    }

    if (options & Qt::FindChildrenRecursively)
    {
        foreach(QtnPropertyBase* childProperty, m_childProperties)
        {
            QtnPropertySet* propertySet = childProperty->asPropertySet();
            if (propertySet)
                propertySet->findChildPropertiesRecursive(re, result);
        }
    }

    return result;
}

QtnPropertyBase* QtnPropertySet::findChildProperty(QtnPropertyID id)
{
    foreach(QtnPropertyBase* childProperty, m_childProperties)
    {
        if (childProperty->id() == id)
            return childProperty;
    }

    return nullptr;
}

void QtnPropertySet::clearChildProperties()
{
    if (m_childProperties.isEmpty())
        return;

    foreach(QtnPropertyBase* childProperty, m_childProperties)
    {
        qtnDisconnectChildProperty(this, childProperty);
    }

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonChildPropertyRemove, QtnPropertyValuePtr(0));

    m_childProperties.clear();

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonChildPropertyRemove);
}

bool QtnPropertySet::addChildProperty(QtnPropertyBase* childProperty, bool moveOwnership)
{
    Q_CHECK_PTR(childProperty);

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonChildPropertyAdd, QtnPropertyValuePtr(childProperty));

    m_childProperties.append(childProperty);
    qtnConnectChildProperty(this, childProperty);
    if (moveOwnership)
        childProperty->setParent(this);

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonChildPropertyAdd);

    m_ignoreChildPropertyChanges = true;
    childProperty->setStateInherited(state());
    m_ignoreChildPropertyChanges = false;

    return true;
}

bool QtnPropertySet::removeChildProperty(QtnPropertyBase* childProperty)
{
    Q_CHECK_PTR(childProperty);

    int childPropertyIndex = m_childProperties.indexOf(childProperty);
    if (childPropertyIndex < 0)
        return false;

    Q_ASSERT(childPropertyIndex < m_childProperties.size());

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonChildPropertyRemove, QtnPropertyValuePtr(childProperty));

    qtnDisconnectChildProperty(this, childProperty);
    m_childProperties.erase(m_childProperties.begin()+childPropertyIndex);
    if (childProperty->parent() == this)
        childProperty->setParent(nullptr);

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonChildPropertyRemove);

    return true;
}

QtnPropertySet* QtnPropertySet::createNew(QObject* parentForNew) const
{
    return createNewImpl(parentForNew);
}

QtnPropertySet* QtnPropertySet::createCopy(QObject* parentForCopy) const
{
    return createCopyImpl(parentForCopy);
}

bool QtnPropertySet::copyValues(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    return copyValuesImpl(propertySetCopyFrom, ignoreMask);
}

void QtnPropertySet::updateStateInherited(bool force)
{
    m_ignoreChildPropertyChanges = true;
    foreach(QtnPropertyBase* childProperty, m_childProperties)
    {
        childProperty->setStateInherited(state(), force);
    }
    m_ignoreChildPropertyChanges = false;
}

bool QtnPropertySet::fromStrImpl(const QString& str)
{
    static QRegExp parserLine("^\\s*([^=]+)=(.*)$");

    QStringList lines = str.split(QChar::LineFeed, QString::SkipEmptyParts);
    if (lines.isEmpty())
        return true;

    bool anySuccess = false;

    foreach (QString line, lines)
    {
        if (!parserLine.exactMatch(line))
            continue;

        QStringList params = parserLine.capturedTexts();
        if (params.size() != 3)
            continue;

        QString propertyPath = params[1];
        QString propertyStrValue = params[2];

        QList<QtnPropertyBase*> subProperties = findChildProperties(propertyPath, Qt::FindChildrenRecursively);
        if (subProperties.size() != 1)
            continue;

        if (subProperties[0]->fromStr(propertyStrValue))
            anySuccess = true;
    }

    return anySuccess;
}

bool QtnPropertySet::toStrImpl(QString& str) const
{
    return toStrWithPrefix(str, QString());
}

bool QtnPropertySet::loadImpl(QDataStream& stream)
{
    if (!QtnPropertyBase::loadImpl(stream))
        return false;

    if (stream.status() != QDataStream::Ok)
        return false;

    quint8 version = 0;
    stream >> version;
    // version incorrect
    if (version != 1)
        return false;

    forever
    {
        QtnPropertyID id = QtnPropertyIDInvalid;
        stream >> id;

        // no child properties any more
        if (id == QtnPropertyIDInvalid)
            break;

        QtnPropertyBase *childProperty = findChildProperty(id);
        if (!childProperty)
        {
            // cannot find subproperty -> skip
            if (!skipLoad(stream))
                return false;
            continue;
        }

        if (childProperty->state() & QtnPropertyStateNonSerialized)
        {
            // should not load such subproperty
            if (!skipLoad(stream))
                return false;
            continue;
        }

        if (!childProperty->load(stream))
            return false;
    }

    return stream.status() == QDataStream::Ok;
}

bool QtnPropertySet::saveImpl(QDataStream& stream) const
{
    if (!QtnPropertyBase::saveImpl(stream))
        return false;

    if (stream.status() != QDataStream::Ok)
        return false;

    // for compatibility
    quint8 version = 1;
    stream << version;

    foreach (QtnPropertyBase* childProperty, m_childProperties)
    {
        if (childProperty->state() & QtnPropertyStateNonSerialized)
            continue;

        if (childProperty->id() == QtnPropertyIDInvalid)
        {
            // serializable properties should have unique ids
            Q_ASSERT(false && "serializable properties should have unique ids");
            continue;
        }

        // save child property id
        stream << childProperty->id();
        // save child property
        if (!childProperty->save(stream))
            return false;
    }

    // mark end of children list
    stream << QtnPropertyIDInvalid;

    return stream.status() == QDataStream::Ok;
}

void QtnPropertySet::findChildPropertiesRecursive(const QString& name, QList<QtnPropertyBase*>& result)
{
    foreach(QtnPropertyBase* childProperty, m_childProperties)
    {
        if (childProperty->name() == name)
            result.append(childProperty);

        QtnPropertySet* propertySet = childProperty->asPropertySet();
        if (propertySet)
            propertySet->findChildPropertiesRecursive(name, result);
    }
}

void QtnPropertySet::findChildPropertiesRecursive(const QRegularExpression& re, QList<QtnPropertyBase*>& result)
{
    foreach(QtnPropertyBase* childProperty, m_childProperties)
    {
        if (re.match(childProperty->name()).isValid())
            result.append(childProperty);

        QtnPropertySet* propertySet = childProperty->asPropertySet();
        if (propertySet)
            propertySet->findChildPropertiesRecursive(re, result);
    }
}

void QtnPropertySet::childPropertyWillChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue)
{
    if (m_ignoreChildPropertyChanges)
        return;

    Q_EMIT propertyWillChange(changedProperty, this, reason, newValue);
}

void QtnPropertySet::childPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
    if (m_ignoreChildPropertyChanges)
        return;

    Q_EMIT propertyDidChange(changedProperty, this, reason);
}

bool QtnPropertySet::toStrWithPrefix(QString& str, const QString& prefix) const
{
#ifdef Q_OS_WIN32
    static QString lineEnd("\r\n");
#else
    static QString lineEnd("\n");
#endif

    foreach(QtnPropertyBase* childPropertyBase, m_childProperties)
    {
        QtnProperty* childProperty = childPropertyBase->asProperty();
        if (childProperty)
        {
            QString strValue;
            if (!childProperty->toStr(strValue))
                return false;

            str.append(QString("%1%2 = %3%4").arg(prefix, childProperty->name(), strValue, lineEnd));
        }
        else
        {
            QtnPropertySet* childPropertySet = childPropertyBase->asPropertySet();
            if (childPropertySet)
            {
                if (!childPropertySet->toStrWithPrefix(str, QString("%1%2.").arg(prefix, childPropertySet->name())))
                    return false;
            }
            else
            {
                // neither property no propertyset
                Q_ASSERT(false);
            }
        }
    }

    return true;
}
