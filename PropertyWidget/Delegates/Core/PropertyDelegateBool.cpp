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

#include "PropertyDelegateBool.h"

#include "../../../Core/Core/PropertyBool.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QStyleOption>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

namespace Qtinuum
{

class PropertyBoolCheckBoxHandler: public PropertyEditorHandler<PropertyBoolBase, QCheckBox>
{
public:
    PropertyBoolCheckBoxHandler(PropertyBoolBase& property, QCheckBox& editor)
        : PropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(&editor, &QCheckBox::toggled, this, &PropertyBoolCheckBoxHandler::onToggled);
    }

private:
    void updateEditor() override
    {
        editor().setChecked(property());
    }

    void onToggled(bool checked)
    {
        property() = checked;
    }
};

class QCheckBoxPropertyBool: public QCheckBox
{
public:
    explicit QCheckBoxPropertyBool(QWidget *parent=0)
        : QCheckBox(parent)
    {
    }

    explicit QCheckBoxPropertyBool(const QString& text, QWidget* parent=0)
        : QCheckBox(text, parent)
    {
    }

protected:
    bool hitButton(const QPoint& pos) const override
    {
        return QAbstractButton::hitButton(pos);
    }
};

class PropertyBoolComboBoxHandler: public PropertyEditorHandler<PropertyBoolBase, QComboBox>
{
public:
    PropertyBoolComboBoxHandler(PropertyBoolBase& property, QComboBox& editor)
        : PropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(  &editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                         , this, &PropertyBoolComboBoxHandler::onCurrentIndexChanged);
    }

private:
    void updateEditor() override
    {
        int index = editor().findData((bool)property());
        Q_ASSERT(index >= 0);
        editor().setCurrentIndex(index);
    }

    void onCurrentIndexChanged(int index)
    {
        QVariant data = editor().itemData(index);
        if (data.canConvert<bool>())
            property() = data.toBool();
    }
};

static bool regBoolDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyBoolBase::staticMetaObject
                                , &createDelegate<PropertyDelegateBoolCheck, PropertyBoolBase>
                                , "CheckBox");

static bool regBoolDelegateLabels = PropertyDelegateFactory::staticInstance()
                                .registerDelegate(&PropertyBoolBase::staticMetaObject
                                , &createDelegate<PropertyDelegateBoolLabels, PropertyBoolBase>
                                , "ComboBox");

QCheckBox *createPropertyBoolCheckBox(PropertyBoolBase& owner, QWidget* parent, const QRect& rect)
{
    QCheckBox *checkBox = new QCheckBoxPropertyBool(parent);
    checkBox->setGeometry(rect);

    // connect widget and property
    new PropertyBoolCheckBoxHandler(owner, *checkBox);

    return checkBox;
}

void PropertyDelegateBoolCheck::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    QStyleOptionButton opt;
    opt.rect = rect;
    opt.state = state;

    bool value = owner().value();
    if (value)
        opt.state |= QStyle::State_On;

    painter.drawControl(QStyle::CE_CheckBox, opt);
}

QWidget* PropertyDelegateBoolCheck::createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo)
{
    if (!owner().isEditableByUser())
        return 0;

    QCheckBox *checkBox = createPropertyBoolCheckBox(owner(), parent, rect);

    if (inplaceInfo)
        checkBox->setAutoFillBackground(true);

    return checkBox;
}

PropertyDelegateBoolLabels::PropertyDelegateBoolLabels(PropertyBoolBase& owner)
    : PropertyDelegateTyped<PropertyBoolBase>(owner)
{
    static QString labels[2] = { owner.tr("False"), owner.tr("True")};

    m_labels[0] = labels[0];
    m_labels[1] = labels[1];
}

void PropertyDelegateBoolLabels::applyAttributesImpl(const PropertyDelegateAttributes& attributes)
{
    getAttribute(attributes, "labelFalse", m_labels[0]);
    getAttribute(attributes, "labelTrue", m_labels[1]);
}

QWidget* PropertyDelegateBoolLabels::createValueEditorImpl(QWidget* parent, const QRect& rect, InplaceInfo* inplaceInfo)
{
    if (owner().isEditableByUser())
    {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->addItem(m_labels[true], true);
        comboBox->addItem(m_labels[false], false);

        comboBox->setGeometry(rect);

        // connect widget and property
        new PropertyBoolComboBoxHandler(owner(), *comboBox);

        if (inplaceInfo)
            comboBox->showPopup();

        return comboBox;
    }
    else
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        lineEdit->setReadOnly(true);
        lineEdit->setText(m_labels[owner()]);

        lineEdit->setGeometry(rect);

        return lineEdit;
    }
}

bool PropertyDelegateBoolLabels::propertyValueToStr(QString& strValue) const
{
    strValue = m_labels[owner().value()];
    return true;
}

} // end namespace Qtinuum

