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

void regPenStyleDelegates()
{
  QtnPropertyDelegateFactory::staticInstance()
    .registerDelegateDefault(&QtnPropertyQPenStyleBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateQPenStyle, QtnPropertyQPenStyleBase>
                 , "ComboBox");
}

static void DrawPenStyle(QPainter& painter, QRect rect, Qt::PenStyle penStyle)
{
    rect.adjust(2, 2, -2, -2);
    QPen pen = painter.pen();
    pen.setStyle(penStyle);
    painter.save();
    painter.setPen(pen);
    auto midY = rect.top() + rect.height() / 2;
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
        DrawPenStyle(*painter, option.rect, penStyle);
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
        DrawPenStyle(painter, event->rect().adjusted(0, 0, -event->rect().height(), 0), penStyle);
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
    : QtnPropertyDelegateTyped<QtnPropertyQPenStyleBase>(owner)
{
}

void QtnPropertyDelegateQPenStyle::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& /*state*/, bool* needTooltip) const
{
    Qt::PenStyle value = owner().value();
    DrawPenStyle(painter, rect, value);

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
        for (auto ps = (int)Qt::NoPen; ps < Qt::CustomDashLine; ++ps)
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

