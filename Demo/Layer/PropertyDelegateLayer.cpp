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

#include "PropertyDelegateLayer.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "PropertyLayer.h"
#include "Delegates/Utils/PropertyEditorHandler.h"

#include <QComboBox>
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QPaintEvent>

void regLayerDelegates()
{
  QtnPropertyDelegateFactory::staticInstance()
    .registerDelegateDefault(&QtnPropertyLayerBase::staticMetaObject
                            , &qtnCreateDelegate<QtnPropertyDelegateLayer, QtnPropertyLayerBase>);
}

static void drawLayer(const LayerInfo& layer, QPainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip)
{
    QRect textRect = rect;

    QRect colorRect = rect;
    colorRect.setWidth(colorRect.height());
    colorRect.adjust(2, 2, -2, -2);

    painter.fillRect(colorRect, Qt::black);
    colorRect.adjust(1, 1, -1, -1);
    painter.fillRect(colorRect, layer.color);

    textRect.setLeft(colorRect.right() + 5);

    if (textRect.isValid())
    {
        qtnDrawValueText(layer.name, painter, textRect, state, needTooltip);
    }
}

class QtnPropertyLayerComboBox : public QComboBox
{
public:
    explicit QtnPropertyLayerComboBox(QtnPropertyLayerBase& property, QWidget *parent = Q_NULLPTR)
        : QComboBox(parent),
          m_property(property),
          m_layers(property.layers())
    {
        setLineEdit(nullptr);
        setItemDelegate(new ItemDelegate(m_layers));
        for (const auto& layer : m_layers)
        {
            Q_UNUSED(layer);
            addItem("");
        }

        updateEditor();

        if (!property.isEditableByUser())
            setDisabled(true);

        QObject::connect(  &m_property, &QtnPropertyBase::propertyDidChange,
                           this, &QtnPropertyLayerComboBox::onPropertyDidChange, Qt::QueuedConnection);
        QObject::connect(  this, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                         , this, &QtnPropertyLayerComboBox::onCurrentIndexChanged);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QComboBox::paintEvent(event);

        QPainter painter(this);

        auto index = currentIndex();
        if (index != -1)
        {
            Q_ASSERT(index < m_layers.size());
            drawLayer(m_layers[index], painter, event->rect(), QStyle::State_Active|QStyle::State_Enabled, nullptr);
        }
    }

    void updateEditor()
    {
        setCurrentIndex(m_property.value());
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
            Q_ASSERT(index < m_layers.size());
            m_property.setValue(index);
        }
    }

private:
    QtnPropertyLayerBase& m_property;
    QList<LayerInfo> m_layers;

    class ItemDelegate : public QStyledItemDelegate
    {
    public:
        ItemDelegate(const QList<LayerInfo>& layers)
            : m_layers(layers)
        {
        }

        void paint(QPainter *painter,
                    const QStyleOptionViewItem &option,
                    const QModelIndex &index) const override
        {
            QStyledItemDelegate::paint(painter, option, index);

            Q_ASSERT(index.row() < m_layers.size());
            drawLayer(m_layers[index.row()], *painter, option.rect, option.state, nullptr);
        }

    private:
        const QList<LayerInfo>& m_layers;
    };
};

void QtnPropertyDelegateLayer::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    auto layer = owner().valueLayer();
    if (layer)
        drawLayer(*layer, painter, rect, state, needTooltip);
}

QWidget* QtnPropertyDelegateLayer::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    if (owner().isEditableByUser())
    {
        auto combo = new QtnPropertyLayerComboBox(owner(), parent);
        combo->setGeometry(rect);
        if (inplaceInfo)
            combo->showPopup();

        return combo;
    }

    return nullptr;
}
