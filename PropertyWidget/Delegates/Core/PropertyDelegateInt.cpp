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
