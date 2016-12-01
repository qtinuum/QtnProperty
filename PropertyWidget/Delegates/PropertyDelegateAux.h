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

#ifndef QTN_PROPERTY_DELEGATE_AUX_H
#define QTN_PROPERTY_DELEGATE_AUX_H

#include "../PropertyWidgetAPI.h"

#include <functional>
#include <QStyleOption>
#include <QStylePainter>
#include <QEvent>

class QtnPropertyView;
struct QtnDrawContext;
struct QtnEventContext;

class QtnSubItemEvent: public QEvent
{
public:
    enum Type
    {
        Activated = 3 * QEvent::User + 15,
        Deactivated = 3 * QEvent::User + 16,
        PressMouse = 3 * QEvent::User + 17,
        ReleaseMouse = 3 * QEvent::User + 18
    };

    QtnSubItemEvent(Type type, QPoint mousePos);

    QPoint pos() const { return m_mousePos; }
    int x() const { return m_mousePos.x(); }
    int y() const { return m_mousePos.y(); }

private:
    QPoint m_mousePos;
};

enum QtnSubItemState
{
    QtnSubItemStateNone,
    QtnSubItemStateUnderCursor,
    QtnSubItemStatePushed
};

struct QTN_PW_EXPORT QtnSubItem
{
    QtnSubItem(const QRect& rect);

    QRect rect;

    std::function<void(QtnDrawContext&, const QtnSubItem&)> drawHandler;
    std::function<bool(QtnEventContext&, const QtnSubItem&)> eventHandler;
    std::function<QString(QtnEventContext&, const QtnSubItem&)> tooltipHandler;

    QtnSubItemState state() const { return m_state; }
    void trackState() { m_trackState = true; }

    void setTextAsTooltip(const QString& text);
    void setPropertyNameAsTooltip(const QtnPropertyBase& property);
    void setPropertyDescriptionAsTooltip(const QtnPropertyBase& property);

private:
    bool activate(QtnPropertyView* widget, QPoint mousePos);
    bool deactivate(QtnPropertyView* widget, QPoint mousePos);

    bool grabMouse(QtnPropertyView* widget, QPoint mousePos);
    bool releaseMouse(QtnPropertyView* widget, QPoint mousePos);

    void draw(QtnDrawContext& context) const;
    bool event(QtnEventContext& context);
    bool selfEvent(QtnSubItemEvent::Type type, QtnPropertyView* widget, QPoint mousePos);

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

    int eventType() const { return static_cast<int>(event->type()); }
    template <class EventT>
    EventT* eventAs() { return static_cast<EventT*>(event); }

    void updateWidget();

private:
    bool grabMouse(QtnSubItem* subItem, QPoint mousePos);
    bool releaseMouse(QtnSubItem* subItem, QPoint mousePos);

    friend struct QtnSubItem;
};

QTN_PW_EXPORT QString qtnElidedText(const QPainter& painter, const QString& text, const QRect& rect, bool* elided = 0);
QTN_PW_EXPORT void qtnDrawValueText(const QString& text, QPainter& painter, const QRect& rect, QStyle::State state, bool* needTooltip = nullptr);


#endif // QTN_PROPERTY_DELEGATE_AUX_H
