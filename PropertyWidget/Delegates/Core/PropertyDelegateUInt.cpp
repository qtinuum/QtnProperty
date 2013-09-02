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

#include "PropertyDelegateUInt.h"
#include "../../../Core/Core/PropertyUInt.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QSpinBox>

namespace Qtinuum
{

const quint32 u_2 = std::numeric_limits<quint32>::max() / 2 + 1;
static qint32 u2i(quint32 val)
{
    return qint32(val - u_2);
}
static quint32 i2u(qint32 val)
{
    return (quint32)val + u_2;
}

class PropertyUIntSpinBoxHandler: public PropertyEditorHandler<PropertyUIntBase, QSpinBox>
{
public:
    PropertyUIntSpinBoxHandler(PropertyUIntBase &property, QSpinBox &editor)
        : PropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(u2i(property.minValue()), u2i(property.maxValue()));
        editor.setSingleStep(u2i(property.stepValue()));

        updateEditor();

        QObject::connect(  &editor, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged)
                         , this, &PropertyUIntSpinBoxHandler::onValueChanged);
    }

private:
    void updateEditor() override
    {
        editor().setValue(u2i(property().value()));
    }

    void onValueChanged(int value)
    {
        property() = i2u(value);
    }
};

class SpinBoxUnsigned: public QSpinBox
{
public:
    SpinBoxUnsigned(QWidget *parent)
        : QSpinBox(parent)
    {
    }

protected:
    int valueFromText(const QString &text) const override
    {
        return u2i(locale().toUInt(text));
    }

    QString textFromValue(int val) const override
    {
        return locale().toString(i2u(val));
    }
};

static bool regUIntDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyUIntBase::staticMetaObject
                                , &createDelegate<PropertyDelegateUInt, PropertyUIntBase>
                                , "SpinBox");

QWidget *PropertyDelegateUInt::createValueEditorImpl(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo)
{
    QSpinBox *spinBox = new SpinBoxUnsigned(parent);
    spinBox->setGeometry(rect);

    new PropertyUIntSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool PropertyDelegateUInt::propertyValueToStr(QString &strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}


} // end namespace Qtinuum

