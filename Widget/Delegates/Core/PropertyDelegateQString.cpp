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

#include "PropertyDelegateQString.h"
#include "../../../Core/Core/PropertyQString.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"
#include "../PropertyEditorAux.h"
#include "../GUI/MultilineTextDialog.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QFileInfo>
#include <QFileDialog>
#include <QComboBox>
#include <QDebug>

static bool regQStringDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyQStringBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateQString, QtnPropertyQStringBase>
                                , "LineEdit");

static bool regQStringFileDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegate(&QtnPropertyQStringBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateQStringFile, QtnPropertyQStringBase>
                                , "File");

static bool regQStringListDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegate(&QtnPropertyQStringBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateQStringList, QtnPropertyQStringBase>
                                , "List");

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


class QtnPropertyQStringMultilineEditBttnHandler: public QtnPropertyEditorHandler<QtnPropertyQStringBase, QtnLineEditBttn>
{
public:
	QtnPropertyQStringMultilineEditBttnHandler(QtnPropertyQStringBase& property, QtnLineEditBttn& editor)
		: QtnPropertyEditorHandlerType(property, editor)
		, dialog(&editor)
		, revert(false)
		, multiline(false)
	{
		updateEditor();

		editor.lineEdit->installEventFilter(this);

		QObject::connect(editor.toolButton, &QToolButton::clicked,
		this, &QtnPropertyQStringMultilineEditBttnHandler::onToolButtonClicked);

		QObject::connect(editor.lineEdit, &QLineEdit::editingFinished,
		this, &QtnPropertyQStringMultilineEditBttnHandler::onEditingFinished);
	}

protected:
	virtual void updateEditor() override
	{
		auto edit = editor().lineEdit;
		edit->setReadOnly(!property().isEditableByUser());
		if (QtnPropertyDelegateQString::isMultilineText(property().value()))
		{
			multiline = true;
			edit->setText(QString());
			edit->setPlaceholderText(QtnPropertyQString::getMultilinePlaceholderStr());
		} else
		{
			multiline = false;
			edit->setText(property().value());
			edit->setPlaceholderText(QString());
		}
	}

	virtual bool eventFilter(QObject *obj, QEvent *event) override
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			// revert all changes
			if (keyEvent->key() == Qt::Key_Escape)
			{
				revert = true;
			}
		}

		return QObject::eventFilter(obj, event);
	}

private:
	void onEditingFinished()
	{
		auto text = editor().lineEdit->text();
		if (!revert && (!multiline || !text.isEmpty()))
		{
			property() = text;
			updateEditor();
		}

		revert = false;
	}

	void onToolButtonClicked(bool)
	{
		auto text = editor().lineEdit->text();

		if (text.isEmpty() && multiline)
		{
			text = property().value();
		}

		revert = true;
		bool readonly = !property().isEditableByUser();
		dialog.setReadOnly(readonly);
		if (readonly)
			dialog.setWindowTitle(QtnPropertyQString::getReadOnlyPropertyTitleFormat().arg(property().name()));
		else
			dialog.setWindowTitle(property().name());
		dialog.setText(text);
		dialog.show();
		dialog.raise();
		if (dialog.exec() == QDialog::Accepted)
			property() = dialog.getText();			

		updateEditor();
	}

	MultilineTextDialog dialog;
	bool revert;
	bool multiline;
};

QtnPropertyDelegateQString::QtnPropertyDelegateQString(QtnPropertyQStringBase& owner)
    : QtnPropertyDelegateTyped<QtnPropertyQStringBase>(owner)
	, check_multiline(true)
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
	if (check_multiline)
	{
		QtnLineEditBttn* editor = new QtnLineEditBttn(parent);
		editor->setGeometry(rect);

		new QtnPropertyQStringMultilineEditBttnHandler(owner(), *editor);

		qtnInitLineEdit(editor->lineEdit, inplaceInfo);
		return editor;
	}

	QLineEdit* lineEdit = new QLineEdit(parent);

	lineEdit->setGeometry(rect);

	new QtnPropertyQStringLineEditHandler(owner(), *lineEdit);

	qtnInitLineEdit(lineEdit, inplaceInfo);

	return lineEdit;
}

bool QtnPropertyDelegateQString::propertyValueToStr(QString& strValue) const
{
    strValue = owner().value();
	if (check_multiline && isMultilineText(strValue))
		strValue = QtnPropertyQString::getMultilinePlaceholderStr();
	return true;
}

void QtnPropertyDelegateQString::drawValueImpl(QStylePainter &painter, const QRect &rect, const QStyle::State &state, bool *needTooltip) const
{
	if (check_multiline && isMultilineText(owner().value()))
	{
		QPen oldPen = painter.pen();
		painter.setPen(Qt::darkGray);
		Inherited::drawValueImpl(painter, rect, state, needTooltip);
		painter.setPen(oldPen);
	}
	else
	{
		Inherited::drawValueImpl(painter, rect, state, needTooltip);
	}
}

bool QtnPropertyDelegateQString::isMultilineText(const QString &text)
{
	return text.contains('\n') || text.contains('\r');
}

class QtnPropertyQStringFileLineEditBttnHandler: public QtnPropertyEditorHandler<QtnPropertyQStringBase, QtnLineEditBttn>
{
public:
    QtnPropertyQStringFileLineEditBttnHandler(QtnPropertyQStringBase& property, QtnLineEditBttn& editor)
        : QtnPropertyEditorHandlerType(property, editor),
          m_dlg(&editor)
    {
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
	check_multiline = false;
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
	check_multiline = false;
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
