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

#include "PropertyDelegateUInt.h"
#include "../../../Core/Core/PropertyUInt.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"
#include "../Utils/PropertyDelegateSliderBox.h"

#include <QSpinBox>

void regUIntDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyUIntBase::staticMetaObject
            , &qtnCreateDelegate<QtnPropertyDelegateUInt, QtnPropertyUIntBase>
            , "SpinBox");


    factory.registerDelegate(&QtnPropertyUIntBase::staticMetaObject
            , &qtnCreateDelegate<QtnPropertyDelegateSlideBoxTyped<QtnPropertyUIntBase>, QtnPropertyUIntBase>
            , "SliderBox");
}


class QtnPropertyUIntSpinBoxHandler: public QtnPropertyEditorHandler<QtnPropertyUIntBase, QtnSpinBoxUnsigned>
{
public:
    QtnPropertyUIntSpinBoxHandler(QtnPropertyUIntBase& property, QtnSpinBoxUnsigned& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setUintRange(property.minValue(), property.maxValue());
        editor.setUintSingleStep(property.stepValue());

        updateEditor();

        QObject::connect(  &editor, &QtnSpinBoxUnsigned::uintValueChanged
                         , this, &QtnPropertyUIntSpinBoxHandler::onUintValueChanged);
    }

private:
    void updateEditor() override
    {
        editor().setUintValue(property().value());
    }

    void onUintValueChanged(quint32 value)
    {
        property() = value;
    }
};

QWidget* QtnPropertyDelegateUInt::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    auto spinBox = new QtnSpinBoxUnsigned(parent);
    spinBox->setGeometry(rect);

    new QtnPropertyUIntSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool QtnPropertyDelegateUInt::propertyValueToStrImpl(QString& strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}
