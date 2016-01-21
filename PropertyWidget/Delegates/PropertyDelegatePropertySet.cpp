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

#include "PropertyDelegatePropertySet.h"
#include "../../Core/PropertySet.h"

void regPropertySetDelegates()
{
  QtnPropertyDelegateFactory::staticInstance()
    .registerDelegateDefault(&QtnPropertySet::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegatePropertySet, QtnPropertySet>
                 , "Default");
}

int QtnPropertyDelegatePropertySet::subPropertyCountImpl() const
{
    return owner().childProperties().size();
}

QtnPropertyBase* QtnPropertyDelegatePropertySet::subPropertyImpl(int index)
{
    return owner().childProperties()[index];
}

void QtnPropertyDelegatePropertySet::createSubItemsImpl(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems)
{
    // background
    {
        QtnPropertyDelegateSubItem bgItem;
        bgItem.rect = context.rect;

        if (bgItem.rect.isValid())
        {
            bgItem.drawHandler = [this](QtnPropertyDelegateDrawContext& context, const QtnPropertyDelegateSubItem& item) {
                // fill background
                context.painter->fillRect(item.rect, (context.isActive) ?
                                              context.widget->palette().color(QPalette::Highlight) :
                                              context.widget->palette().color(QPalette::Button));

            };

            subItems.append(bgItem);
        }
    }

    // branch node sign
    if (context.hasChildren)
    {
        QtnPropertyDelegateSubItem brItem;
        brItem.rect = context.rect.marginsRemoved(context.margins);
        brItem.rect.setRight(brItem.rect.left() + brItem.rect.height());
        context.margins.setLeft(context.margins.left() + brItem.rect.height());

        if (brItem.rect.isValid())
        {
            brItem.drawHandler = [this](QtnPropertyDelegateDrawContext& context, const QtnPropertyDelegateSubItem& item) {
                auto& painter = *context.painter;
                QRectF branchRect = item.rect;
                qreal side = branchRect.height() / 3.5f;

                QPainterPath branchPath;
                if (propertyImmutable()->stateLocal() & QtnPropertyStateCollapsed)
                {
                    branchPath.moveTo(branchRect.left() + side, branchRect.top() + side);
                    branchPath.lineTo(branchRect.right() - side - 1, branchRect.top() + branchRect.height() / 2.f);
                    branchPath.lineTo(branchRect.left() + side, branchRect.bottom() - side);
                    branchPath.closeSubpath();
                }
                else
                {
                    branchPath.moveTo(branchRect.left() + side, branchRect.top() + side);
                    branchPath.lineTo(branchRect.right() - side, branchRect.top() + side);
                    branchPath.lineTo(branchRect.left() + branchRect.width() / 2.f, branchRect.bottom() - side - 1);
                    branchPath.closeSubpath();
                }

                if (painter.testRenderHint(QPainter::Antialiasing))
                {
                    painter.fillPath(branchPath, context.widget->palette().color(QPalette::Text));
                }
                else
                {
                    painter.setRenderHint(QPainter::Antialiasing, true);
                    painter.fillPath(branchPath, context.widget->palette().color(QPalette::Text));
                    painter.setRenderHint(QPainter::Antialiasing, false);
                }
            };

            brItem.eventHandler = [this](QtnPropertyDelegateEventContext& context, const QtnPropertyDelegateSubItem&) -> bool {
                if (context.eventType() == QEvent::MouseButtonPress)
                {
                    property()->switchStateAuto(QtnPropertyStateCollapsed);
                    return true;
                }

                return false;
            };

            subItems.append(brItem);
        }
    }

    // property set name
    {
        QtnPropertyDelegateSubItem nameItem;
        nameItem.rect = context.rect.marginsRemoved(context.margins);

        if (nameItem.rect.isValid())
        {
            nameItem.drawHandler = [this](QtnPropertyDelegateDrawContext& context, const QtnPropertyDelegateSubItem& item) {
                QPalette::ColorGroup cg = property()->isEditableByUser() ? QPalette::Active : QPalette::Disabled;

                QFont oldFont = context.painter->font();
                QPen oldPen = context.painter->pen();

                // draw name
                QFont font = oldFont;
                font.setBold(true);
                context.painter->setFont(font);

                context.painter->setPen(context.widget->palette().color(cg, (context.isActive) ? QPalette::HighlightedText : QPalette::Text));

                QString elidedName = context.painter->fontMetrics().elidedText(property()->name(), Qt::ElideRight, item.rect.width());
                context.painter->drawText(item.rect, Qt::AlignLeading|Qt::AlignVCenter|Qt::TextSingleLine, elidedName);

                context.painter->setPen(oldPen);
                context.painter->setFont(oldFont);
            };

            subItems.append(nameItem);
        }
    }
}
