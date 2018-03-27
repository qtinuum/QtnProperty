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

#include "PropertyDelegateQBrush.h"
#include "../../../Core/GUI/PropertyQBrush.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"
#include "../Utils/PropertyEditorAux.h"

#include <QComboBox>
#include <QStyledItemDelegate>
#include <QPaintEvent>

#include <Core/PropertyInt.h>

#include <GUI/PropertyQColor.h>

void regQBrushStyleDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyQBrushStyleBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateQBrushStyle, QtnPropertyQBrushStyleBase>
                 , "ComboBox");
}

static void drawBrushStyle(QPainter& painter, QRect rect, const QStyle::State& state, Qt::BrushStyle brushStyle)
{
    rect.adjust(2, 2, -2, -2);

    switch (brushStyle)
    {
        case Qt::NoBrush:
        case Qt::LinearGradientPattern:
        case Qt::RadialGradientPattern:
        case Qt::ConicalGradientPattern:
        case Qt::TexturePattern:
        {
            auto str = QtnPropertyQBrushStyleBase::brushStyleToStr(brushStyle);
            qtnDrawValueText(str, painter, rect, state, nullptr);
            break;
        }

        default:
        {
            auto brush = painter.brush();
            brush.setStyle(brushStyle);
            painter.save();
            painter.setBrush(brush);
            painter.drawRect(rect);
            painter.restore();
        }
    }
}

class QtnPropertyBrushStyleItemDelegate : public QStyledItemDelegate
{
public:
    void paint(QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);
        auto brushStyle = index.data(Qt::UserRole).value<Qt::BrushStyle>();
        drawBrushStyle(*painter, option.rect, option.state, brushStyle);
    }
};

class QtnPropertyBrushStyleComboBox : public QComboBox
{
public:
    explicit QtnPropertyBrushStyleComboBox(QWidget *parent = Q_NULLPTR)
        : QComboBox(parent)
    {}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QComboBox::paintEvent(event);

        QPainter painter(this);
        auto brushStyle = currentData().value<Qt::BrushStyle>();
        drawBrushStyle(painter, event->rect().adjusted(0, 0, -event->rect().height(), 0), QStyle::State_Active|QStyle::State_Enabled, brushStyle);
    }
};

class QtnPropertyBrushStyleComboBoxHandler: public QtnPropertyEditorHandler<QtnPropertyQBrushStyleBase, QComboBox>
{
public:
    QtnPropertyBrushStyleComboBoxHandler(QtnPropertyQBrushStyleBase& property, QComboBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(  &editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                         , this, &QtnPropertyBrushStyleComboBoxHandler::onCurrentIndexChanged);
    }

private:
    void updateEditor() override
    {
        int index = editor().findData((int)property());
        Q_ASSERT(index >= 0);
        editor().setCurrentIndex(index);
    }

    void onCurrentIndexChanged(int index)
    {
        QVariant data = editor().itemData(index);
        if (data.canConvert<Qt::BrushStyle>())
            property() = data.value<Qt::BrushStyle>();
    }
};

QtnPropertyDelegateQBrushStyle::QtnPropertyDelegateQBrushStyle(QtnPropertyQBrushStyleBase &owner)
    : QtnPropertyDelegateTyped<QtnPropertyQBrushStyleBase>(owner),
      m_showAll(false)
{
}

void QtnPropertyDelegateQBrushStyle::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    qtnGetAttribute(attributes, "showAll", m_showAll);
}

void QtnPropertyDelegateQBrushStyle::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    auto value = owner().value();
    drawBrushStyle(painter, rect, state, value);

    if (needTooltip)
        *needTooltip = true;
}

QWidget* QtnPropertyDelegateQBrushStyle::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    if (owner().isEditableByUser())
    {
        QComboBox* combo = new QtnPropertyBrushStyleComboBox(parent);
        combo->setLineEdit(nullptr);
        combo->setItemDelegate(new QtnPropertyBrushStyleItemDelegate());
        if (m_showAll)
        {
            for (auto bs = Qt::NoBrush; bs <= Qt::ConicalGradientPattern; bs = Qt::BrushStyle(bs + 1))
            {
                combo->addItem("", QVariant::fromValue(bs));
            }
            combo->addItem("", QVariant::fromValue(Qt::TexturePattern));
        }
        else
        {
            combo->addItem("", QVariant::fromValue(Qt::NoBrush));
            combo->addItem("", QVariant::fromValue(Qt::SolidPattern));
            combo->addItem("", QVariant::fromValue(Qt::HorPattern));
            combo->addItem("", QVariant::fromValue(Qt::VerPattern));
            combo->addItem("", QVariant::fromValue(Qt::CrossPattern));
            combo->addItem("", QVariant::fromValue(Qt::BDiagPattern));
            combo->addItem("", QVariant::fromValue(Qt::FDiagPattern));
            combo->addItem("", QVariant::fromValue(Qt::DiagCrossPattern));
        }

        combo->setGeometry(rect);

        new QtnPropertyBrushStyleComboBoxHandler(owner(), *combo);

        if (inplaceInfo)
            combo->showPopup();

        return combo;
    }

    return nullptr;
}

bool QtnPropertyDelegateQBrushStyle::propertyValueToStrImpl(QString& strValue) const
{
    return owner().toStr(strValue);
}
