/*
 * Copyright (c) 2012 - 2014, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
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
    return createValueEditorImpl(parent, rect, inplaceInfo);
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

