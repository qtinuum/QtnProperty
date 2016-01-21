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

#include "PropertyDelegateButton.h"
#include "../../../Core/GUI/PropertyButton.h"
#include "../PropertyDelegateFactory.h"
#include <QStyleOptionButton>

void regButtonDelegates()
{
  QtnPropertyDelegateFactory::staticInstance()
    .registerDelegateDefault(&QtnPropertyButton::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateButton, QtnPropertyButton>
                 , "Button");
}

QtnPropertyDelegateButton::QtnPropertyDelegateButton(QtnPropertyButton& owner)
    : QtnPropertyDelegateTyped<QtnPropertyButton, QtnPropertyDelegate>(owner)
{
    m_title = owner.name();
}

void QtnPropertyDelegateButton::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    qtnGetAttribute(attributes, "title", m_title);
}

void QtnPropertyDelegateButton::createSubItemsImpl(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems)
{
    QtnPropertyDelegateSubItem buttonItem;
    buttonItem.rect = context.rect;//.marginsRemoved(context.margins);

    buttonItem.drawHandler = [this](QtnPropertyDelegateDrawContext& context, const QtnPropertyDelegateSubItem& item) {

        context.painter->save();

        auto style = context.style();

        QStyleOptionButton option;
        context.initStyleOption(option);

        option.state = QStyle::State_Active;
        if (property()->isEditableByUser())
            option.state |= QStyle::State_Enabled;
        if (context.isActive)
        {
            option.state |= QStyle::State_Selected;
            option.state |= QStyle::State_HasFocus;
            option.state |= QStyle::State_MouseOver;
        }

        // dont initialize styleObject from widget for QWindowsVistaStyle
        // this disables buggous animations
        if (style->inherits("QWindowsVistaStyle"))
            option.styleObject = nullptr;

        //option.state |= m_pushableTracker.styleStateByItem(cache.item);
        option.rect = item.rect;

        // draw button
        style->drawControl(QStyle::CE_PushButtonBevel, &option, context.painter, context.widget);

        // setup standard palette
        auto cg = context.colorGroup();
        context.painter->setPen(context.palette().color(cg, QPalette::ButtonText));
        context.painter->setBackground(context.palette().brush(cg, QPalette::Button));

        // shift sub-view's origin if button has pressed
        if (option.state & QStyle::State_Sunken)
            context.painter->translate(QPoint(1, 1));

        int bttnMargin = style->pixelMetric(QStyle::PM_ButtonMargin);
        QRect contentRect = item.rect.marginsRemoved(QMargins(bttnMargin, 0, bttnMargin, 0));
        context.painter->drawText(contentRect, Qt::AlignHCenter | Qt::AlignVCenter
                         , qtnElidedText(*context.painter, m_title, contentRect));

        // restore sub-view's origin if button has pressed
        if (option.state & QStyle::State_Sunken)
            context.painter->translate(QPoint(-1, -1));

        context.painter->restore();
    };

    buttonItem.eventHandler = [this](QtnPropertyDelegateEventContext& context, const QtnPropertyDelegateSubItem&) -> bool {
        bool doClick = false;
        switch (context.eventType())
        {
        case QEvent::MouseButtonRelease:
            doClick = true;
            break;

        case QEvent::KeyPress:
            int key = context.eventAs<QKeyEvent>()->key();
            doClick = (key == Qt::Key_Space) || (key == Qt::Key_Return);
            break;
        }

        if (doClick)
        {
            owner().invokeClick();
            return true;
        }

        return false;
    };


    subItems.append(buttonItem);
}
