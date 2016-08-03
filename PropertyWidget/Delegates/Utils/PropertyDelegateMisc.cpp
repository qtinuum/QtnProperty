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

QIcon qtnResetIcon;

static QIcon resetIcon()
{
    if (!qtnResetIcon.isNull())
        return qtnResetIcon;

    static const char iconData[] = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJ"
            "bWFnZVJlYWR5ccllPAAAAsJJREFUeNp0UktIVGEUPve/j7l37h2aCSQRzEU46jg5D3QWBUnaWGKE"
            "WTmjNYobF4EERYWYD8bMVUW7XEVGtWsduIloZWoINRkR0oCLWaRCNa/76vzXuRdb+F/O/V/nfN85"
            "/3eY2tpaOGgwhAFCyKJpmNcNw/iz/y6bzVozMeHgj+d4kGU5JSvyZ5Zj22mAfWcPYq1tw8ESNiYI"
            "whtJlH673W7T4/EAWp2iKO94gX/IMIywLx440zTtdAVknJLc0rjL5SIIAhzHAc/ze8AsS/c383/z"
            "XcViMYFHGQcA6wRRFJ9KkjQiu2VAJisA2RwmCkj3oXCo7kr/ZY+TgfXjuA5MeUSRFeB4DsKRMCQH"
            "EuBv8EPfhUtAWKwUiZIDSUgOJt5jyIYDQFFrampmy2rZcmxqaoSpmcm3eDd96kT7suJRTN9hH9y6"
            "fQui0ch0WySWLpfLsLW1VQFAqVRdbaVlqKoKvX29q23RWHepVLLSDgQDMJOe2a2uPtIfaYkuaaoG"
            "9rvZGbDFQlHA+i0An893p1Qsga7roBENHj959AnveluaQ1l6tl8BS8a19VVAiQAbBTRNgwdz8znq"
            "SFkoIAafDDYez+qaLiHZKGY6inPCAQgFw3q9v16lwRRkc3Nz1moVBDB0A5rqAwV6jg11V3SJC6Ik"
            "LqD/mANA2c51n12jM33QQr5wEVVZAAYOVWoVUNIJZL6HPULlhrEbY/n/AOJd8ckz8U5Hb5R0FJtp"
            "F9c/BZdQQtb72Bss7Y2e8z3Q0Xk67TwiTbO5Ibi0ur6yWFVVNfTq5WvAIMDep2UcpVnRQGpXrw1C"
            "ajj1DP0/5HK5vQ72+rxAGALffmwIuJ/KfMmMLz5/QVY+rgDVm2bU2tYKQ8MpI9AcmEeftP9YQ3n7"
            "13YFwOvdW1SYvn7PxHA7gRZHk9AKaEtoc/igy7ZCOzs7Vtw/AQYASsoRySkHkqIAAAAASUVORK5C"
            "YII=";

    auto bytes = QByteArray::fromBase64(iconData);
    QPixmap pixmap;
    pixmap.loadFromData(bytes);

    qtnResetIcon.addPixmap(pixmap);

    return qtnResetIcon;
}

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
                                , qtnElidedText(*context.painter, property()->displayName(), item.rect));

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
        QIcon icon = resetIcon();
        if (!icon.availableSizes().empty())
        {
            option.icon = resetIcon();
            option.iconSize = icon.availableSizes().first();
        }
        else
            option.text = "R";

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

QtnDoubleSpinBox::QtnDoubleSpinBox(QWidget* parent)
    : QDoubleSpinBox(parent)
{}

QString QtnDoubleSpinBox::textFromValue(double val) const
{
    // get original text
    QString text = QDoubleSpinBox::textFromValue(val);

    // remove thousand separator
    text.remove(locale().groupSeparator());

    // remove trailing zero chars after decimal point
    int index = text.lastIndexOf(locale().decimalPoint());
    if (index != -1)
    {
        int lastDigit = text.length() - 1;
        for (int i = lastDigit; i > index; --i)
        {
            if (text[i] == '0')
                lastDigit = i;
            else
                break;
        }
        if (lastDigit == index + 1)
            lastDigit = index;

        text.remove(lastDigit, text.length() - 1);
    }

    return text;
}
