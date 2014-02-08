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

#include "PropertyDelegateBool.h"

#include "../../../Core/Core/PropertyBool.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include <QStyleOption>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

class QtnPropertyBoolCheckBoxHandler: public QtnPropertyEditorHandler<QtnPropertyBoolBase, QCheckBox>
{
public:
    QtnPropertyBoolCheckBoxHandler(QtnPropertyBoolBase& property, QCheckBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(&editor, &QCheckBox::toggled, this, &QtnPropertyBoolCheckBoxHandler::onToggled);
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

class QtnPropertyBoolComboBoxHandler: public QtnPropertyEditorHandler<QtnPropertyBoolBase, QComboBox>
{
public:
    QtnPropertyBoolComboBoxHandler(QtnPropertyBoolBase& property, QComboBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(  &editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                         , this, &QtnPropertyBoolComboBoxHandler::onCurrentIndexChanged);
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

static bool regBoolDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyBoolBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateBoolCheck, QtnPropertyBoolBase>
                                , "CheckBox");

static bool regBoolDelegateCombobox = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegate(&QtnPropertyBoolBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateBoolCombobox, QtnPropertyBoolBase>
                                , "ComboBox");

QCheckBox* createPropertyBoolCheckBox(QtnPropertyBoolBase& owner, QWidget* parent, const QRect& rect)
{
    QCheckBox *checkBox = new QCheckBoxPropertyBool(parent);
    checkBox->setGeometry(rect);

    // connect widget and property
    new QtnPropertyBoolCheckBoxHandler(owner, *checkBox);

    return checkBox;
}

void QtnPropertyDelegateBoolCheck::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    QStyleOptionButton opt;
    opt.rect = rect;
    opt.state = state;

    bool value = owner().value();
    if (value)
        opt.state |= QStyle::State_On;

    painter.drawControl(QStyle::CE_CheckBox, opt);
}

QWidget* QtnPropertyDelegateBoolCheck::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    if (!owner().isEditableByUser())
        return 0;

    QCheckBox *checkBox = createPropertyBoolCheckBox(owner(), parent, rect);

    if (inplaceInfo)
        checkBox->setAutoFillBackground(true);

    return checkBox;
}

QtnPropertyDelegateBoolCombobox::QtnPropertyDelegateBoolCombobox(QtnPropertyBoolBase& owner)
    : QtnPropertyDelegateTyped<QtnPropertyBoolBase>(owner)
{
    static QString labels[2] = { owner.tr("False"), owner.tr("True")};

    m_labels[0] = labels[0];
    m_labels[1] = labels[1];
}

void QtnPropertyDelegateBoolCombobox::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    qtnGetAttribute(attributes, "labelFalse", m_labels[0]);
    qtnGetAttribute(attributes, "labelTrue", m_labels[1]);
}

QWidget* QtnPropertyDelegateBoolCombobox::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    if (owner().isEditableByUser())
    {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->addItem(m_labels[true], true);
        comboBox->addItem(m_labels[false], false);

        comboBox->setGeometry(rect);

        // connect widget and property
        new QtnPropertyBoolComboBoxHandler(owner(), *comboBox);

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

bool QtnPropertyDelegateBoolCombobox::propertyValueToStr(QString& strValue) const
{
    strValue = m_labels[owner().value()];
    return true;
}
