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

#ifndef QTN_PROPERTY_DELEGATE_AUX_H
#define QTN_PROPERTY_DELEGATE_AUX_H

#include "../PropertyWidgetAPI.h"

#include <functional>
#include <QStylePainter>
#include <QEvent>

class QtnPropertyView;
struct QtnPropertyDelegateDrawContext;
struct QtnPropertyDelegateEventContext;

enum QtnPropertyDelegateSubItemState
{
    QtnSubItemStateNone,
    QtnSubItemStateUnderCursor,
    QtnSubItemStatePushed
};

struct QTN_PW_EXPORT QtnPropertyDelegateSubItem
{
    QtnPropertyDelegateSubItem(bool trackState = false);

    QRect rect;

    std::function<void(QtnPropertyDelegateDrawContext&, const QtnPropertyDelegateSubItem&)> drawHandler;
    std::function<bool(QtnPropertyDelegateEventContext&, const QtnPropertyDelegateSubItem&)> eventHandler;

    QtnPropertyDelegateSubItemState state() const { return m_state; }
    void trackState() { m_trackState = true; }

private:
    bool activate(QtnPropertyView* widget);
    bool deactivate(QtnPropertyView* widget);

    void draw(QtnPropertyDelegateDrawContext& context) const;
    bool event(QtnPropertyDelegateEventContext& context) const;

    bool m_trackState;
    quint8 m_activeCount;
    QtnPropertyDelegateSubItemState m_state;

    friend class QtnPropertyView;
};

struct QTN_PW_EXPORT QtnPropertyDelegateDrawContext
{
public:
    QStylePainter* painter;
    const QtnPropertyView* widget;

    const QRect rect;
    QMargins margins;
    const int splitPos;

    const bool isActive;
    const bool hasChildren;

    QStyle* style() const;
    void initStyleOption(QStyleOption& option) const;
    const QPalette& palette() const;
    QPalette::ColorGroup colorGroup() const;
};

struct QTN_PW_EXPORT QtnPropertyDelegateEventContext
{
public:
    QEvent* event;
    const QtnPropertyView* widget;

    QEvent::Type eventType() const { return event->type(); }
    template <class EventT>
    EventT* eventAs() { return static_cast<EventT*>(event); }

    bool grabMouse(const QtnPropertyDelegateSubItem& subItem);
};

#endif // QTN_PROPERTY_DELEGATE_AUX_H
