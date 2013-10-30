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

#include "PropertyDelegateInt.h"
#include "../../../Core/Core/PropertyInt.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QSpinBox>

namespace Qtinuum
{

class PropertyIntSpinBoxHandler: public PropertyEditorHandler<PropertyIntBase, QSpinBox>
{
public:
    PropertyIntSpinBoxHandler(PropertyIntBase& property, QSpinBox& editor)
        : PropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(property.minValue(), property.maxValue());
        editor.setSingleStep(property.stepValue());

        updateEditor();

        QObject::connect(  &editor, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged)
                         , this, &PropertyIntSpinBoxHandler::onValueChanged);
    }

private:
    void updateEditor() override
    {
        editor().setValue(property());
    }

    void onValueChanged(int value)
    {
        property() = value;
    }
};

static bool regIntDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyIntBase::staticMetaObject
                                , &createDelegate<PropertyDelegateInt, PropertyIntBase>
                                , "SpinBox");

QWidget* PropertyDelegateInt::createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo)
{
    QSpinBox* spinBox = new QSpinBox(parent);
    spinBox->setGeometry(rect);

    new PropertyIntSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool PropertyDelegateInt::propertyValueToStr(QString& strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}

} // end namespace Qtinuum

