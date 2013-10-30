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

#ifndef QTN_PROPERTYWIDGET_H
#define QTN_PROPERTYWIDGET_H

#include "PropertyWidgetAPI.h"
#include "PropertyView.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

namespace Qtinuum
{

enum PropertyWidgetPartsFlag
{
    PropertyWidgetPartsNone = 0x0000,
    PropertyWidgetPartsToolbar = 0x0001,
    PropertyWidgetPartsDescriptionPanel = 0x0002
};
Q_DECLARE_FLAGS(PropertyWidgetParts, PropertyWidgetPartsFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(PropertyWidgetParts)

class QTN_PW_EXPORT PropertyWidget: public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(PropertyWidget)

public:
    explicit PropertyWidget(QWidget* parent = 0);
    ~PropertyWidget();

    PropertyWidgetParts parts() const { return m_parts; }
    void setParts(PropertyWidgetParts newParts);

    const PropertySet* propertySet() const { return m_propertyView->propertySet(); }
    PropertySet* propertySet() { return m_propertyView->propertySet(); }
    void setPropertySet(PropertySet* newPropertySet) { m_propertyView->setPropertySet(newPropertySet); }

    PropertyView *propertyView() { return m_propertyView; }

private:
    void updateParts();
    void setActiveProperty(const PropertyBase* activeProperty);

private:
    PropertyWidgetParts m_parts;

    QVBoxLayout* m_layout;
    QLabel* m_toolbar;
    PropertyView* m_propertyView;
    QWidget* m_descriptionSplitter;
    QLabel* m_descriptionPanel;
};

} // end namespace Qtinuum

#endif // QTN_PROPERTYWIDGET_H
