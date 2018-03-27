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

#include "PropertyDelegateBool.h"

#include "../../../Core/Core/PropertyBool.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"

#include <QDebug>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QMouseEvent>

void regBoolDelegates(QtnPropertyDelegateFactory& factory)
{
    factory.registerDelegateDefault(&QtnPropertyBoolBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateBoolCheck, QtnPropertyBoolBase>
                 , "CheckBox");

    factory.registerDelegate(&QtnPropertyBoolBase::staticMetaObject
              , &qtnCreateDelegate<QtnPropertyDelegateBoolCombobox, QtnPropertyBoolBase>
              , "ComboBox");
}

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

QCheckBox* createPropertyBoolCheckBox(QtnPropertyBoolBase& owner, QWidget* parent, const QRect& rect)
{
    QCheckBox *checkBox = new QCheckBoxPropertyBool(parent);
    checkBox->setGeometry(rect);

    // connect widget and property
    new QtnPropertyBoolCheckBoxHandler(owner, *checkBox);

    return checkBox;
}

bool QtnPropertyDelegateBoolCheck::createSubItemValueImpl(QtnDrawContext& context, QtnSubItem& subItemValue)
{
    subItemValue.trackState();
    subItemValue.rect.setWidth(context.style()->pixelMetric(QStyle::PM_IndicatorWidth));

    subItemValue.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {
        QStyleOptionButton opt;
        opt.rect = item.rect;
        opt.state = state(context.isActive, item.state());

        bool value = owner().value();
        if (value)
            opt.state |= QStyle::State_On;

        context.painter->drawControl(QStyle::CE_CheckBox, opt);
    };

    subItemValue.eventHandler = [this](QtnEventContext& context, const QtnSubItem&) {
        bool toggleValue = false;
        switch (context.eventType())
        {
        case QEvent::MouseButtonRelease:
            toggleValue = true;
            break;

        case QEvent::KeyPress:
        {
            int key = context.eventAs<QKeyEvent>()->key();
            toggleValue = (key == Qt::Key_Space) || (key == Qt::Key_Return);
        }
        break;

        default:
            ;
        }

        if (toggleValue)
            owner().setValue(!owner().value());

        return toggleValue;
    };

    return true;
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

bool QtnPropertyDelegateBoolCombobox::propertyValueToStrImpl(QString& strValue) const
{
    strValue = m_labels[owner().value()];
    return true;
}
