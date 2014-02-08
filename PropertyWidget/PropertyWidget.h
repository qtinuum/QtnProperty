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

#ifndef QTN_PROPERTYWIDGET_H
#define QTN_PROPERTYWIDGET_H

#include "PropertyWidgetAPI.h"
#include "PropertyView.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

enum QtnPropertyWidgetPartsFlag
{
    QtnPropertyWidgetPartsNone = 0x0000,
    QtnPropertyWidgetPartsToolbar = 0x0001,
    QtnPropertyWidgetPartsDescriptionPanel = 0x0002
};
Q_DECLARE_FLAGS(QtnPropertyWidgetParts, QtnPropertyWidgetPartsFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtnPropertyWidgetParts)

class QTN_PW_EXPORT QtnPropertyWidget: public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(QtnPropertyWidget)

public:
    explicit QtnPropertyWidget(QWidget* parent = 0);
    ~QtnPropertyWidget();

    QtnPropertyWidgetParts parts() const { return m_parts; }
    void setParts(QtnPropertyWidgetParts newParts);

    const QtnPropertySet* propertySet() const { return m_propertyView->propertySet(); }
    QtnPropertySet* propertySet() { return m_propertyView->propertySet(); }
    void setPropertySet(QtnPropertySet* newPropertySet) { m_propertyView->setPropertySet(newPropertySet); }

    QtnPropertyView* propertyView() { return m_propertyView; }

private:
    void updateParts();
    void setActiveProperty(const QtnPropertyBase* activeProperty);

private:
    QtnPropertyWidgetParts m_parts;

    QVBoxLayout* m_layout;
    QLabel* m_toolbar;
    QtnPropertyView* m_propertyView;
    QWidget* m_descriptionSplitter;
    QLabel* m_descriptionPanel;
};

#endif // QTN_PROPERTYWIDGET_H
