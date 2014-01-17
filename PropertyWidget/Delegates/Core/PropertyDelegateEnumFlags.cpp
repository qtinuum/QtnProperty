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

#include "PropertyDelegateEnumFlags.h"
#include "../../../Core/Core/PropertyEnumFlags.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include "../../../Core/Core/PropertyBool.h"
#include <QLineEdit>

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
            text += e.name();
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
//        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        updateEditor();
    }

private:
    void updateEditor() override
    {
        editor().setText(enumFlagsProperty2Str(property()));
    }
};

static bool regEnumFlagsDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyEnumFlagsBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateEnumFlags, QtnPropertyEnumFlagsBase>
                                , "FlagsList");

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
                flagProperty->setName(e.name());
                flagProperty->setDescription(owner.tr("%1 flag for %2.").arg(e.name(), owner.name()));
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

bool QtnPropertyDelegateEnumFlags::propertyValueToStr(QString& strValue) const
{
    strValue = enumFlagsProperty2Str(owner());
    return true;
}
