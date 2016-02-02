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
    QtnPropertyWidget(QtnPropertyView* propertyView, QWidget* parent = 0);
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
