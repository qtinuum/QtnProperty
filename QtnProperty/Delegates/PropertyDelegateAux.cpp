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

#include "PropertyDelegateAux.h"
#include "PropertyView.h"
#include <QMouseEvent>

QtnSubItem::QtnSubItem(const QRect &rect)
    : rect(rect),
      m_trackState(false),
      m_activeCount(0),
      m_state(QtnSubItemStateNone)
{
}

void QtnSubItem::setTextAsTooltip(const QString& text)
{
    tooltipHandler = [text](QtnEventContext&, const QtnSubItem&)->QString {
        return text;
    };
}

void QtnSubItem::setPropertyNameAsTooltip(const QtnPropertyBase& property)
{
    tooltipHandler = [&property](QtnEventContext&, const QtnSubItem&)->QString {
        return property.name();
    };
}

void QtnSubItem::setPropertyDescriptionAsTooltip(const QtnPropertyBase& property)
{
    tooltipHandler = [&property](QtnEventContext&, const QtnSubItem&)->QString {
        return property.description();
    };
}

bool QtnSubItem::activate(QtnPropertyView *widget, QPoint mousePos)
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
        selfEvent(QtnSubItemEvent::Activated, widget, mousePos);
     }

    return true;
}

bool QtnSubItem::deactivate(QtnPropertyView *widget, QPoint mousePos)
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
        selfEvent(QtnSubItemEvent::Deactivated, widget, mousePos);
    }

    return true;
}

bool QtnSubItem::grabMouse(QtnPropertyView* widget, QPoint mousePos)
{
    Q_ASSERT(m_activeCount > 0);
    Q_ASSERT(m_state == QtnSubItemStateUnderCursor);

    m_state = QtnSubItemStatePushed;
    widget->viewport()->update();
    selfEvent(QtnSubItemEvent::PressMouse, widget, mousePos);

    return true;
}

bool QtnSubItem::releaseMouse(QtnPropertyView* widget, QPoint mousePos)
{
    Q_ASSERT(m_activeCount > 0);
    Q_ASSERT(m_state == QtnSubItemStatePushed);

    m_state = QtnSubItemStateUnderCursor;
    widget->viewport()->update();
    selfEvent(QtnSubItemEvent::ReleaseMouse, widget, mousePos);

    return true;
}

void QtnSubItem::draw(QtnDrawContext& context) const
{
    if (drawHandler)
        drawHandler(context, *this);
}

bool QtnSubItem::event(QtnEventContext& context)
{
    if (m_trackState)
    {
        switch (context.eventType())
        {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick:
        {
            auto event = context.eventAs<QMouseEvent>();
            if (event->button() == Qt::LeftButton)
                context.grabMouse(this, event->pos());
        } break;

        case QEvent::MouseButtonRelease:
        {
            auto event = context.eventAs<QMouseEvent>();
            if ((event->button() == Qt::LeftButton)
                    && (m_state == QtnSubItemStatePushed))
                context.releaseMouse(this, event->pos());
        } break;

        default:
            ;
        }
    }

    if (context.eventType() == QEvent::ToolTip)
    {
        if (!tooltipHandler)
            return false;

        QString tooltipText = tooltipHandler(context, *this);
        if (!tooltipText.isEmpty())
        {
            auto event = context.eventAs<QHelpEvent>();
            QToolTip::showText(event->globalPos(), tooltipText, context.widget, rect);
            return true;
        }
    }

    if (eventHandler)
        return eventHandler(context, *this);

    return false;
}

bool QtnSubItem::selfEvent(QtnSubItemEvent::Type type, QtnPropertyView* widget, QPoint mousePos)
{
    QtnSubItemEvent event_(type, mousePos);
    QtnEventContext context{&event_, widget};
    return event(context);
}

QStyle* QtnDrawContext::style() const
{
    return widget->style();
}

void QtnDrawContext::initStyleOption(QStyleOption& option) const
{
    option.initFrom(widget);
    // State_MouseOver should be set explicitly
    option.state &= ~QStyle::State_MouseOver;
}

const QPalette& QtnDrawContext::palette() const
{
    return widget->palette();
}

QPalette::ColorGroup QtnDrawContext::colorGroup() const
{
    if (!widget->isEnabled())
        return QPalette::Disabled;
    else if (!widget->hasFocus())
        return QPalette::Inactive;
    else
        return QPalette::Active;
}

void QtnEventContext::updateWidget()
{
    widget->viewport()->update();
}

bool QtnEventContext::grabMouse(QtnSubItem* subItem, QPoint mousePos)
{
    return widget->grabMouseForSubItem(subItem, mousePos);
}

bool QtnEventContext::releaseMouse(QtnSubItem* subItem, QPoint mousePos)
{
    return widget->releaseMouseForSubItem(subItem, mousePos);
}

QtnSubItemEvent::QtnSubItemEvent(Type type, QPoint mousePos)
    : QEvent((QEvent::Type)type),
      m_mousePos(mousePos)
{}

QString qtnElidedText(const QPainter& painter, const QString& text, const QRect& rect, bool* elided)
{
    QString newText = painter.fontMetrics().elidedText(text, Qt::ElideRight, rect.width());

    if (elided)
        *elided = (newText != text);

    return newText;
}

void qtnDrawValueText(const QString& text, QPainter& painter, const QRect& rect, QStyle::State state, bool* needTooltip)
{
    Q_UNUSED(state);

    if (text.isEmpty())
        return;

    painter.drawText(rect, Qt::AlignLeading | Qt::AlignVCenter
                     , qtnElidedText(painter, text, rect, needTooltip));
}
