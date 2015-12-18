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

int QtnPropertyDelegate::subPropertyCount() const
{
    return subPropertyCountImpl();
}

QtnPropertyBase* QtnPropertyDelegate::subProperty(int index)
{
    return subPropertyImpl(index);
}

void QtnPropertyDelegate::applyAttributes(const QtnPropertyDelegateAttributes& attributes)
{
    applyAttributesImpl(attributes);
}

void QtnPropertyDelegate::drawValue(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    drawValueImpl(painter, rect, state, needTooltip);
}

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

void QtnPropertyDelegate::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    QString strValue;
    if (propertyValueToStr(strValue))
    {
        drawValueText(strValue, painter, rect, state, needTooltip);
    }
}

QString QtnPropertyDelegate::toolTipImpl() const
{
    QString strValue;
    propertyValueToStr(strValue);
    return strValue;
}

bool QtnPropertyDelegate::acceptKeyPressedForInplaceEditImpl(QKeyEvent* keyEvent) const
{
    int key = keyEvent->key();
    return (key == Qt::Key_F2) || (key == Qt::Key_Space) || (key == Qt::Key_Return);
}

void QtnPropertyDelegate::drawValueText(const QString& text, QStylePainter& painter, const QRect& rect, const QStyle::State &state, bool* needTooltip)
{
    if (text.isEmpty())
        return;

    painter.drawText(rect, Qt::AlignLeading | Qt::AlignVCenter
                     , qtnElidedText(painter, text, rect, needTooltip));
}

QWidget* QtnPropertyDelegate::createValueEditorLineEdit(QWidget* parent, const QRect& rect, bool readOnly, QtnInplaceInfo* inplaceInfo)
{
    QLineEdit* lineEdit = new QLineEdit(parent);
    lineEdit->setGeometry(rect);
    lineEdit->setReadOnly(readOnly);

    QString strValue;
    propertyValueToStr(strValue);
    lineEdit->setText(strValue);

    if (inplaceInfo)
    {
        lineEdit->selectAll();
    }

    return lineEdit;
}

