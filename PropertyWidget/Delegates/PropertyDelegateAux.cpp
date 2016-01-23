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

#include "PropertyDelegateAux.h"
#include "PropertyView.h"

QtnPropertyDelegateSubItem::QtnPropertyDelegateSubItem(bool trackState)
    : m_trackState(trackState),
      m_activeCount(0),
      m_state(QtnSubItemStateNone)
{
}

bool QtnPropertyDelegateSubItem::activate(QtnPropertyView *widget)
{
    if (!m_trackState)
        return false;

    Q_ASSERT(m_activeCount <= 1);
    if (m_activeCount > 1)
        return false;

    ++m_activeCount;

    if (m_state != QtnSubItemStateUnderCursor)
    {
        m_state = QtnSubItemStateUnderCursor;
        widget->viewport()->update();
     }

    return true;
}

bool QtnPropertyDelegateSubItem::deactivate(QtnPropertyView *widget)
{
    if (!m_trackState)
        return false;

    Q_ASSERT(m_activeCount > 0);
    if (m_activeCount == 0)
        return false;

    --m_activeCount;

    if ((m_activeCount == 0) && (m_state != QtnSubItemStateNone))
    {
        m_state = QtnSubItemStateNone;
        widget->viewport()->update();
    }

    return true;
}

void QtnPropertyDelegateSubItem::draw(QtnPropertyDelegateDrawContext& context) const
{
    if (drawHandler)
        drawHandler(context, *this);
}

bool QtnPropertyDelegateSubItem::event(QtnPropertyDelegateEventContext& context) const
{
    if (eventHandler)
        return eventHandler(context, *this);

    return false;
}

QStyle* QtnPropertyDelegateDrawContext::style() const
{
    return widget->style();
}

void QtnPropertyDelegateDrawContext::initStyleOption(QStyleOption& option) const
{
    option.initFrom(widget);
    // State_MouseOver should be set explicitly
    option.state &= ~QStyle::State_MouseOver;
}

const QPalette& QtnPropertyDelegateDrawContext::palette() const
{
    return widget->palette();
}

QPalette::ColorGroup QtnPropertyDelegateDrawContext::colorGroup() const
{
    if (!widget->isEnabled())
        return QPalette::Disabled;
    else if (!widget->hasFocus())
        return QPalette::Inactive;
    else
        return QPalette::Active;
}
