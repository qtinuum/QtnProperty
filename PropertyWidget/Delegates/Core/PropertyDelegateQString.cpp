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

#include "PropertyDelegateQString.h"
#include "../../../Core/Core/PropertyQString.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"
#include "../Utils/PropertyEditorAux.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QFileInfo>
#include <QFileDialog>
#include <QComboBox>
#include <QCompleter>
#include <QTimer>
#include <QDebug>

void regQStringDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyQStringBase::staticMetaObject
              , &qtnCreateDelegate<QtnPropertyDelegateQString, QtnPropertyQStringBase>
              , "LineEdit");

    factory.registerDelegate(&QtnPropertyQStringBase::staticMetaObject
		      , &qtnCreateDelegate<QtnPropertyDelegateQStringFile, QtnPropertyQStringBase>
		      , "File");

    factory.registerDelegate(&QtnPropertyQStringBase::staticMetaObject
		      , &qtnCreateDelegate<QtnPropertyDelegateQStringList, QtnPropertyQStringBase>
		      , "List");

    factory.registerDelegate(&QtnPropertyQStringBase::staticMetaObject
              , &qtnCreateDelegate<QtnPropertyDelegateQStringCallback, QtnPropertyQStringBase>
              , "Callback");
}

class QtnPropertyQStringLineEditHandler: public QtnPropertyEditorHandler<QtnPropertyQStringBase, QLineEdit>
{
public:
    QtnPropertyQStringLineEditHandler(QtnPropertyQStringBase& property, QLineEdit& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.installEventFilter(this);
        QObject::connect(  &editor, &QLineEdit::editingFinished
                         , this, &QtnPropertyQStringLineEditHandler::onEditingFinished);
    }

    ~QtnPropertyQStringLineEditHandler()
    {
    }

private:
    void updateEditor() override
    {
        editor().setText(property());
    }

    void onEditingFinished()
    {
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

QtnPropertyDelegateQString::QtnPropertyDelegateQString(QtnPropertyQStringBase& owner)
    : QtnPropertyDelegateTyped<QtnPropertyQStringBase>(owner)
{
}

bool QtnPropertyDelegateQString::acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const
{
    if (QtnPropertyDelegateTyped<QtnPropertyQStringBase>::acceptKeyPressedForInplaceEditImpl(keyEvent))
        return true;

    // accept any printable key
    return qtnAcceptForLineEdit(keyEvent);
}

QWidget* QtnPropertyDelegateQString::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QLineEdit* lineEdit = new QLineEdit(parent);
    lineEdit->setGeometry(rect);

    new QtnPropertyQStringLineEditHandler(owner(), *lineEdit);

    qtnInitLineEdit(lineEdit, inplaceInfo);

    return lineEdit;
}

bool QtnPropertyDelegateQString::propertyValueToStrImpl(QString& strValue) const
{
    strValue = owner().value();
    return true;
}

class QtnPropertyQStringFileLineEditBttnHandler: public QtnPropertyEditorHandler<QtnPropertyQStringBase, QtnLineEditBttn>
{
public:
    QtnPropertyQStringFileLineEditBttnHandler(QtnPropertyQStringBase& property, QtnLineEditBttn& editor)
        : QtnPropertyEditorHandlerType(property, editor),
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
                         , this, &QtnPropertyQStringFileLineEditBttnHandler::onToolButtonClicked);
        QObject::connect(  editor.lineEdit, &QLineEdit::editingFinished
                         , this, &QtnPropertyQStringFileLineEditBttnHandler::onEditingFinished);

    }

    void applyAttributes(const QtnPropertyDelegateAttributes& attributes)
    {
        int option = 0;
        if (qtnGetAttribute(attributes, "acceptMode", option))
            m_dlg.setAcceptMode(QFileDialog::AcceptMode(option));

        QString str;
        if (qtnGetAttribute(attributes, "defaultSuffix", str))
            m_dlg.setDefaultSuffix(str);

        if (qtnGetAttribute(attributes, "fileMode", option))
            m_dlg.setFileMode(QFileDialog::FileMode(option));

        if (qtnGetAttribute(attributes, "options", option))
            m_dlg.setOptions(QFileDialog::Options(QFlag(option)));

        if (qtnGetAttribute(attributes, "viewMode", option))
            m_dlg.setViewMode(QFileDialog::ViewMode(option));

        if (qtnGetAttribute(attributes, "nameFilter", str))
            m_dlg.setNameFilter(str);

        QStringList list;
        if (qtnGetAttribute(attributes, "nameFilters", list))
            m_dlg.setNameFilters(list);
    }

private:
    void updateEditor() override
    {
        editor().lineEdit->setText(property().value());
    }

    void onToolButtonClicked(bool checked)
    {
        Q_UNUSED(checked);

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

    bool eventFilter(QObject* obj, QEvent* event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            // revert all changes
            if (keyEvent->key() == Qt::Key_Escape)
                updateEditor();
        }

        return QObject::eventFilter(obj, event);
    }

    QFileDialog m_dlg;
};

QtnPropertyDelegateQStringInvalidBase::QtnPropertyDelegateQStringInvalidBase(QtnPropertyQStringBase& owner)
    : QtnPropertyDelegateQString(owner),
      m_invalidColor(Qt::red)
{
}

void QtnPropertyDelegateQStringInvalidBase::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    qtnGetAttribute(attributes, "invalidColor", m_invalidColor);
}

void QtnPropertyDelegateQStringInvalidBase::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
    if ((m_invalidColor.alpha() != 0) && !isPropertyValid())
    {
        QPen oldPen = painter.pen();
        painter.setPen(m_invalidColor);
        QtnPropertyDelegateQString::drawValueImpl(painter, rect, state, needTooltip);
        painter.setPen(oldPen);
    }
    else
    {
        QtnPropertyDelegateQString::drawValueImpl(painter, rect, state, needTooltip);
    }
}


QtnPropertyDelegateQStringFile::QtnPropertyDelegateQStringFile(QtnPropertyQStringBase& owner)
    : QtnPropertyDelegateQStringInvalidBase(owner)
{
}

void QtnPropertyDelegateQStringFile::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    QtnPropertyDelegateQStringInvalidBase::applyAttributesImpl(attributes);
    m_editorAttributes = attributes;
}

QWidget* QtnPropertyDelegateQStringFile::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QtnLineEditBttn* editor = new QtnLineEditBttn(parent);
    editor->setGeometry(rect);

    auto handler = new QtnPropertyQStringFileLineEditBttnHandler(owner(), *editor);
    handler->applyAttributes(m_editorAttributes);

    qtnInitLineEdit(editor->lineEdit, inplaceInfo);

    return editor;
}

bool QtnPropertyDelegateQStringFile::isPropertyValid() const
{
    return QFileInfo(owner().value()).exists();
}

class QtnPropertyQStringListComboBoxHandler: public QtnPropertyEditorHandler<QtnPropertyQStringBase, QComboBox>
{
public:
    QtnPropertyQStringListComboBoxHandler(QtnPropertyQStringBase& property, QComboBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(  &editor, &QComboBox::currentTextChanged
                         , this, &QtnPropertyQStringListComboBoxHandler::onCurrentTextChanged);
    }

private:
    void updateEditor() override
    {
        editor().setCurrentText(property());
    }

    void onCurrentTextChanged(const QString& text)
    {
        property() = text;
    }
};

QtnPropertyDelegateQStringList::QtnPropertyDelegateQStringList(QtnPropertyQStringBase& owner)
    : QtnPropertyDelegateQString(owner)
{
}

void QtnPropertyDelegateQStringList::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    qtnGetAttribute(attributes, "items", m_items);
}

QWidget* QtnPropertyDelegateQStringList::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    if (!owner().isEditableByUser())
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        lineEdit->setReadOnly(true);
        lineEdit->setText(owner().value());

        lineEdit->setGeometry(rect);

        return lineEdit;
    }

    QComboBox* editor = new QComboBox(parent);
    editor->setGeometry(rect);

    editor->addItems(m_items);

    new QtnPropertyQStringListComboBoxHandler(owner(), *editor);

    if (inplaceInfo)
    {
        editor->showPopup();
    }

    return editor;
}
/*
class QtnPropertyQStringCallbackLineEditBttnHandler: public QtnPropertyEditorHandler<QtnPropertyQStringBase, QtnLineEditBttn>
{
public:
    QtnPropertyQStringCallbackLineEditBttnHandler(QtnPropertyQStringBase& property, QtnLineEditBttn& editor, const QtnPropertyDelegateAttributes& attributes)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        qtnGetAttribute(attributes, "GetCandidatesFn", m_getCandidatesFn);
        qtnGetAttribute(attributes, "CreateCandidateFn", m_createCandidateFn);

        {
            QString createCandidateLabel = "*";
            qtnGetAttribute(attributes, "CreateCandidateLabel", createCandidateLabel);
            editor.toolButton->setText(createCandidateLabel);
        }

        if (!property.isEditableByUser())
        {
            editor.lineEdit->setReadOnly(true);
            editor.toolButton->setEnabled(false);
        }

        updateEditor();
        updateCompleter();

        editor.installEventFilter(this);
        QObject::connect(  editor.lineEdit, &QLineEdit::editingFinished
                         , this, &QtnPropertyQStringCallbackLineEditBttnHandler::onEditingFinished);

        if (m_createCandidateFn)
        {
            QObject::connect(  editor.toolButton, &QToolButton::clicked
                             , this, &QtnPropertyQStringCallbackLineEditBttnHandler::onToolButtonClicked);
        }
        else
        {
            editor.toolButton->setEnabled(false);
        }
    }

private:
    void updateCompleter()
    {
        if (m_getCandidatesFn)
            m_candidates = m_getCandidatesFn();

        QCompleter* completer = nullptr;
        if (!m_candidates.isEmpty())
        {
            completer = new QCompleter(m_candidates, editor().lineEdit);
            completer->setFilterMode(Qt::MatchContains);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
        }

        editor().lineEdit->setCompleter(completer);

        m_completer = completer;

        if (m_completer)
            m_completer->complete();
    }

    void updateEditor() override
    {
        editor().lineEdit->setText(property().value());
    }

    void createNewCandidate(QString candidate)
    {
        if (!m_createCandidateFn || m_stopSync)
            return;

        m_stopSync = true;

        candidate = m_createCandidateFn(editor().lineEdit, candidate);
        if (candidate.isEmpty())
            return;

        property() = candidate;
        updateCompleter();

        m_stopSync = false;
    }

    void onToolButtonClicked(bool checked)
    {
        Q_UNUSED(checked);
        createNewCandidate("");
    }

    void onEditingFinished()
    {
        if (m_stopSync)
            return;

        auto text = editor().lineEdit->text();
        if (m_candidates.indexOf(text) != -1)
            property() = text;
        else
            createNewCandidate(text);
    }

    bool eventFilter(QObject* obj, QEvent* event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            // revert all changes
            if (keyEvent->key() == Qt::Key_Escape)
                updateEditor();
        }

        return QObject::eventFilter(obj, event);
    }

    QtnGetCandidatesFn m_getCandidatesFn;
    QtnCreateCandidateFn m_createCandidateFn;

    QStringList m_candidates;
    QCompleter* m_completer = nullptr;

    bool m_stopSync = false;
};
*/

class QtnPropertyQStringCandidatesComboBoxHandler: public QtnPropertyEditorHandler<QtnPropertyQStringBase, QtnComboBoxBttn>
{
public:
    QtnPropertyQStringCandidatesComboBoxHandler(QtnPropertyQStringBase& property, QtnComboBoxBttn& editor, const QtnPropertyDelegateAttributes& attributes)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        Q_ASSERT(property.isEditableByUser());

        qtnGetAttribute(attributes, "GetCandidatesFn", m_getCandidatesFn);
        qtnGetAttribute(attributes, "CreateCandidateFn", m_createCandidateFn);

        {
            QString createCandidateLabel = "*";
            qtnGetAttribute(attributes, "CreateCandidateLabel", createCandidateLabel);
            editor.toolButton->setText(createCandidateLabel);
        }

        editor.comboBox->setEditable(true);
        editor.installEventFilter(this);

        updateCandidates();
        updateEditor();

        QObject::connect(  editor.comboBox->lineEdit(), &QLineEdit::editingFinished
                         , this, &QtnPropertyQStringCandidatesComboBoxHandler::onEditingFinished);

        if (m_createCandidateFn)
        {
            QObject::connect(  editor.toolButton, &QToolButton::clicked
                             , this, &QtnPropertyQStringCandidatesComboBoxHandler::onToolButtonClicked);
        }
        else
        {
            editor.toolButton->setEnabled(false);
        }
    }

private:
    void updateCandidates()
    {
        m_stopSync = true;

        if (m_getCandidatesFn)
            m_candidates = m_getCandidatesFn();

        QCompleter* completer = nullptr;
        if (!m_candidates.isEmpty())
        {
            completer = new QCompleter(m_candidates, editor().comboBox);
            completer->setFilterMode(Qt::MatchContains);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
        }

        editor().comboBox->setCompleter(completer);
        editor().comboBox->clear();
        editor().comboBox->addItems(m_candidates);

        QTimer::singleShot(0, editor().comboBox, [this](){
            editor().comboBox->showPopup();
            //editor().comboBox->lineEdit()->setFocus();
        });

        m_stopSync = false;
    }

    void updateEditor() override
    {
        if (m_stopSync)
            return;

        editor().comboBox->setCurrentText(property());
    }

    void createNewCandidate(QString candidate)
    {
        if (!m_createCandidateFn || m_stopSync)
            return;

        m_stopSync = true;

        candidate = m_createCandidateFn(&editor(), candidate);
        if (candidate.isEmpty())
        {
            m_stopSync = false;
            updateEditor();
            return;
        }

        property() = candidate;
        updateCandidates();

        m_stopSync = false;
    }

    void onToolButtonClicked(bool checked)
    {
        Q_UNUSED(checked);
        createNewCandidate("");
    }

    void onEditingFinished()
    {
        if (m_stopSync)
            return;

        auto text = editor().comboBox->currentText();
        if (text.isEmpty())
            return;

        if (m_candidates.indexOf(text) != -1)
            property() = text;
        else
            createNewCandidate(text);
    }

    bool eventFilter(QObject* obj, QEvent* event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            // revert all changes
            if (keyEvent->key() == Qt::Key_Escape)
                updateEditor();
        }

        return QObject::eventFilter(obj, event);
    }

    QtnGetCandidatesFn m_getCandidatesFn;
    QtnCreateCandidateFn m_createCandidateFn;

    QStringList m_candidates;

    bool m_stopSync = false;
};


QtnPropertyDelegateQStringCallback::QtnPropertyDelegateQStringCallback(QtnPropertyQStringBase& owner)
    : QtnPropertyDelegateQString(owner)
{
}

void QtnPropertyDelegateQStringCallback::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
    m_editorAttributes = attributes;
}

QWidget* QtnPropertyDelegateQStringCallback::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    if (!owner().isEditableByUser())
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        lineEdit->setReadOnly(true);
        lineEdit->setText(owner().value());

        lineEdit->setGeometry(rect);

        return lineEdit;
    }

    auto editor = new QtnComboBoxBttn(parent);
    editor->setGeometry(rect);

    new QtnPropertyQStringCandidatesComboBoxHandler(owner(), *editor, m_editorAttributes);

    qtnInitLineEdit(editor->comboBox->lineEdit(), inplaceInfo);

    return editor;
}
