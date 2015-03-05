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

#include "AccessibilityProxy.h"
#include "../PropertyView.h"
#include <QMessageBox>

static int typeID = qRegisterMetaType<QtnAccessibilityProxy*>("QtnAccessibilityProxy*");

QtnAccessibilityProxy::QtnAccessibilityProxy(QtnPropertyView* owner)
    : QObject(owner),
      m_owner(owner)
{
}

QtnAccessibilityProxy::~QtnAccessibilityProxy()
{
}

QtnPropertyView* QtnAccessibilityProxy::owner()
{
    return m_owner;
}

QtnPropertySet* QtnAccessibilityProxy::propertySet()
{
    return m_owner->propertySet();
}

QtnPropertyBase* QtnAccessibilityProxy::activeProperty()
{
    return m_owner->activeProperty();
}

QtnPropertyBase* QtnAccessibilityProxy::findProperty(QString nameOrPath)
{
    auto root = m_owner->propertySet();
    if (!root)
        return nullptr;

    auto properties = root->findChildProperties(nameOrPath);
    if (properties.size() != 1)
        return nullptr;

    return properties[0];
}

QtnPropertyBase* QtnAccessibilityProxy::propertyUnderPoint(QPoint point)
{
    int index = m_owner->visibleItemIndexByPoint(point);
    if (index < 0)
        return nullptr;

    return m_owner->m_visibleItems[index].item->property;
}

void QtnAccessibilityProxy::ensureVisibleProperty(QtnPropertyBase* property)
{
    if (!property)
        return;

    auto propertySet = qobject_cast<QtnPropertySet*>(property->parent());
    while (propertySet)
    {
        propertySet->removeState(QtnPropertyStateCollapsed);
        propertySet = qobject_cast<QtnPropertySet*>(propertySet->parent());
    }

    m_owner->ensureVisible(property);
}

QRect QtnAccessibilityProxy::propertyNameRect(QtnPropertyBase* property)
{
    if (!property)
        return QRect();

    int index = m_owner->visibleItemIndexByProperty(property);
    if (index < 0)
        return QRect();

    QRect rect = m_owner->visibleItemRect(index);
    rect.setRight(m_owner->splitPosition());
    return rect;
}

QRect QtnAccessibilityProxy::propertyValueRect(QtnPropertyBase* property)
{
    if (!property)
        return QRect();

    int index = m_owner->visibleItemIndexByProperty(property);
    if (index < 0)
        return QRect();

    QRect rect = m_owner->visibleItemRect(index);
    rect.setLeft(m_owner->splitPosition() + 1);
    return rect;
}

QRect QtnAccessibilityProxy::propertyActionRect(QtnPropertyBase* property, int actionIndex)
{
    if (!property)
        return QRect();

    int index = m_owner->visibleItemIndexByProperty(property);
    if (index < 0)
        return QRect();

    const auto& item = m_owner->m_visibleItems[index];
    if (!item.actionsValid)
        return QRect();

    if (actionIndex < 0 || actionIndex >= item.actions.size())
        return QRect();

    return item.actions[actionIndex].rect;
}

QString QtnAccessibilityProxy::propertyDelegateName(QtnPropertyBase* property)
{
    if (!property)
        return QString();

    auto theProperty = property->asProperty();
    if (!theProperty)
        return QString();

    if (!theProperty->delegate())
        return QString("default");

    return theProperty->delegate()->name;
}

