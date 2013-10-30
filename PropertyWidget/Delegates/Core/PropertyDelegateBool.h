/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
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

namespace Qtinuum
{

class PropertyBoolBase;

class QTN_PW_EXPORT PropertyDelegateBoolCheck: public PropertyDelegateTyped<PropertyBoolBase>
{
    Q_DISABLE_COPY(PropertyDelegateBoolCheck)

public:
    PropertyDelegateBoolCheck(PropertyBoolBase& owner)
        : PropertyDelegateTyped<PropertyBoolBase>(owner)
    {
    }

protected:
    void drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip = 0) const override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo = 0) override;
};

class QTN_PW_EXPORT PropertyDelegateBoolLabels: public PropertyDelegateTyped<PropertyBoolBase>
{
    Q_DISABLE_COPY(PropertyDelegateBoolLabels)

public:
    PropertyDelegateBoolLabels(PropertyBoolBase& owner);

protected:
    void applyAttributesImpl(const PropertyDelegateAttributes& attributes) override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo = 0) override;
    bool propertyValueToStr(QString& strValue) const override;

private:
    QString m_labels[2];
};

} // end namespace Qtinuum

#endif // PROPERTY_DELEGATE_BOOL_H
