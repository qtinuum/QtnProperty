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

#include "PropertyDelegate.h"

#include <QLineEdit>
#include <QKeyEvent>

QString qtnElidedText(const QPainter& painter, const QString& text, const QRect& rect, bool* elided)
{
    QString newText = painter.fontMetrics().elidedText(text, Qt::ElideRight, rect.width());

    if (elided)
        *elided = (newText != text);

    return newText;
}

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

void QtnPropertyDelegateWithValue::createSubItemsImpl(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    addSubItemBackground(context, subItems);
    addSubItemSelection(context, subItems);
    addSubItemBranchNode(context, subItems);
    addSubItemName(context, subItems);
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
        QPen linesPen(context.widget->palette().color(QPalette::Button));
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
            context.painter->fillRect(item.rect, context.widget->palette().color(QPalette::Highlight));
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
        QColor fillClr = context.widget->palette().color(QPalette::Text);
        QColor outlineClr = (item.state() != QtnSubItemStateNone)
                    ? Qt::blue
                    : context.widget->palette().color(QPalette::Text);

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

    subItems.append(brItem);
}

void QtnPropertyDelegateWithValue::addSubItemName(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    QtnSubItem nameItem;
    nameItem.rect = context.rect.marginsRemoved(context.margins);
    nameItem.rect.setRight(context.splitPos);

    if (!nameItem.rect.isValid())
        return;

    nameItem.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {
        context.painter->save();

        QPalette::ColorGroup cg = property()->isEditableByUser() ? QPalette::Active : QPalette::Disabled;
        context.painter->setPen(context.widget->palette().color(cg, (context.isActive) ? QPalette::HighlightedText : QPalette::Text));

        context.painter->drawText(item.rect, Qt::AlignLeading|Qt::AlignVCenter|Qt::TextSingleLine
                                , qtnElidedText(*context.painter, property()->name(), item.rect));

        context.painter->restore();
    };

    subItems.append(nameItem);
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

    return true;
}

void QtnPropertyDelegateWithValueEditor::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    QString strValue;
    if (propertyValueToStrImpl(strValue))
    {
        drawValueText(strValue, painter, rect, state, needTooltip);
    }
}

bool QtnPropertyDelegateWithValueEditor::acceptKeyPressedForInplaceEditImpl(QKeyEvent* keyEvent) const
{
    int key = keyEvent->key();
    return (key == Qt::Key_F2) || (key == Qt::Key_Space) || (key == Qt::Key_Return);
}
/*
QString QtnPropertyDelegateWithValue::toolTipImpl() const
{
    QString strValue;
    propertyValueToStrImpl(strValue);
    return strValue;
}

/*
QString QtnPropertyDelegate::toolTip() const
{
    return toolTipImpl();
}

bool QtnPropertyDelegate::acceptKeyPressedForInplaceEdit(QKeyEvent* keyEvent) const
{
    return acceptKeyPressedForInplaceEditImpl(keyEvent);
}

QWidget* QtnPropertyDelegate::createValueEditor(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QWidget* valueEditor = createValueEditorImpl(parent, rect, inplaceInfo);
    if (!valueEditor)
        return valueEditor;

    valueEditor->setObjectName("QtnPropertyValueEditor");
    return valueEditor;
}


bool QtnPropertyDelegate::acceptKeyPressedForInplaceEditImpl(QKeyEvent* keyEvent) const
{
    int key = keyEvent->key();
    return (key == Qt::Key_F2) || (key == Qt::Key_Space) || (key == Qt::Key_Return);
}
*/
void QtnPropertyDelegate::drawValueText(const QString& text, QStylePainter& painter, const QRect& rect, QStyle::State state, bool* needTooltip)
{
    Q_UNUSED(state);

    if (text.isEmpty())
        return;

    painter.drawText(rect, Qt::AlignLeading | Qt::AlignVCenter
                     , qtnElidedText(painter, text, rect, needTooltip));
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
    drawValueText(valuePartToStr(valuePart), painter, boxRect, state(context.isActive, item.state()), nullptr);
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

