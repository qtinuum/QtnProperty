/*
 * Copyright (c) 2012 - 2014, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
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
