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
struct QtnDrawContext;
struct QtnEventContext;

enum QtnSubItemState
{
    QtnSubItemStateNone,
    QtnSubItemStateUnderCursor,
    QtnSubItemStatePushed
};

struct QTN_PW_EXPORT QtnSubItem
{
    QtnSubItem(bool trackState = false);

    QRect rect;

    std::function<void(QtnDrawContext&, const QtnSubItem&)> drawHandler;
    std::function<bool(QtnEventContext&, const QtnSubItem&)> eventHandler;

    QtnSubItemState state() const { return m_state; }
    void trackState() { m_trackState = true; }

    enum SubItemEventType
    {
        EventActivated = 3 * QEvent::User + 15,
        EventDeactivated = 3 * QEvent::User + 16,
        EventGrabMouse = 3 * QEvent::User + 17,
        EventReleaseMouse = 3 * QEvent::User + 18
    };

private:
    bool activate(QtnPropertyView* widget);
    bool deactivate(QtnPropertyView* widget);

    bool grabMouse(QtnPropertyView* widget);
    bool releaseMouse(QtnPropertyView* widget);

    void draw(QtnDrawContext& context) const;
    bool event(QtnEventContext& context);
    bool selfEvent(SubItemEventType type, QtnPropertyView* widget);

    bool m_trackState;
    quint8 m_activeCount;
    QtnSubItemState m_state;

    friend class QtnPropertyView;
};

struct QTN_PW_EXPORT QtnDrawContext
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

struct QTN_PW_EXPORT QtnEventContext
{
public:
    QEvent* event;
    QtnPropertyView* widget;

    QEvent::Type eventType() const { return event->type(); }
    template <class EventT>
    EventT* eventAs() { return static_cast<EventT*>(event); }

    bool grabMouse(QtnSubItem* subItem);
    bool releaseMouse(QtnSubItem* subItem);
};


#endif // QTN_PROPERTY_DELEGATE_AUX_H
