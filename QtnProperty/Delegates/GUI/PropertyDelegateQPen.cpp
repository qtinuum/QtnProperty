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

#include "PropertyDelegateQPen.h"
#include "../../../Core/GUI/PropertyQPen.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"
#include "../Utils/PropertyEditorAux.h"

#include <QComboBox>
#include <QStyledItemDelegate>
#include <QPaintEvent>

#include <Core/PropertyInt.h>

#include <GUI/PropertyQColor.h>

void regQPenStyleDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyQPenStyleBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateQPenStyle, QtnPropertyQPenStyleBase>
                 , "ComboBox");
}

static void drawPenStyle(QPainter& painter, QRect rect, Qt::PenStyle penStyle)
{
    rect.adjust(2, 2, -2, -2);
    QPen pen = painter.pen();
    pen.setStyle(penStyle);
    painter.save();
    painter.setPen(pen);
    auto midY = rect.center().y();
    painter.drawLine(rect.left(), midY, rect.right(), midY);
    painter.restore();
}

class QtnPropertyPenStyleItemDelegate : public QStyledItemDelegate
{
public:
    void paint(QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);
        auto penStyle = (Qt::PenStyle)index.data(Qt::UserRole).toInt();
        drawPenStyle(*painter, option.rect, penStyle);
    }
};

class QtnPropertyPenStyleComboBox : public QComboBox
{
public:
    explicit QtnPropertyPenStyleComboBox(QWidget *parent = Q_NULLPTR)
        : QComboBox(parent)
    {}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QComboBox::paintEvent(event);

        QPainter painter(this);
        auto penStyle = (Qt::PenStyle)currentData().toInt();
        drawPenStyle(painter, event->rect().adjusted(0, 0, -event->rect().height(), 0), penStyle);
    }
};

class QtnPropertyPenStyleComboBoxHandler: public QtnPropertyEditorHandler<QtnPropertyQPenStyleBase, QComboBox>
{
public:
    QtnPropertyPenStyleComboBoxHandler(QtnPropertyQPenStyleBase& property, QComboBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(  &editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                         , this, &QtnPropertyPenStyleComboBoxHandler::onCurrentIndexChanged);
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
        if (data.canConvert<int>())
            property() = (Qt::PenStyle)data.toInt();
    }
};

QtnPropertyDelegateQPenStyle::QtnPropertyDelegateQPenStyle(QtnPropertyQPenStyleBase &owner)
    : QtnPropertyDelegateTyped<QtnPropertyQPenStyleBase>(owner),
      m_showNoPen(false)
{
}

void QtnPropertyDelegateQPenStyle::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    qtnGetAttribute(attributes, "showNoPen", m_showNoPen);
}

void QtnPropertyDelegateQPenStyle::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& /*state*/, bool* needTooltip) const
{
    Qt::PenStyle value = owner().value();
    drawPenStyle(painter, rect, value);

    if (needTooltip)
        *needTooltip = true;
}

QWidget* QtnPropertyDelegateQPenStyle::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    if (owner().isEditableByUser())
    {
        QComboBox* combo = new QtnPropertyPenStyleComboBox(parent);
        combo->setLineEdit(nullptr);
        combo->setItemDelegate(new QtnPropertyPenStyleItemDelegate());
        auto startStyle = m_showNoPen ? Qt::NoPen : Qt::SolidLine;
        for (auto ps = (int)startStyle; ps < Qt::CustomDashLine; ++ps)
            combo->addItem("", ps);

        combo->setGeometry(rect);

        new QtnPropertyPenStyleComboBoxHandler(owner(), *combo);

        if (inplaceInfo)
            combo->showPopup();

        return combo;
    }

    return nullptr;
}

bool QtnPropertyDelegateQPenStyle::propertyValueToStrImpl(QString& strValue) const
{
    return owner().toStr(strValue);
}

void regQPenDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyQPenBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateQPen, QtnPropertyQPenBase>
                 , "LineEditBttn");
}

class QtnPropertyQPenLineEditHandler: public QtnPropertyEditorHandler<QtnPropertyQPenBase, QLineEdit>
{
public:
    QtnPropertyQPenLineEditHandler(QtnPropertyQPenBase& property, QLineEdit& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        editor.setReadOnly(true);

        updateEditor();
    }

private:
    void updateEditor() override
    {
        QString text;
        if (property().toStr(text))
            editor().setText(text);
    }
};

QtnPropertyDelegateQPen::QtnPropertyDelegateQPen(QtnPropertyQPenBase& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyQPenBase>(owner)
{
}

void QtnPropertyDelegateQPen::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    auto& owner = QtnPropertyDelegateQPen::owner();

    {
        bool editColor = true;
        qtnGetAttribute(attributes, "editColor", editColor);

        if (editColor)
        {
            auto propertyColor = new QtnPropertyQColorCallback(nullptr);
            addSubProperty(propertyColor);
            propertyColor->setName(owner.tr("Color"));
            propertyColor->setDescription(owner.tr("Pen color for %1.").arg(owner.name()));
            propertyColor->setCallbackValueGet([&owner]()->QColor {
                                                   return owner.value().color();
                                               });
            propertyColor->setCallbackValueSet([&owner](QColor value) {
                auto pen = owner.value();
                pen.setColor(value);
                owner.setValue(pen);
            });
        }
    }

    {
        bool editStyle = true;
        qtnGetAttribute(attributes, "editStyle", editStyle);

        if (editStyle)
        {
            auto propertyStyle = new QtnPropertyQPenStyleCallback(nullptr);
            addSubProperty(propertyStyle);
            propertyStyle->setName(owner.tr("PenStyle"));
            propertyStyle->setDescription(owner.tr("Pen style for %1.").arg(owner.name()));
            propertyStyle->setCallbackValueGet([&owner]()->Qt::PenStyle {
                return owner.value().style();
            });
            propertyStyle->setCallbackValueSet([&owner](Qt::PenStyle value) {
                auto pen = owner.value();
                pen.setStyle(value);
                owner.setValue(pen);
            });
        }
    }

    {
        bool editWidth = true;
        qtnGetAttribute(attributes, "editWidth", editWidth);

        if (editWidth)
        {
            auto propertyWidth = new QtnPropertyIntCallback(nullptr);
            addSubProperty(propertyWidth);
            propertyWidth->setName(owner.tr("Width"));
            propertyWidth->setDescription(owner.tr("Pen width for %1").arg(owner.name()));
            propertyWidth->setCallbackValueGet([&owner]()->qint32 {
                return owner.value().width();
            });
            propertyWidth->setCallbackValueSet([&owner](qint32 value) {
                auto pen = owner.value();
                pen.setWidth(value);
                owner.setValue(pen);
            });
            propertyWidth->setMinValue(1);
            propertyWidth->setMaxValue(20);
        }
    }

    {
        bool editCapStyle = true;
        qtnGetAttribute(attributes, "editCapStyle", editCapStyle);

        if (editCapStyle)
        {
            auto propertyCapStyle = new QtnPropertyEnumCallback(nullptr);
            addSubProperty(propertyCapStyle);
            propertyCapStyle->setName(owner.tr("CapStyle"));
            propertyCapStyle->setDescription(owner.tr("Pen Cap Style for %1.").arg(owner.name()));
            propertyCapStyle->setEnumInfo(QtnPropertyQPenBase::penCapStyleEnum());
            propertyCapStyle->setCallbackValueGet([&owner]()->QtnEnumValueType {
                return owner.value().capStyle();
            });
            propertyCapStyle->setCallbackValueSet([&owner](QtnEnumValueType value) {
                QPen pen = owner.value();
                pen.setCapStyle((Qt::PenCapStyle)value);
                owner.setValue(pen);
            });
        }
    }

    {
        bool editJoinStyle = true;
        qtnGetAttribute(attributes, "editJoinStyle", editJoinStyle);

        if (editJoinStyle)
        {
            auto propertyJoinStyle = new QtnPropertyEnumCallback(nullptr);
            addSubProperty(propertyJoinStyle);
            propertyJoinStyle->setName(owner.tr("JoinStyle"));
            propertyJoinStyle->setDescription(owner.tr("Pen Join Style for %1.").arg(owner.name()));
            propertyJoinStyle->setEnumInfo(QtnPropertyQPenBase::penJoinStyleEnum());
            propertyJoinStyle->setCallbackValueGet([&owner]()->QtnEnumValueType {
                return owner.value().joinStyle();
            });
            propertyJoinStyle->setCallbackValueSet([&owner](QtnEnumValueType value) {
                QPen pen = owner.value();
                pen.setJoinStyle((Qt::PenJoinStyle)value);
                owner.setValue(pen);
            });
        }
    }
}

void QtnPropertyDelegateQPen::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    QtnPropertyDelegateTypedEx<QtnPropertyQPenBase>::drawValueImpl(painter, rect, state, needTooltip);
}

QWidget* QtnPropertyDelegateQPen::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    auto editor = new QLineEdit(parent);
    editor->setGeometry(rect);

    new QtnPropertyQPenLineEditHandler(owner(), *editor);

    if (inplaceInfo)
    {
        editor->selectAll();
    }

    return editor;
}

bool QtnPropertyDelegateQPen::propertyValueToStrImpl(QString& strValue) const
{
    return owner().toStr(strValue);
}


