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

#include "PropertyDelegateFloat.h"
#include "../../../Core/Core/PropertyFloat.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"
#include "../Utils/PropertyDelegateSliderBox.h"

#include <QKeyEvent>

void regFloatDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyFloatBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateFloat, QtnPropertyFloatBase>
                                , "SpinBox");

    factory.registerDelegate(&QtnPropertyFloatBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateSlideBoxTyped<QtnPropertyFloatBase>, QtnPropertyFloatBase>
                                , "SliderBox");
}

class QtnPropertyFloatSpinBoxHandler: public QtnPropertyEditorHandler<QtnPropertyFloatBase, QDoubleSpinBox>
{
public:
    QtnPropertyFloatSpinBoxHandler(QtnPropertyFloatBase& property, QDoubleSpinBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(property.minValue(), property.maxValue());
        editor.setSingleStep(property.stepValue());
        editor.setDecimals(6);

        updateEditor();

        QObject::connect(  &editor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
                         , this, &QtnPropertyFloatSpinBoxHandler::onValueChanged);
    }

private:
    void updateEditor() override
    {
        float editorValue = (float)editor().value();
        float propertyValue = (float)property();
        // update editor if property value differs from editor value
        if (editorValue != propertyValue)
            editor().setValue(propertyValue);
    }

    void onValueChanged(double value)
    {
        property() = (float)value;
    }
};


QWidget* QtnPropertyDelegateFloat::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QDoubleSpinBox *spinBox = new QtnDoubleSpinBox(parent);
    spinBox->setGeometry(rect);

    new QtnPropertyFloatSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool QtnPropertyDelegateFloat::propertyValueToStrImpl(QString& strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}
