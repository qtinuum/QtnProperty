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

#include "PropertyDelegateEnum.h"
#include "../../../Core/Core/PropertyEnum.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"

#include <QComboBox>
#include <QLineEdit>

void regEnumDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyEnumBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateEnum, QtnPropertyEnumBase>
                 , "ComboBox");
}

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

QWidget* QtnPropertyDelegateEnum::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    const QtnEnumInfo* info = owner().enumInfo();

    if (!info)
        return 0;

    if (owner().isEditableByUser())
    {
        QComboBox* combo = new QComboBox(parent);
        info->forEachEnumValue([combo](const QtnEnumValueInfo &value)->bool {
            combo->addItem(value.displayName(), QVariant(value.value()));
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
        lineEdit->setText(valueInfo->displayName());

        lineEdit->setGeometry(rect);

        return lineEdit;
    }

}

bool QtnPropertyDelegateEnum::propertyValueToStrImpl(QString& strValue) const
{
    QtnEnumValueType value = owner().value();
    const QtnEnumInfo* info = owner().enumInfo();
    const QtnEnumValueInfo* valueInfo = info ? info->findByValue(value) : 0;

    if (!valueInfo)
        return false;

    strValue = valueInfo->displayName();
    return true;
}
