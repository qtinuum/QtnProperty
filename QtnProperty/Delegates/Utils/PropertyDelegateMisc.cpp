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

static const quint32 qtn_u_2 = std::numeric_limits<quint32>::max() / 2 + 1;
static qint32 qtn_u2i(quint32 val)
{
    return qint32(val - qtn_u_2);
}
static quint32 qtn_i2u(qint32 val)
{
    return (quint32)val + qtn_u_2;
}

QIcon qtnResetIcon;

static QIcon resetIcon()
{
    if (!qtnResetIcon.isNull())
        return qtnResetIcon;

    const char iconData16[] = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJ"
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

    const char iconData32[] = "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";

    {
        auto bytes = QByteArray::fromBase64(iconData16);
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        qtnResetIcon.addPixmap(pixmap);
    }

    {
        auto bytes = QByteArray::fromBase64(iconData32);
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        qtnResetIcon.addPixmap(pixmap);
    }

    return qtnResetIcon;
}

void QtnPropertyDelegateWithValues::createSubItemsImpl(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    addSubItemBackground(context, subItems);
    addSubItemSelection(context, subItems);
    addSubItemBranchNode(context, subItems);
    addSubItemName(context, subItems);
    addSubItemReset(context, subItems);
    addSubItemValues(context, subItems);
}

void QtnPropertyDelegateWithValues::addSubItemBackground(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    QtnSubItem bgItem(context.rect);

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

void QtnPropertyDelegateWithValues::addSubItemSelection(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    QtnSubItem selItem(context.rect);
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

void QtnPropertyDelegateWithValues::addSubItemBranchNode(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    if (!context.hasChildren)
        return;

    QtnSubItem brItem(context.rect.marginsRemoved(context.margins));
    brItem.rect.setWidth(brItem.rect.height());
    context.margins.setLeft(context.margins.left() + brItem.rect.height());
    brItem.trackState();

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

void QtnPropertyDelegateWithValues::addSubItemName(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    QtnSubItem nameItem(context.rect.marginsRemoved(context.margins));
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


void QtnPropertyDelegateWithValues::addSubItemReset(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    if (!propertyImmutable()->hasResetCallback())
        return;

    QtnSubItem resetItem(context.rect.marginsRemoved(context.margins));
    resetItem.rect.setLeft(resetItem.rect.right() - resetItem.rect.height());
    resetItem.setTextAsTooltip("Reset");
    resetItem.trackState();

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
            option.icon = icon;
            option.iconSize = icon.actualSize(item.rect.size());
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

void QtnPropertyDelegateWithValues::addSubItemValues(QtnDrawContext& context, QList<QtnSubItem>& subItems)
{
    auto rect = context.rect.marginsRemoved(context.margins);
    rect.setLeft(context.splitPos + context.widget->valueLeftMargin());

    if (rect.isValid())
        createSubItemValuesImpl(context, rect, subItems);
}

void QtnPropertyDelegateWithValue::createSubItemValuesImpl(QtnDrawContext& context, const QRect& valueRect, QList<QtnSubItem>& subItems)
{
    QtnSubItem subItem(valueRect);
    if (createSubItemValueImpl(context, subItem))
        subItems.append(subItem);
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

QtnSpinBoxUnsigned::QtnSpinBoxUnsigned(QWidget* parent)
    : QSpinBox(parent),
      m_min(0),
      m_max(0)
{
    QObject::connect(  this, static_cast<void (QSpinBox::*)(int)>(&QtnSpinBoxUnsigned::valueChanged)
                     , this, &QtnSpinBoxUnsigned::onValueChanged);
}

void QtnSpinBoxUnsigned::setUintRange(quint32 minValue, quint32 maxValue)
{
    m_min = minValue;
    m_max = maxValue;
    setRange(qtn_u2i(minValue), qtn_u2i(maxValue));
}

void QtnSpinBoxUnsigned::setUintSingleStep(quint32 stepValue)
{
    setSingleStep(qtn_u2i(stepValue));
}

void QtnSpinBoxUnsigned::setUintValue(quint32 value)
{
    setValue(qtn_u2i(value));
}

int QtnSpinBoxUnsigned::valueFromText(const QString& text) const
{
    return qtn_u2i(locale().toUInt(text));
}

QString QtnSpinBoxUnsigned::textFromValue(int val) const
{
    return locale().toString(qtn_i2u(val));
}

QValidator::State QtnSpinBoxUnsigned::validate(QString &text, int &) const
{
    bool ok = false;
    auto value = locale().toUInt(text, &ok);
    if (!ok)
        return QValidator::Invalid;
    if (value < m_min)
        return QValidator::Intermediate;
    if (value > m_max)
        return QValidator::Intermediate;

    return QValidator::Acceptable;
}

void QtnSpinBoxUnsigned::onValueChanged(int value)
{
    Q_EMIT uintValueChanged(qtn_i2u(value));
}

