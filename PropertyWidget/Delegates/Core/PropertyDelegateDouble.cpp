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

#include "PropertyDelegateDouble.h"
#include "../../../Core/Core/PropertyDouble.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"
#include "../Utils/PropertyDelegateSliderBox.h"

#include <QDoubleSpinBox>


void regDoubleDelegates()
{
  QtnPropertyDelegateFactory::staticInstance()
    .registerDelegateDefault(&QtnPropertyDoubleBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateDouble, QtnPropertyDoubleBase>
                 , "SpinBox");

  QtnPropertyDelegateFactory::staticInstance()
      .registerDelegate(&QtnPropertyDoubleBase::staticMetaObject
                  , &qtnCreateDelegate<QtnPropertyDelegateSlideBoxTyped<QtnPropertyDoubleBase>, QtnPropertyDoubleBase>
                  , "SliderBox");
}

class QtnPropertyDoubleSpinBoxHandler: public QtnPropertyEditorHandler<QtnPropertyDoubleBase, CustomeDoubleSpinBox>
{
public:
    QtnPropertyDoubleSpinBoxHandler(QtnPropertyDoubleBase& property, CustomeDoubleSpinBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(property.minValue(), property.maxValue());
        editor.setSingleStep(property.stepValue());
        editor.setDecimals(12);

        updateEditor();

        QObject::connect(  &editor, static_cast<void (CustomeDoubleSpinBox::*)(double)>(&CustomeDoubleSpinBox::valueChanged)
                         , this, &QtnPropertyDoubleSpinBoxHandler::onValueChanged);
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

QWidget* QtnPropertyDelegateDouble::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    CustomeDoubleSpinBox* spinBox = new CustomeDoubleSpinBox(parent);
    spinBox->setGeometry(rect);

    new QtnPropertyDoubleSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool QtnPropertyDelegateDouble::propertyValueToStrImpl(QString& strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}
