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

#include "PropertyDelegatePenWidth.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "PropertyPenWidth.h"
#include "Delegates/Utils/PropertyEditorHandler.h"

#include <QComboBox>
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QPaintEvent>

void regPenWidthDelegates()
{
  QtnPropertyDelegateFactory::staticInstance()
    .registerDelegateDefault(&QtnPropertyPenWidthBase::staticMetaObject
                            , &qtnCreateDelegate<QtnPropertyDelegatePenWidth, QtnPropertyPenWidthBase>);
}

static void drawPenWidth(PenWidth penWidth, QPainter& painter, QRect rect, const QStyle::State& state, bool* needTooltip)
{
    if (needTooltip) *needTooltip = true;
    rect.adjust(2, 2, -2, -2);

    QPen pen = painter.pen();

    switch (penWidth) {
    case PenWidth::Default:
        qtnDrawValueText("Default", painter, rect, state, nullptr);
        return;
    case PenWidth::Thin:
        pen.setWidth(1);
        break;
    case PenWidth::Middle:
        pen.setWidth(2);
        break;
    case PenWidth::Thick:
        pen.setWidth(3);
        break;
    default:
        break;
    }

    painter.save();
    painter.setPen(pen);
    auto midY = rect.center().y();
    painter.drawLine(rect.left(), midY, rect.right(), midY);
    painter.restore();
}

class QtnPropertyPenWidthComboBox : public QComboBox
{
public:
    explicit QtnPropertyPenWidthComboBox(QtnPropertyPenWidthBase& property, QWidget *parent = Q_NULLPTR)
        : QComboBox(parent),
          m_property(property)
    {
        setLineEdit(nullptr);
        setItemDelegate(new ItemDelegate());

        addItem("", QVariant::fromValue(PenWidth::Default));
        addItem("", QVariant::fromValue(PenWidth::Thin));
        addItem("", QVariant::fromValue(PenWidth::Middle));
        addItem("", QVariant::fromValue(PenWidth::Thick));

        updateEditor();

        if (!property.isEditableByUser())
            setDisabled(true);

        QObject::connect(  &m_property, &QtnPropertyBase::propertyDidChange,
                           this, &QtnPropertyPenWidthComboBox::onPropertyDidChange, Qt::QueuedConnection);
        QObject::connect(  this, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                         , this, &QtnPropertyPenWidthComboBox::onCurrentIndexChanged);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QComboBox::paintEvent(event);

        QPainter painter(this);

        auto index = currentIndex();
        if (index != -1)
        {
            auto penWidth = currentData().value<PenWidth>();
            drawPenWidth(penWidth, painter, event->rect().adjusted(0, 0, -event->rect().height(), 0), QStyle::State_Active|QStyle::State_Enabled, nullptr);
        }
    }

    void updateEditor()
    {
        for (int i = 0; i < count(); ++i)
        {
            if (itemData(i).value<PenWidth>() == m_property.value())
                setCurrentIndex(i);
        }
    }

    void onPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
    {
        Q_UNUSED(changedProperty);

        if ((reason & QtnPropertyChangeReasonValue) && (&m_property == firedProperty))
            updateEditor();
    }

    void onCurrentIndexChanged(int index)
    {
        if (index >= 0)
        {
            m_property.setValue(itemData(index).value<PenWidth>());
        }
    }

private:
    QtnPropertyPenWidthBase& m_property;

    class ItemDelegate : public QStyledItemDelegate
    {
    public:
        ItemDelegate() {}

        void paint(QPainter *painter,
                    const QStyleOptionViewItem &option,
                    const QModelIndex &index) const override
        {
            QStyledItemDelegate::paint(painter, option, index);
            drawPenWidth(index.data(Qt::UserRole).value<PenWidth>(), *painter, option.rect, option.state, nullptr);
        }
    };
};

bool QtnPropertyDelegatePenWidth::propertyValueToStrImpl(QString& strValue) const
{
    return owner().toStr(strValue);
}

void QtnPropertyDelegatePenWidth::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    drawPenWidth(owner().value(), painter, rect, state, needTooltip);
}

QWidget* QtnPropertyDelegatePenWidth::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    if (owner().isEditableByUser())
    {
        auto combo = new QtnPropertyPenWidthComboBox(owner(), parent);
        combo->setGeometry(rect);
        if (inplaceInfo)
            combo->showPopup();

        return combo;
    }

    return nullptr;
}
