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

#include "PropertyDelegateQString.h"
#include "../../../Core/Core/PropertyQString.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"
#include "../PropertyEditorAux.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QFileInfo>
#include <QFileDialog>
#include <QComboBox>
#include <QDebug>

namespace Qtinuum
{

static bool regQStringDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyQStringBase::staticMetaObject
                                , &createDelegate<PropertyDelegateQString, PropertyQStringBase>
                                , "LineEdit");

static bool regQStringFileDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegate(&PropertyQStringBase::staticMetaObject
                                , &createDelegate<PropertyDelegateQStringFile, PropertyQStringBase>
                                , "File");

static bool regQStringListDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegate(&PropertyQStringBase::staticMetaObject
                                , &createDelegate<PropertyDelegateQStringList, PropertyQStringBase>
                                , "List");

class PropertyQStringLineEditHandler: public PropertyEditorHandler<PropertyQStringBase, QLineEdit>
{
public:
    PropertyQStringLineEditHandler(PropertyQStringBase &property, QLineEdit &editor)
        : PropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.installEventFilter(this);
        QObject::connect(  &editor, &QLineEdit::editingFinished
                         , this, &PropertyQStringLineEditHandler::onEditingFinished);
    }

    ~PropertyQStringLineEditHandler()
    {
    }

private:
    void updateEditor() override
    {
        editor().setText(property());
    }

    void onEditingFinished()
    {
        qDebug() << "onEditingFinished with text: " << editor().text();
        property() = editor().text();
    }

    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            // revert all changes
            if (keyEvent->key() == Qt::Key_Escape)
                updateEditor();
        }

        return QObject::eventFilter(obj, event);
    }
};

PropertyDelegateQString::PropertyDelegateQString(PropertyQStringBase &owner)
    : PropertyDelegateTyped<PropertyQStringBase>(owner)
{
}

bool PropertyDelegateQString::acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const
{
    if (PropertyDelegateTyped<PropertyQStringBase>::acceptKeyPressedForInplaceEditImpl(keyEvent))
        return true;

    // accept any printable key
    return acceptForLineEdit(keyEvent);
}

QWidget *PropertyDelegateQString::createValueEditorImpl(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo)
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setGeometry(rect);

    new PropertyQStringLineEditHandler(owner(), *lineEdit);

    initLineEdit(lineEdit, inplaceInfo);

    return lineEdit;
}

bool PropertyDelegateQString::propertyValueToStr(QString &strValue) const
{
    strValue = owner().value();
    return true;
}

class PropertyQStringFileLineEditBttnHandler: public PropertyEditorHandler<PropertyQStringBase, LineEditBttn>
{
public:
    PropertyQStringFileLineEditBttnHandler(PropertyQStringBase &property, LineEditBttn &editor)
        : PropertyEditorHandlerType(property, editor),
          m_dlg(&editor)
    {
        //editor.lineEdit->setReadOnly(true);

        if (!property.isEditableByUser())
        {
            editor.lineEdit->setReadOnly(true);
            editor.toolButton->setEnabled(false);
        }

        updateEditor();

        editor.installEventFilter(this);
        QObject::connect(  editor.toolButton, &QToolButton::clicked
                         , this, &PropertyQStringFileLineEditBttnHandler::onToolButtonClicked);
        QObject::connect(  editor.lineEdit, &QLineEdit::editingFinished
                         , this, &PropertyQStringFileLineEditBttnHandler::onEditingFinished);

    }

    void applyAttributes(const PropertyDelegateAttributes &attributes)
    {
        int option = 0;
        if (getAttribute(attributes, "acceptMode", option))
            m_dlg.setAcceptMode(QFileDialog::AcceptMode(option));

        QString str;
        if (getAttribute(attributes, "defaultSuffix", str))
            m_dlg.setDefaultSuffix(str);

        if (getAttribute(attributes, "fileMode", option))
            m_dlg.setFileMode(QFileDialog::FileMode(option));

        if (getAttribute(attributes, "options", option))
            m_dlg.setOptions(QFileDialog::Options(QFlag(option)));

        if (getAttribute(attributes, "viewMode", option))
            m_dlg.setViewMode(QFileDialog::ViewMode(option));

        if (getAttribute(attributes, "nameFilter", str))
            m_dlg.setNameFilter(str);

        QStringList list;
        if (getAttribute(attributes, "nameFilters", list))
            m_dlg.setNameFilters(list);
    }

private:
    void updateEditor() override
    {
        editor().lineEdit->setText(property().value());
    }

    void onToolButtonClicked(bool checked)
    {
        m_dlg.selectFile(property().value());
        if (m_dlg.exec() == QDialog::Accepted)
        {
            QStringList files = m_dlg.selectedFiles();
            if (files.size() == 1)
                property() = files.first();
        }
    }

    void onEditingFinished()
    {
        property() = editor().lineEdit->text();
    }

    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            // revert all changes
            if (keyEvent->key() == Qt::Key_Escape)
                updateEditor();
        }

        return QObject::eventFilter(obj, event);
    }

    QFileDialog m_dlg;
};

PropertyDelegateQStringInvalidBase::PropertyDelegateQStringInvalidBase(PropertyQStringBase &owner)
    : PropertyDelegateQString(owner),
      m_invalidColor(Qt::red)
{
}

void PropertyDelegateQStringInvalidBase::applyAttributesImpl(const PropertyDelegateAttributes &attributes)
{
    getAttribute(attributes, "invalidColor", m_invalidColor);
}

void PropertyDelegateQStringInvalidBase::drawValueImpl(QStylePainter &painter, const QRect &rect, const QStyle::State &state, bool *needTooltip) const
{
    if ((m_invalidColor.alpha() != 0) && !isPropertyValid())
    {
        QPen oldPen = painter.pen();
        painter.setPen(m_invalidColor);
        PropertyDelegateQString::drawValueImpl(painter, rect, state, needTooltip);
        painter.setPen(oldPen);
    }
    else
    {
        PropertyDelegateQString::drawValueImpl(painter, rect, state, needTooltip);
    }
}


PropertyDelegateQStringFile::PropertyDelegateQStringFile(PropertyQStringBase &owner)
    : PropertyDelegateQStringInvalidBase(owner)
{
}

void PropertyDelegateQStringFile::applyAttributesImpl(const PropertyDelegateAttributes &attributes)
{
    PropertyDelegateQStringInvalidBase::applyAttributesImpl(attributes);
    m_editorAttributes = attributes;
}

QWidget *PropertyDelegateQStringFile::createValueEditorImpl(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo)
{
    LineEditBttn *editor = new LineEditBttn(parent);
    editor->setGeometry(rect);

    auto handler = new PropertyQStringFileLineEditBttnHandler(owner(), *editor);
    handler->applyAttributes(m_editorAttributes);

    initLineEdit(editor->lineEdit, inplaceInfo);

    return editor;
}

bool PropertyDelegateQStringFile::isPropertyValid() const
{
    return QFileInfo(owner().value()).exists();
}

class PropertyQStringListComboBoxHandler: public PropertyEditorHandler<PropertyQStringBase, QComboBox>
{
public:
    PropertyQStringListComboBoxHandler(PropertyQStringBase &property, QComboBox &editor)
        : PropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(  &editor, &QComboBox::currentTextChanged
                         , this, &PropertyQStringListComboBoxHandler::onCurrentTextChanged);
    }

private:
    void updateEditor() override
    {
        editor().setCurrentText(property());
    }

    void onCurrentTextChanged(const QString &text)
    {
        property() = text;
    }
};

PropertyDelegateQStringList::PropertyDelegateQStringList(PropertyQStringBase &owner)
    : PropertyDelegateQString(owner)
{
}

void PropertyDelegateQStringList::applyAttributesImpl(const PropertyDelegateAttributes &attributes)
{
    getAttribute(attributes, "items", m_items);
}

QWidget *PropertyDelegateQStringList::createValueEditorImpl(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo)
{
    QComboBox *editor = new QComboBox(parent);
    editor->setGeometry(rect);

    editor->addItems(m_items);

    new PropertyQStringListComboBoxHandler(owner(), *editor);

    if (inplaceInfo)
    {
        editor->showPopup();
    }

    return editor;
}

} // end namespace Qtinuum

