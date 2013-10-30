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

#include "PropertyDelegateFloat.h"
#include "../../../Core/Core/PropertyFloat.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QDoubleSpinBox>

namespace Qtinuum
{

class PropertyFloatSpinBoxHandler: public PropertyEditorHandler<PropertyFloatBase, QDoubleSpinBox>
{
public:
    PropertyFloatSpinBoxHandler(PropertyFloatBase& property, QDoubleSpinBox& editor)
        : PropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(property.minValue(), property.maxValue());
        editor.setSingleStep(property.stepValue());
        editor.setDecimals(6);

        updateEditor();

        QObject::connect(  &editor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
                         , this, &PropertyFloatSpinBoxHandler::onValueChanged);
    }

private:
    void updateEditor() override
    {
        editor().setValue((float)property());
    }

    void onValueChanged(double value)
    {
        property() = (float)value;
    }
};

static bool regFloatDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyFloatBase::staticMetaObject
                                , &createDelegate<PropertyDelegateFloat, PropertyFloatBase>
                                , "SpinBox");

QWidget* PropertyDelegateFloat::createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo)
{
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(parent);
    spinBox->setGeometry(rect);

    new PropertyFloatSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool PropertyDelegateFloat::propertyValueToStr(QString& strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}

} // end namespace Qtinuum

