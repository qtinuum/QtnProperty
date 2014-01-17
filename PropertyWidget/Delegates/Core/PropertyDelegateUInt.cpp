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

const quint32 qtn_u_2 = std::numeric_limits<quint32>::max() / 2 + 1;
static qint32 qtn_u2i(quint32 val)
{
    return qint32(val - qtn_u_2);
}
static quint32 qtn_i2u(qint32 val)
{
    return (quint32)val + qtn_u_2;
}

class QtnPropertyUIntSpinBoxHandler: public QtnPropertyEditorHandler<QtnPropertyUIntBase, QSpinBox>
{
public:
    QtnPropertyUIntSpinBoxHandler(QtnPropertyUIntBase& property, QSpinBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(qtn_u2i(property.minValue()), qtn_u2i(property.maxValue()));
        editor.setSingleStep(qtn_u2i(property.stepValue()));

        updateEditor();

        QObject::connect(  &editor, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged)
                         , this, &QtnPropertyUIntSpinBoxHandler::onValueChanged);
    }

private:
    void updateEditor() override
    {
        editor().setValue(qtn_u2i(property().value()));
    }

    void onValueChanged(int value)
    {
        property() = qtn_i2u(value);
    }
};

class SpinBoxUnsigned: public QSpinBox
{
public:
    SpinBoxUnsigned(QWidget* parent)
        : QSpinBox(parent)
    {
    }

protected:
    int valueFromText(const QString& text) const override
    {
        return qtn_u2i(locale().toUInt(text));
    }

    QString textFromValue(int val) const override
    {
        return locale().toString(qtn_i2u(val));
    }
};

static bool regUIntDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyUIntBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateUInt, QtnPropertyUIntBase>
                                , "SpinBox");

QWidget* QtnPropertyDelegateUInt::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QSpinBox* spinBox = new SpinBoxUnsigned(parent);
    spinBox->setGeometry(rect);

    new QtnPropertyUIntSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool QtnPropertyDelegateUInt::propertyValueToStr(QString& strValue) const
{
    strValue = QString::number(owner().value());
    return true;
}
