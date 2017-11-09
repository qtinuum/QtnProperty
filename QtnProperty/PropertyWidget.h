/*******************************************************************************
Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#ifndef QTN_PROPERTYWIDGET_H
#define QTN_PROPERTYWIDGET_H

#include "CoreAPI.h"
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

class QTN_IMPORT_EXPORT QtnPropertyWidget : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnPropertyWidget)

public:
	explicit QtnPropertyWidget(QWidget *parent = 0);
	virtual ~QtnPropertyWidget();

	inline QtnPropertyWidgetParts parts() const;
	void setParts(QtnPropertyWidgetParts newParts);

	inline const QtnPropertySet *propertySet() const;
	inline QtnPropertySet *propertySet();
	inline void setPropertySet(QtnPropertySet *newPropertySet);

	inline QtnPropertyView *propertyView() const;

private:
	void updateParts();
	void setActiveProperty(const QtnPropertyBase *activeProperty);

private:
	QtnPropertyWidgetParts m_parts;

	QVBoxLayout *m_layout;
	QLabel *m_toolbar;
	QtnPropertyView *m_propertyView;
	QWidget *m_descriptionSplitter;
	QLabel *m_descriptionPanel;
};

QtnPropertyWidgetParts QtnPropertyWidget::parts() const
{
	return m_parts;
}

const QtnPropertySet *QtnPropertyWidget::propertySet() const
{
	return m_propertyView->propertySet();
}

QtnPropertySet *QtnPropertyWidget::propertySet()
{
	return m_propertyView->propertySet();
}

void QtnPropertyWidget::setPropertySet(QtnPropertySet *newPropertySet)
{
	m_propertyView->setPropertySet(newPropertySet);
}

QtnPropertyView *QtnPropertyWidget::propertyView() const
{
	return m_propertyView;
}

#endif // QTN_PROPERTYWIDGET_H
