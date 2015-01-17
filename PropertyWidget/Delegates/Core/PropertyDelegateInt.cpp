/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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

#include "PropertyDelegateInt.h"
#include "../../../Core/Core/PropertyInt.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QSpinBox>

class QtnPropertyIntSpinBoxHandler: public QtnPropertyEditorHandler<QtnPropertyIntBase, QSpinBox>
{
public:
    QtnPropertyIntSpinBoxHandler(QtnPropertyIntBase& property, QSpinBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(property.minValue(), property.maxValue());
        editor.setSingleStep(property.stepValue());

        updateEditor();

        QObject::connect(  &editor, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged)
                         , this, &QtnPropertyIntSpinBoxHandler::onValueChanged);
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

static bool regIntDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyIntBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateInt, QtnPropertyIntBase>
                                , "SpinBox");

QWidget* QtnPropertyDelegateInt::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QSpinBox* spinBox = new QSpinBox(parent);
    spinBox->setGeometry(rect);

    new QtnPropertyIntSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool QtnPropertyDelegateInt::propertyValueToStr(QString& strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}
