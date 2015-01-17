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

#include "PropertyDelegateDouble.h"
#include "../../../Core/Core/PropertyDouble.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QDoubleSpinBox>

class QtnPropertyDoubleSpinBoxHandler: public QtnPropertyEditorHandler<QtnPropertyDoubleBase, QDoubleSpinBox>
{
public:
    QtnPropertyDoubleSpinBoxHandler(QtnPropertyDoubleBase& property, QDoubleSpinBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(property.minValue(), property.maxValue());
        editor.setSingleStep(property.stepValue());
        editor.setDecimals(12);

        updateEditor();

        QObject::connect(  &editor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
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

static bool regDoubleDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyDoubleBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateDouble, QtnPropertyDoubleBase>
                                , "SpinBox");

QWidget* QtnPropertyDelegateDouble::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
    spinBox->setGeometry(rect);

    new QtnPropertyDoubleSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool QtnPropertyDelegateDouble::propertyValueToStr(QString& strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}
