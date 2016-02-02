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

#include "PropertyDelegateMisc.h"
#include "../../PropertyView.h"
#include "../../Utils/InplaceEditing.h"

#include <QLineEdit>
#include <QKeyEvent>

void QtnPropertyDelegateWithValue::createSubItemsImpl(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    addSubItemBackground(context, subItems);
    addSubItemSelection(context, subItems);
    addSubItemBranchNode(context, subItems);
    addSubItemName(context, subItems);
    addSubItemReset(context, subItems);
    addSubItemValue(context, subItems);
}

void QtnPropertyDelegateWithValue::addSubItemBackground(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    QtnSubItem bgItem;
    bgItem.rect = context.rect;

    if (!bgItem.rect.isValid())
        return;

    bgItem.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {

        auto& painter = *context.painter;
        const auto& rect = item.rect;
        auto splitPos = context.splitPos;

        QPen oldPen = painter.pen();
        QPen linesPen(context.palette().color(QPalette::Button));
        painter.setPen(linesPen);

        // draw item borders
        painter.drawLine(rect.bottomLeft(), rect.bottomRight());
        painter.drawLine(splitPos, rect.top(), splitPos, rect.bottom());

        painter.setPen(oldPen);
    };

    subItems.append(bgItem);
}

void QtnPropertyDelegateWithValue::addSubItemSelection(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    QtnSubItem selItem;
    selItem.rect = context.rect;
    selItem.rect.setRight(context.splitPos);

    if (!selItem.rect.isValid())
        return;

    selItem.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {
        // highlight background if active property
        if (context.isActive)
            context.painter->fillRect(item.rect, context.palette().color(QPalette::Highlight));
    };

    subItems.append(selItem);
}

void QtnPropertyDelegateWithValue::addSubItemBranchNode(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    if (!context.hasChildren)
        return;

    QtnSubItem brItem(true);
    brItem.rect = context.rect.marginsRemoved(context.margins);
    brItem.rect.setWidth(brItem.rect.height());
    context.margins.setLeft(context.margins.left() + brItem.rect.height());

    if (!brItem.rect.isValid())
        return;

    brItem.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {
        auto& painter = *context.painter;
        QRectF branchRect = item.rect;
        qreal side = branchRect.height() / 3.5f;
        QColor fillClr = context.palette().color(QPalette::Text);
        QColor outlineClr = (item.state() != QtnSubItemStateNone)
                    ? Qt::blue
                    : context.palette().color(QPalette::Text);

        painter.save();
        painter.setPen(outlineClr);

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
            painter.fillPath(branchPath, fillClr);
            painter.drawPath(branchPath);
        }
        else
        {
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.fillPath(branchPath, fillClr);
            painter.drawPath(branchPath);
            painter.setRenderHint(QPainter::Antialiasing, false);
        }

        painter.restore();
    };

    brItem.eventHandler = [this](QtnEventContext& context, const QtnSubItem&) -> bool {
        if ((context.eventType() == QEvent::MouseButtonPress) || (context.eventType() == QEvent::MouseButtonDblClick))
        {
            property()->switchStateAuto(QtnPropertyStateCollapsed);
            return true;
        }

        return false;
    };

    brItem.tooltipHandler = [this](QtnEventContext&, const QtnSubItem&) -> QString {
        return (property()->stateLocal() & QtnPropertyStateCollapsed)
                ? "Click to expand"
                : "Click to collapse";
    };

    subItems.append(brItem);
}

void QtnPropertyDelegateWithValue::addSubItemName(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    QtnSubItem nameItem;
    nameItem.rect = context.rect.marginsRemoved(context.margins);
    nameItem.rect.setRight(context.splitPos);
    nameItem.setPropertyDescriptionAsTooltip(*propertyImmutable());

    if (!nameItem.rect.isValid())
        return;

    nameItem.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {
        context.painter->save();

        QPalette::ColorGroup cg = property()->isEditableByUser() ? QPalette::Active : QPalette::Disabled;
        context.painter->setPen(context.palette().color(cg, (context.isActive) ? QPalette::HighlightedText : QPalette::Text));

        context.painter->drawText(item.rect, Qt::AlignLeading|Qt::AlignVCenter|Qt::TextSingleLine
                                , qtnElidedText(*context.painter, property()->name(), item.rect));

        context.painter->restore();
    };

    subItems.append(nameItem);
}


void QtnPropertyDelegateWithValue::addSubItemReset(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    if (!propertyImmutable()->hasResetCallback())
        return;

    QtnSubItem resetItem(true);
    resetItem.rect = context.rect.marginsRemoved(context.margins);
    resetItem.rect.setLeft(resetItem.rect.right() - resetItem.rect.height());
    resetItem.setTextAsTooltip("Reset");

    if (!resetItem.rect.isValid())
        return;

    resetItem.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {

        auto style = context.style();

        QStyleOptionButton option;
        context.initStyleOption(option);

        option.state = state(context.isActive, item.state());

        // dont initialize styleObject from widget for QWindowsVistaStyle
        // this disables buggous animations
        if (style->inherits("QWindowsVistaStyle"))
            option.styleObject = nullptr;

        option.rect = item.rect;
        option.text = "R";

        //owner().invokePreDrawButton(&option);

        // draw button
        style->drawControl(QStyle::CE_PushButton, &option, context.painter, context.widget);
    };

    resetItem.eventHandler = [this](QtnEventContext& context, const QtnSubItem&) -> bool {
        bool doClick = false;
        switch (context.eventType())
        {
        case QtnSubItemEvent::ReleaseMouse:
            doClick = true;
            break;
/*
        case QEvent::KeyPress:
            int key = context.eventAs<QKeyEvent>()->key();
            doClick = (key == Qt::Key_Space) || (key == Qt::Key_Return);
            break;
            */
        }

        if (doClick)
        {
            property()->reset();
            return true;
        }

        return false;
    };

    subItems.append(resetItem);
}

void QtnPropertyDelegateWithValue::addSubItemValue(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    QtnSubItem valueItem;
    valueItem.rect = context.rect.marginsRemoved(context.margins);
    valueItem.rect.setLeft(context.splitPos + context.widget->valueLeftMargin());

    if (valueItem.rect.isValid() && createSubItemValueImpl(context, valueItem))
        subItems.append(valueItem);
}

bool QtnPropertyDelegateWithValueEditor::createSubItemValueImpl(QtnDrawContext&, QtnSubItem& subItemValue)
{
    subItemValue.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {
        // draw property value
       drawValueImpl(*context.painter, item.rect, state(context.isActive, item.state()), nullptr);
    };

    subItemValue.eventHandler = [this](QtnEventContext& context, const QtnSubItem& item) -> bool {
        bool doEdit = false;
        switch (context.eventType())
        {
        case QEvent::MouseButtonDblClick:
            doEdit = (context.widget->propertyViewStyle() & QtnPropertyViewStyleDblClickActivation);
            break;

        case QEvent::MouseButtonRelease:
            doEdit = !(context.widget->propertyViewStyle() & QtnPropertyViewStyleDblClickActivation);
            break;

        case QEvent::KeyPress:
            doEdit = acceptKeyPressedForInplaceEditImpl(context.eventAs<QKeyEvent>());
            break;
        }

        if (doEdit)
        {
            QtnInplaceInfo inplaceInfo;
            inplaceInfo.activationEvent = context.event;
            QWidget* editor = createValueEditorImpl(context.widget->viewport(), item.rect, &inplaceInfo);
            if (!editor)
                return false;

            if (editor->objectName().isEmpty())
                editor->setObjectName("QtnPropertyValueEditor");

            if (!editor->isVisible())
                editor->show();

            qtnStartInplaceEdit(editor);

            return true;
        }

        return false;
    };

    subItemValue.tooltipHandler = [this](QtnEventContext&, const QtnSubItem&) -> QString {
        QString valueText;
        if (!propertyValueToStrImpl(valueText))
            return QString();
        return valueText;
    };

    return true;
}

void QtnPropertyDelegateWithValueEditor::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    QString strValue;
    if (propertyValueToStrImpl(strValue))
    {
        qtnDrawValueText(strValue, painter, rect, state, needTooltip);
    }
}

bool QtnPropertyDelegateWithValueEditor::acceptKeyPressedForInplaceEditImpl(QKeyEvent* keyEvent) const
{
    int key = keyEvent->key();
    return (key == Qt::Key_F2) || (key == Qt::Key_Space) || (key == Qt::Key_Return);
}

QWidget* QtnPropertyDelegateWithValueEditor::createValueEditorLineEdit(QWidget* parent, const QRect& rect, bool readOnly, QtnInplaceInfo* inplaceInfo)
{
    QLineEdit* lineEdit = new QLineEdit(parent);
    lineEdit->setGeometry(rect);
    lineEdit->setReadOnly(readOnly);

    QString strValue;
    propertyValueToStrImpl(strValue);
    lineEdit->setText(strValue);

    if (inplaceInfo)
    {
        lineEdit->selectAll();
    }

    return lineEdit;
}

bool QtnPropertyDelegateError::createSubItemValueImpl(QtnDrawContext& /*context*/, QtnSubItem& subItemValue)
{
    subItemValue.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {
        QPen oldPen = context.painter->pen();
        context.painter->setPen(Qt::red);
        qtnDrawValueText(m_error, *context.painter, item.rect, state(context.isActive, item.state()));
        context.painter->setPen(oldPen);
    };

    return true;
}

QtnPropertyDelegate* qtnCreateDelegateError(QtnPropertyBase& owner, QString error)
{
    return new QtnPropertyDelegateError(owner, error);
}

