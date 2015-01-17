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

#include "PropertyDelegateQColor.h"
#include "../../../Core/GUI/PropertyQColor.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"
#include "../PropertyEditorAux.h"

#include <QColorDialog>

class QtnPropertyQColorLineEditBttnHandler: public QtnPropertyEditorHandler<QtnPropertyQColorBase, QtnLineEditBttn>
{
public:
    QtnPropertyQColorLineEditBttnHandler(QtnPropertyQColorBase& property, QtnLineEditBttn& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        editor.lineEdit->setReadOnly(true);

        if (!property.isEditableByUser())
        {
//            editor.lineEdit->setReadOnly(true);
            editor.toolButton->setEnabled(false);
        }

        updateEditor();

        QObject::connect(  editor.toolButton, &QToolButton::clicked
                         , this, &QtnPropertyQColorLineEditBttnHandler::onToolButtonClicked);
    }

private:
    void updateEditor() override
    {
        editor().lineEdit->setText(property().value().name());
    }

    void onToolButtonClicked(bool checked)
    {
        QColorDialog dlg(property(), &editor());
        if (dlg.exec() == QDialog::Accepted)
        {
            property() = dlg.currentColor();
        }
    }
};

static bool regQColorDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyQColorBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateQColor, QtnPropertyQColorBase>
                                , "LineEditBttn");

QtnPropertyDelegateQColor::QtnPropertyDelegateQColor(QtnPropertyQColorBase& owner)
    : QtnPropertyDelegateTyped<QtnPropertyQColorBase>(owner),
      m_shape(QtnColorDelegateShapeSquare)
{
}

void QtnPropertyDelegateQColor::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    qtnGetAttribute(attributes, "shape", m_shape);
}

void QtnPropertyDelegateQColor::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    QColor value = owner().value();

    QRect textRect = rect;

    if (m_shape != QtnColorDelegateShapeNone)
    {
        QRect colorRect = rect;
        colorRect.setRight(colorRect.left() + colorRect.height());
        colorRect.adjust(2, 2, -2, -2);

        if (m_shape == QtnColorDelegateShapeSquare)
        {
            painter.fillRect(colorRect, Qt::black);
            colorRect.adjust(1, 1, -1, -1);
            painter.fillRect(colorRect, value);
        }
        else if (m_shape == QtnColorDelegateShapeCircle)
        {
            bool oldAntiAliasing = painter.testRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::Antialiasing);

            QPainterPath path;
            path.addEllipse(colorRect);
            painter.fillPath(path, value);
            painter.drawEllipse(colorRect);

            painter.setRenderHint(QPainter::Antialiasing, oldAntiAliasing);
        }

        textRect.setLeft(colorRect.right() + 3);
    }

    if (textRect.isValid())
    {
        QtnPropertyDelegateTyped<QtnPropertyQColorBase>::drawValueImpl(painter, textRect, state, needTooltip);
    }
}

QWidget* QtnPropertyDelegateQColor::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QtnLineEditBttn* editor = new QtnLineEditBttn(parent);
    editor->setGeometry(rect);

    new QtnPropertyQColorLineEditBttnHandler(owner(), *editor);

    if (inplaceInfo)
    {
        editor->lineEdit->selectAll();
    }

    return editor;
}

bool QtnPropertyDelegateQColor::propertyValueToStr(QString& strValue) const
{
    strValue = owner().value().name();
    return true;
}
