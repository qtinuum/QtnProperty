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

#ifndef PROPERTY_DELEGATE_QCOLOR_H
#define PROPERTY_DELEGATE_QCOLOR_H

#include "../PropertyDelegate.h"

namespace Qtinuum
{

class PropertyQColorBase;

class QTN_PW_EXPORT PropertyDelegateQColor: public PropertyDelegateTyped<PropertyQColorBase>
{
    Q_DISABLE_COPY(PropertyDelegateQColor)

public:
    PropertyDelegateQColor(PropertyQColorBase& owner);

protected:
    void applyAttributesImpl(const PropertyDelegateAttributes& attributes) override;
    void drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip = 0) const override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo = 0) override;
    bool propertyValueToStr(QString& strValue) const override;

private:
    quint32 m_shape;
};

} // end namespace Qtinuum

#endif // PROPERTY_DELEGATE_QCOLOR_H
