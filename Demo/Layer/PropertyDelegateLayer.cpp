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

void regLayerDelegates()
{
  QtnPropertyDelegateFactory::staticInstance()
    .registerDelegateDefault(&QtnPropertyLayerBase::staticMetaObject
                            , &qtnCreateDelegate<QtnPropertyDelegateLayer, QtnPropertyLayerBase>);
}
/*
class QtnPropertyLayerComboBox : public QComboBox
{
public:
    explicit QtnPropertyLayerComboBox(QtnPropertyLayerBase& property, QWidget *parent = Q_NULLPTR)
        : QComboBox(parent)
    {

    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QComboBox::paintEvent(event);

        QPainter painter(this);
        auto index = currentData().to Int();
        DrawPenStyle(painter, event->rect().adjusted(0, 0, -event->rect().height(), 0), penStyle);
    }

private:
    QtnPropertyLayerBase& m_property;
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
*/
void QtnPropertyDelegateLayer::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    auto layer = owner().value();

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
        QtnPropertyDelegateTyped<QtnPropertyLayerBase>::drawValueImpl(painter, textRect, state, needTooltip);
    }
}

bool QtnPropertyDelegateLayer::propertyValueToStrImpl(QString& strValue) const
{
    strValue = owner().value().name;
    return true;
}

QWidget* QtnPropertyDelegateLayer::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    /*
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
    */
    return createValueEditorLineEdit(parent, rect, !owner().isEditableByUser(), inplaceInfo);
}
