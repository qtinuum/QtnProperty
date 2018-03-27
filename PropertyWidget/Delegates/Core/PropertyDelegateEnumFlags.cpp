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

#include "PropertyDelegateEnumFlags.h"
#include "../../../Core/Core/PropertyEnumFlags.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"

#include "../../../Core/Core/PropertyBool.h"
#include <QLineEdit>

void regEnumFlagsDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyEnumFlagsBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateEnumFlags, QtnPropertyEnumFlagsBase>
                 , "FlagsList");
}

static QString enumFlagsProperty2Str(const QtnPropertyEnumFlagsBase& property)
{
    QString text;

    const QtnEnumInfo *enumInfo = property.enumInfo();
    if (!enumInfo)
        return text;

    QtnEnumFlagsValueType value = property.value();
    if (value == 0)
        return text;

    enumInfo->forEachEnumValue([&text, value](const QtnEnumValueInfo& e)->bool {
        if (value & e.value())
        {
            if (!text.isEmpty()) text += "|";
            text += e.displayName();
        }

        return true;
    });

    return text;
}

class QtnPropertyEnumFlagsLineEditHandler: public QtnPropertyEditorHandler<QtnPropertyEnumFlagsBase, QLineEdit>
{
public:
    QtnPropertyEnumFlagsLineEditHandler(QtnPropertyEnumFlagsBase& property, QLineEdit& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
      // if (!property.isEditableByUser())
      editor.setReadOnly(true);

      updateEditor();
    }

private:
    void updateEditor() override
    {
        editor().setText(enumFlagsProperty2Str(property()));
    }
};

QtnPropertyDelegateEnumFlags::QtnPropertyDelegateEnumFlags(QtnPropertyEnumFlagsBase& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyEnumFlagsBase>(owner)
{
    const QtnEnumInfo *enumInfo = owner.enumInfo();
    if (enumInfo)
    {
        QtnEnumFlagsValueType value= owner;
        enumInfo->forEachEnumValue([this, &owner, value](const QtnEnumValueInfo& e)->bool {
            if (e.state() == QtnEnumValueStateNone)
            {
                QtnEnumValueType enum_value = e.value();
                QtnPropertyEnumFlagsBase& _owner = owner;

                QtnPropertyBoolCallback *flagProperty = new QtnPropertyBoolCallback(0);
                flagProperty->setName(e.displayName());
                flagProperty->setDescription(owner.tr("%1 flag for %2.").arg(e.displayName(), owner.name()));
                flagProperty->setCallbackValueGet([&_owner, enum_value]()->bool {
                    return _owner.value() & enum_value;
                });
                flagProperty->setCallbackValueSet([&_owner, enum_value](bool value) {
                    if (value)
                        _owner.setValue(_owner.value() | enum_value);
                    else
                        _owner.setValue(_owner.value() & ~enum_value);
                });

                this->addSubProperty(flagProperty);
            }

            return true;
        });
    }
}

QWidget* QtnPropertyDelegateEnumFlags::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setGeometry(rect);

    new QtnPropertyEnumFlagsLineEditHandler(owner(), *lineEdit);

    if (inplaceInfo)
    {
        lineEdit->selectAll();
    }

    return lineEdit;
}

bool QtnPropertyDelegateEnumFlags::propertyValueToStrImpl(QString& strValue) const
{
    strValue = enumFlagsProperty2Str(owner());
    return true;
}
