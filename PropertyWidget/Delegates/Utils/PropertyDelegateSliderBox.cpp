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

#include "PropertyDelegateSliderBox.h"
#include "../../PropertyView.h"

#include <QMouseEvent>

QtnPropertyDelegateSlideBox::QtnPropertyDelegateSlideBox()
    : m_boxFillColor(QColor::fromRgb(200, 200, 255)),
      m_dragValuePart(0),
      m_liveUpdate(false)
{
}

void QtnPropertyDelegateSlideBox::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    qtnGetAttribute(attributes, "fillColor", m_boxFillColor);
    qtnGetAttribute(attributes, "liveUpdate", m_liveUpdate);
}

bool QtnPropertyDelegateSlideBox::createSubItemValueImpl(QtnDrawContext&, QtnSubItem& subItemValue)
{
    subItemValue.trackState();
    subItemValue.drawHandler = qtnMemFn(this, &QtnPropertyDelegateSlideBox::draw);
    subItemValue.eventHandler = qtnMemFn(this, &QtnPropertyDelegateSlideBox::event);
    return true;
}

void QtnPropertyDelegateSlideBox::draw(QtnDrawContext& context, const QtnSubItem& item)
{
    float valuePart = (item.state() == QtnSubItemStatePushed) ? dragValuePart() : propertyValuePart();
    if (valuePart < 0)
        return;

    auto boxBorderColor = (item.state() == QtnSubItemStateNone) ? m_boxFillColor : m_boxFillColor.darker(150);

    auto boxRect = item.rect;
    boxRect.adjust(2, 2, -2, -2);

    auto valueRect = boxRect;
    valueRect.setWidth(int(valuePart * valueRect.width()));

    auto& painter = *context.painter;

    painter.save();

    painter.fillRect(valueRect, m_boxFillColor);

    painter.setPen(boxBorderColor);
    painter.drawRect(valueRect);
    painter.drawRect(boxRect);

    painter.restore();

    boxRect.adjust(context.widget->valueLeftMargin(), 0, 0, 0);
    qtnDrawValueText(valuePartToStr(valuePart), painter, boxRect, state(context.isActive, item.state()), nullptr);
}

bool QtnPropertyDelegateSlideBox::event(QtnEventContext& context, const QtnSubItem& item)
{
    switch (context.eventType())
    {
    case QEvent::KeyPress:
    {
        int key = context.eventAs<QKeyEvent>()->key();

        if ((key == Qt::Key_Plus) || (key == Qt::Key_Equal))
            incrementPropertyValue(1);
        else if ((key == Qt::Key_Minus) || (key == Qt::Key_Underscore))
            incrementPropertyValue(-1);
        else
            return false;

        return true;
    } break;

    case QEvent::Wheel:
    {
        int steps = context.eventAs<QWheelEvent>()->angleDelta().y()/120;
        incrementPropertyValue(steps);
        return true;
    } break;

    case QtnSubItemEvent::Activated:
    {
        m_oldCursor = context.widget->cursor();
        return true;
    } break;

    case QtnSubItemEvent::PressMouse:
    {
        updateDragValuePart(context.eventAs<QtnSubItemEvent>()->x(), item.rect);
        context.widget->setCursor(Qt::SplitHCursor);
        context.updateWidget();
        return true;
    } break;

    case QEvent::MouseMove:
    {
        if (item.state() == QtnSubItemStatePushed)
        {
            updateDragValuePart(context.eventAs<QMouseEvent>()->x(), item.rect);
            if (m_liveUpdate)
                setPropertyValuePart(m_dragValuePart);
            else
                context.updateWidget();
        }
        return true;
    } break;

    case QtnSubItemEvent::ReleaseMouse:
    {
        updateDragValuePart(context.eventAs<QtnSubItemEvent>()->x(), item.rect);
        context.widget->setCursor(m_oldCursor);
        setPropertyValuePart(m_dragValuePart);
        return true;
    } break;

    default:
        return false;
    }
}

void QtnPropertyDelegateSlideBox::updateDragValuePart(int x, const QRect& rect)
{
    m_dragValuePart = float(x - rect.left()) / rect.width();
    if (m_dragValuePart < 0)
        m_dragValuePart = 0;
    else if (m_dragValuePart > 1.f)
        m_dragValuePart = 1.f;
}
