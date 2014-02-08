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

#include "PropertyDelegateEnum.h"
#include "../../../Core/Core/PropertyEnum.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QComboBox>
#include <QLineEdit>

class QtnPropertyEnumComboBoxHandler: public QtnPropertyEditorHandler<QtnPropertyEnumBase, QComboBox>
{
public:
    QtnPropertyEnumComboBoxHandler(QtnPropertyEnumBase& property, QComboBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(  &editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                         , this, &QtnPropertyEnumComboBoxHandler::onCurrentIndexChanged);
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


static bool regEnumDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyEnumBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateEnum, QtnPropertyEnumBase>
                                , "ComboBox");

QWidget* QtnPropertyDelegateEnum::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    const QtnEnumInfo* info = owner().enumInfo();

    if (!info)
        return 0;

    if (owner().isEditableByUser())
    {
        QComboBox* combo = new QComboBox(parent);
        info->forEachEnumValue([combo](const QtnEnumValueInfo &value)->bool {
            combo->addItem(value.name(), QVariant(value.value()));
            return true;
        });

        combo->setGeometry(rect);

        new QtnPropertyEnumComboBoxHandler(owner(), *combo);

        if (inplaceInfo)
            combo->showPopup();

        return combo;
    }
    else
    {
        const QtnEnumValueInfo* valueInfo = info->findByValue(owner());
        if (!valueInfo)
            return 0;

        QLineEdit* lineEdit = new QLineEdit(parent);
        lineEdit->setReadOnly(true);
        lineEdit->setText(valueInfo->name());

        lineEdit->setGeometry(rect);

        return lineEdit;
    }

}

bool QtnPropertyDelegateEnum::propertyValueToStr(QString& strValue) const
{
    QtnEnumValueType value = owner().value();
    const QtnEnumInfo* info = owner().enumInfo();
    const QtnEnumValueInfo* valueInfo = info ? info->findByValue(value) : 0;

    if (!valueInfo)
        return false;

    strValue = valueInfo->name();
    return true;
}
