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

#include "PropertyWidget.h"
#include <QSplitter>
#include <QMouseEvent>

class QtnSplitterEventsHandler: public QObject
{
public:
    QtnSplitterEventsHandler(QObject* parent)
        : QObject(parent)
    {
    }

protected:
    bool eventFilter(QObject* obj, QEvent* event) override
    {
        // check input
        if (event->type() != QEvent::MouseButtonDblClick)
            return false;

         QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
         if (mouseEvent->button() != Qt::LeftButton)
             return false;

        QSplitterHandle* splitterHandle = qobject_cast<QSplitterHandle*>(obj);
        if (!splitterHandle)
            return false;

        QSplitter* splitter = splitterHandle->splitter();
        if (!splitter || splitter->count() < 2)
            return false;

        // change splitter sizes to make description panel occupy ideal height
        QWidget* bottomWidget = splitter->widget(1);
        QList<int> sizes = splitter->sizes();
        if (sizes.size() != 2)
            return false;

        sizes[0] += sizes[1];
        sizes[1] = bottomWidget->heightForWidth(bottomWidget->size().width());
        sizes[0] -= qMax(sizes[1], 0);

        splitter->setSizes(sizes);

        return true;
    }
};

QtnPropertyWidget::QtnPropertyWidget(QWidget* parent)
    : QWidget(parent),
      m_parts(QtnPropertyWidgetPartsNone),
      m_layout(new QVBoxLayout(this)),
      m_toolbar(0),
      m_propertyView(new QtnPropertyView(this)),
      m_descriptionSplitter(0),
      m_descriptionPanel(0)
{
    m_layout->addWidget(m_propertyView);

    QObject::connect(m_propertyView, &QtnPropertyView::activePropertyChanged, this, &QtnPropertyWidget::setActiveProperty);
}

QtnPropertyWidget::~QtnPropertyWidget()
{

}

void QtnPropertyWidget::setParts(QtnPropertyWidgetParts newParts)
{
    if (m_parts == newParts)
        return;

    m_parts = newParts;
    updateParts();
}

void QtnPropertyWidget::updateParts()
{
    // clear layout
    while (!m_layout->isEmpty())
        m_layout->takeAt(0);

    // check toolbar

    // check description panel
    if (m_parts & QtnPropertyWidgetPartsDescriptionPanel)
    {
        if (!m_descriptionSplitter)
        {
            // create splitter
            Q_ASSERT(!m_descriptionPanel);
            QSplitter* splitter = new QSplitter(Qt::Vertical, this);

            // add property view
            splitter->addWidget(m_propertyView);

            // create description panel
            m_descriptionPanel = new QLabel(splitter);
            m_descriptionPanel->setTextFormat(Qt::RichText);
            m_descriptionPanel->setAlignment(Qt::AlignTop);
            m_descriptionPanel->setWordWrap(true);
            m_descriptionPanel->setFrameStyle(QFrame::Box | QFrame::Sunken);
            m_descriptionPanel->setMinimumSize(0, 0);
            QSizePolicy p = m_descriptionPanel->sizePolicy();
            p.setVerticalPolicy(QSizePolicy::Ignored);
            p.setHorizontalPolicy(QSizePolicy::Ignored);
            m_descriptionPanel->setSizePolicy(p);

            // setup DblClick handler
            splitter->handle(1)->installEventFilter(new QtnSplitterEventsHandler(splitter));

            m_descriptionSplitter = splitter;
        }

        m_layout->addWidget(m_descriptionSplitter);
    }
    else
    {
        if (m_descriptionSplitter)
        {
            delete m_descriptionSplitter;
            m_descriptionSplitter = nullptr;
            m_descriptionPanel = nullptr;
        }

        m_layout->addWidget(m_propertyView);
    }
}

void QtnPropertyWidget::setActiveProperty(const QtnPropertyBase* activeProperty)
{
    if (m_descriptionPanel)
    {
        if (!activeProperty)
        {
            m_descriptionPanel->setText("");
        }
        else
        {
            m_descriptionPanel->setText(QString("<b>%1</b><br>%2").arg(activeProperty->name(), activeProperty->description()));
        }
    }
}
