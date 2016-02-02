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

#include "PropertyDelegate.h"

QtnPropertyBase* QtnPropertyDelegate::property()
{
    return propertyImpl();
}

const QtnPropertyBase* QtnPropertyDelegate::propertyImmutable() const
{
    return propertyImmutableImpl();
}

int QtnPropertyDelegate::subPropertyCount() const
{
    return subPropertyCountImpl();
}

QtnPropertyBase* QtnPropertyDelegate::subProperty(int index)
{
    Q_ASSERT(index >= 0 && index < subPropertyCount());
    return subPropertyImpl(index);
}

void QtnPropertyDelegate::applyAttributes(const QtnPropertyDelegateAttributes& attributes)
{
    applyAttributesImpl(attributes);
}

void QtnPropertyDelegate::createSubItems(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    createSubItemsImpl(context, subItems);
}

QStyle::State QtnPropertyDelegate::state(bool isActive, QtnSubItemState subState) const
{
    QStyle::State state = QStyle::State_Active;
    if (propertyImmutable()->isEditableByUser())
        state |= QStyle::State_Enabled;
    if (isActive)
    {
        state |= QStyle::State_Selected;
        state |= QStyle::State_HasFocus;
    }

    if (subState == QtnSubItemStateUnderCursor)
        state |= QStyle::State_MouseOver;
    else if (subState == QtnSubItemStatePushed)
        state |= QStyle::State_Sunken;

    return state;
}
