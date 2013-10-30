/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
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

namespace Qtinuum
{

class PropertyQColorLineEditBttnHandler: public PropertyEditorHandler<PropertyQColorBase, LineEditBttn>
{
public:
    PropertyQColorLineEditBttnHandler(PropertyQColorBase& property, LineEditBttn& editor)
        : PropertyEditorHandlerType(property, editor)
    {
        editor.lineEdit->setReadOnly(true);

        if (!property.isEditableByUser())
        {
//            editor.lineEdit->setReadOnly(true);
            editor.toolButton->setEnabled(false);
        }

        updateEditor();

        QObject::connect(  editor.toolButton, &QToolButton::clicked
                         , this, &PropertyQColorLineEditBttnHandler::onToolButtonClicked);
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

static bool regQColorDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyQColorBase::staticMetaObject
                                , &createDelegate<PropertyDelegateQColor, PropertyQColorBase>
                                , "LineEditBttn");

PropertyDelegateQColor::PropertyDelegateQColor(PropertyQColorBase& owner)
    : PropertyDelegateTyped<PropertyQColorBase>(owner),
      m_shape(QColorDelegateShapeSquare)
{
}

void PropertyDelegateQColor::applyAttributesImpl(const PropertyDelegateAttributes& attributes)
{
    getAttribute(attributes, "shape", m_shape);
}

void PropertyDelegateQColor::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    QColor value = owner().value();

    QRect textRect = rect;

    if (m_shape != QColorDelegateShapeNone)
    {
        QRect colorRect = rect;
        colorRect.setRight(colorRect.left() + colorRect.height());
        colorRect.adjust(2, 2, -2, -2);

        if (m_shape == QColorDelegateShapeSquare)
        {
            painter.fillRect(colorRect, Qt::black);
            colorRect.adjust(1, 1, -1, -1);
            painter.fillRect(colorRect, value);
        }
        else if (m_shape == QColorDelegateShapeCircle)
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
        PropertyDelegateTyped<PropertyQColorBase>::drawValueImpl(painter, textRect, state, needTooltip);
    }
}

QWidget* PropertyDelegateQColor::createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo)
{
    LineEditBttn* editor = new LineEditBttn(parent);
    editor->setGeometry(rect);

    new PropertyQColorLineEditBttnHandler(owner(), *editor);

    if (inplaceInfo)
    {
        editor->lineEdit->selectAll();
    }

    return editor;
}

bool PropertyDelegateQColor::propertyValueToStr(QString& strValue) const
{
    strValue = owner().value().name();
    return true;
}

} // end namespace Qtinuum

