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

#ifndef PROPERTY_DELEGATE_BOOL_H
#define PROPERTY_DELEGATE_BOOL_H

#include "../PropertyDelegate.h"

class QtnPropertyBoolBase;

class QTN_PW_EXPORT QtnPropertyDelegateBoolCheck: public QtnPropertyDelegateTyped<QtnPropertyBoolBase>
{
    Q_DISABLE_COPY(QtnPropertyDelegateBoolCheck)

public:
    QtnPropertyDelegateBoolCheck(QtnPropertyBoolBase& owner)
        : QtnPropertyDelegateTyped<QtnPropertyBoolBase>(owner)
    {
    }

protected:
    void drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip = nullptr) const override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
};

class QTN_PW_EXPORT QtnPropertyDelegateBoolCombobox: public QtnPropertyDelegateTyped<QtnPropertyBoolBase>
{
    Q_DISABLE_COPY(QtnPropertyDelegateBoolCombobox)

public:
    QtnPropertyDelegateBoolCombobox(QtnPropertyBoolBase& owner);

protected:
    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
    bool propertyValueToStr(QString& strValue) const override;

private:
    QString m_labels[2];
};

#endif // PROPERTY_DELEGATE_BOOL_H
