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

#include "PropertyDelegateEnum.h"
#include "../../../Core/Core/PropertyEnum.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QComboBox>
#include <QLineEdit>

namespace Qtinuum
{

class PropertyEnumComboBoxHandler: public PropertyEditorHandler<PropertyEnumBase, QComboBox>
{
public:
    PropertyEnumComboBoxHandler(PropertyEnumBase& property, QComboBox& editor)
        : PropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(  &editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                         , this, &PropertyEnumComboBoxHandler::onCurrentIndexChanged);
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
            property() = data.toInt();
    }
};


static bool regEnumDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyEnumBase::staticMetaObject
                                , &createDelegate<PropertyDelegateEnum, PropertyEnumBase>
                                , "ComboBox");

QWidget* PropertyDelegateEnum::createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo)
{
    const EnumInfo* info = owner().enumInfo();

    if (!info)
        return 0;

    if (owner().isEditableByUser())
    {
        QComboBox* combo = new QComboBox(parent);
        info->forEachEnumValue([combo](const EnumValueInfo &value)->bool {
            combo->addItem(value.name(), QVariant(value.value()));
            return true;
        });

        combo->setGeometry(rect);

        new PropertyEnumComboBoxHandler(owner(), *combo);

        if (inplaceInfo)
            combo->showPopup();

        return combo;
    }
    else
    {
        const EnumValueInfo* valueInfo = info->findByValue(owner());
        if (!valueInfo)
            return 0;

        QLineEdit* lineEdit = new QLineEdit(parent);
        lineEdit->setReadOnly(true);
        lineEdit->setText(valueInfo->name());

        lineEdit->setGeometry(rect);

        return lineEdit;
    }

}

bool PropertyDelegateEnum::propertyValueToStr(QString& strValue) const
{
    EnumValueType value = owner().value();
    const EnumInfo* info = owner().enumInfo();
    const EnumValueInfo* valueInfo = info ? info->findByValue(value) : 0;

    if (!valueInfo)
        return false;

    strValue = valueInfo->name();
    return true;
}

} // end namespace Qtinuum

