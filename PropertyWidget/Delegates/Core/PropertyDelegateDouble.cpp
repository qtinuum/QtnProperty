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

#include "PropertyDelegateDouble.h"
#include "../../../Core/Core/PropertyDouble.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QDoubleSpinBox>

namespace Qtinuum
{

class PropertyDoubleSpinBoxHandler: public PropertyEditorHandler<PropertyDoubleBase, QDoubleSpinBox>
{
public:
    PropertyDoubleSpinBoxHandler(PropertyDoubleBase& property, QDoubleSpinBox& editor)
        : PropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(property.minValue(), property.maxValue());
        editor.setSingleStep(property.stepValue());
        editor.setDecimals(12);

        updateEditor();

        QObject::connect(  &editor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
                         , this, &PropertyDoubleSpinBoxHandler::onValueChanged);
    }

private:
    void updateEditor() override
    {
        editor().setValue(property());
    }

    void onValueChanged(double value)
    {
        property() = value;
    }
};

static bool regDoubleDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyDoubleBase::staticMetaObject
                                , &createDelegate<PropertyDelegateDouble, PropertyDoubleBase>
                                , "SpinBox");

QWidget* PropertyDelegateDouble::createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo)
{
    QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
    spinBox->setGeometry(rect);

    new PropertyDoubleSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool PropertyDelegateDouble::propertyValueToStr(QString& strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}

} // end namespace Qtinuum

