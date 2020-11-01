/*******************************************************************************
Copyright (c) 2012-2019 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include "PropertyDelegateQString.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"
#include "QtnProperty/PropertyDelegateAttrs.h"
#include "QtnProperty/MultiProperty.h"
#include "QtnProperty/Utils/MultilineTextDialog.h"
#include "QtnProperty/Utils/InplaceEditing.h"
#include "QtnProperty/Utils/QtnCompleterLineEdit.h"
#include "QtnProperty/PropertyView.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QFileInfo>
#include <QFileDialog>
#include <QComboBox>
#include <QCompleter>
#include <QTimer>
#include <QDebug>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QListView>

static QString toSingleLine(const QString &str)
{
	int n = str.indexOf('\n');
	int r = str.indexOf('\r');
	int len = n < 0 ? r : (r < 0 ? n : qMin(n, r));
	return QString(str.data(), len);
}

QByteArray qtnMultiLineEditAttr()
{
	return QByteArrayLiteral("multiline_edit");
}

QByteArray qtnMaxLengthAttr()
{
	return QByteArrayLiteral("max_length");
}

QByteArray qtnPlaceholderAttr()
{
	return QByteArrayLiteral("placeholder");
}

QByteArray qtnItemsAttr()
{
	return QByteArrayLiteral("items");
}

QByteArray qtnEditableAttr()
{
	return QByteArrayLiteral("editable");
}

QByteArray qtnInvalidColorAttr()
{
	return QByteArrayLiteral("invalidColor");
}

QByteArray qtnShowRelativePathAttr()
{
	return QByteArrayLiteral("showRelativePath");
}

QByteArray qtnFileNameFilterAttr()
{
	return QByteArrayLiteral("nameFilter");
}

QByteArray qtnFileNameFiltersAttr()
{
	return QByteArrayLiteral("nameFilters");
}

QByteArray qtnDefaultSuffixAttr()
{
	return QByteArrayLiteral("defaultSuffix");
}

QByteArray qtnDefaultDirAttr()
{
	return QByteArrayLiteral("defaultDirectory");
}

QByteArray qtnOptionsAttr()
{
	return QByteArrayLiteral("options");
}

QByteArray qtnFileModeAttr()
{
	return QByteArrayLiteral("fileMode");
}

QByteArray qtnViewModeAttr()
{
	return QByteArrayLiteral("viewMode");
}

QByteArray qtnAcceptModeAttr()
{
	return QByteArrayLiteral("acceptMode");
}

QByteArray qtnGetCandidatesFnAttr()
{
	return QByteArrayLiteral("GetCandidatesFn");
}

QByteArray qtnCreateCandidateFnAttr()
{
	return QByteArrayLiteral("CreateCandidateFn");
}

QByteArray qtnCreateCandidateIconAttr()
{
	return QByteArrayLiteral("CreateCandidateIcon");
}

QByteArray qtnCreateCandidateToolTipAttr()
{
	return QByteArrayLiteral("CreateCandidateToolTip");
}

QByteArray qtnLineEditDelegate()
{
	return QByteArrayLiteral("LineEdit");
}

QByteArray qtnSelectFileDelegate()
{
	return QByteArrayLiteral("File");
}

QByteArray qtnCallbackDelegate()
{
	return QByteArrayLiteral("Callback");
}

class QtnPropertyQStringLineEditHandler
	: public QtnPropertyEditorHandler<QtnPropertyQStringBase, QLineEdit>
{
public:
	QtnPropertyQStringLineEditHandler(QtnPropertyDelegate *delegate,
		QLineEdit &editor, const QString &placeholder);

protected:
	virtual void updateEditor() override;
	void updateValue();

private:
	QString placeholder;
};

class QtnPropertyQStringMultilineEditBttnHandler
	: public QtnPropertyEditorBttnHandler<QtnPropertyQStringBase,
		  QtnLineEditBttn>
{
public:
	QtnPropertyQStringMultilineEditBttnHandler(QtnPropertyDelegate *delegate,
		QtnLineEditBttn &editor, const QString &placeholder);

protected:
	virtual void updateEditor() override;
	virtual void revertInput() override;
	virtual void onToolButtonClick() override;

private:
	void onEditingFinished();
	void onToolButtonClicked(bool);

	MultilineTextDialog *dialog;
	DialogContainerPtr dialogContainer;
	bool multiline;
	QString placeholder;
};

QtnPropertyDelegateQString::QtnPropertyDelegateQString(
	QtnPropertyQStringBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyQStringBase>(owner)
	, m_maxLength(0x1000000)
	, m_multiline(true)
{
}

void QtnPropertyDelegateQString::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQStringBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQString, QtnPropertyQStringBase>,
		qtnLineEditDelegate());
}

void QtnPropertyDelegateQString::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnMultiLineEditAttr(), m_multiline);
	info.loadAttribute(qtnMaxLengthAttr(), m_maxLength);
	info.loadAttribute(qtnPlaceholderAttr(), m_placeholder);
}

bool QtnPropertyDelegateQString::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<QtnPropertyQStringBase>::
			acceptKeyPressedForInplaceEditImpl(keyEvent))
		return true;

	// accept any printable key
	return qtnAcceptForLineEdit(keyEvent);
}

QWidget *QtnPropertyDelegateQString::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	if (m_multiline)
	{
		QtnLineEditBttn *editor = new QtnLineEditBttn(parent);
		editor->setGeometry(rect);

		editor->lineEdit->setMaxLength(m_maxLength);
		editor->lineEdit->setPlaceholderText(m_placeholder);

		new QtnPropertyQStringMultilineEditBttnHandler(
			this, *editor, m_placeholder);

		qtnInitLineEdit(editor->lineEdit, inplaceInfo);
		return editor;
	}

	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setMaxLength(m_maxLength);
	lineEdit->setPlaceholderText(m_placeholder);
	lineEdit->setGeometry(rect);

	new QtnPropertyQStringLineEditHandler(this, *lineEdit, m_placeholder);

	qtnInitLineEdit(lineEdit, inplaceInfo);

	return lineEdit;
}

bool QtnPropertyDelegateQString::propertyValueToStrImpl(QString &strValue) const
{
	strValue = owner().value();

	auto placeholder = strValue.isEmpty() && !m_placeholder.isEmpty()
		? m_placeholder
		: QtnPropertyQString::getPlaceholderStr(strValue, m_multiline);

	if (!placeholder.isEmpty())
		strValue.swap(placeholder);

	return true;
}

bool QtnPropertyDelegateQString::isPlaceholderColor() const
{
	auto text = owner().value();
	if (text.isEmpty() && !m_placeholder.isEmpty())
	{
		return true;
	}

	return stateProperty()->isEditableByUser() &&
		(stateProperty()->isMultiValue() ||
			!QtnPropertyQString::getPlaceholderStr(text, m_multiline)
				 .isEmpty());
}

class QtnPropertyQStringFileLineEditBttnHandler
	: public QtnPropertyEditorBttnHandler<QtnPropertyQStringBase,
		  QtnLineEditBttn>
{
public:
	QtnPropertyQStringFileLineEditBttnHandler(
		QtnPropertyDelegate *delegate, QtnLineEditBttn &editor);

	void applyAttributes(const QtnPropertyDelegateInfo &info);

	virtual void onToolButtonClick() override;
	virtual void updateEditor() override;

private:
	void onToolButtonClicked(bool);
	void onEditingFinished();

	QFileDialog *dialog;
	DialogContainerPtr dialogContainer;
	QString defaultDirectory;
};

QtnPropertyDelegateQStringInvalidBase::QtnPropertyDelegateQStringInvalidBase(
	QtnPropertyQStringBase &owner)
	: QtnPropertyDelegateQString(owner)
	, m_invalidColor(Qt::red)
{
	m_multiline = false;
}

void QtnPropertyDelegateQStringInvalidBase::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnInvalidColorAttr(), m_invalidColor);
}

void QtnPropertyDelegateQStringInvalidBase::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	QPen oldPen = painter.pen();

	if ((m_invalidColor.alpha() != 0) && isNormalPainterState(painter) &&
		!isPlaceholderColor() && !isPropertyValid() &&
		stateProperty()->isEditableByUser())
	{
		painter.setPen(m_invalidColor.rgb());
	}

	QtnPropertyDelegateQString::drawValueImpl(painter, rect);
	painter.setPen(oldPen);
}

QtnPropertyDelegateQStringFile::QtnPropertyDelegateQStringFile(
	QtnPropertyQStringBase &owner)
	: QtnPropertyDelegateQStringInvalidBase(owner)
{
}

void QtnPropertyDelegateQStringFile::Register(
	QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegate(&QtnPropertyQStringBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQStringFile,
			QtnPropertyQStringBase>,
		qtnSelectFileDelegate());
}

void QtnPropertyDelegateQStringFile::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	QtnPropertyDelegateQStringInvalidBase::applyAttributesImpl(info);
	m_editorAttributes = info;
}

bool QtnPropertyDelegateQStringFile::propertyValueToStrImpl(
	QString &strValue) const
{
	if (!owner().value().isEmpty())
	{
		strValue = QDir::toNativeSeparators(
			shouldShowRelativePath() ? relativeFilePath() : absoluteFilePath());
		return true;
	}

	return QtnPropertyDelegateQStringInvalidBase::propertyValueToStrImpl(
		strValue);
}

bool QtnPropertyDelegateQStringFile::toolTipImpl(QString &strValue) const
{
	strValue = QDir::toNativeSeparators(absoluteFilePath());
	return true;
}

QWidget *QtnPropertyDelegateQStringFile::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QtnLineEditBttn *editor = new QtnLineEditBttn(parent);
	editor->setGeometry(rect);

	auto handler = new QtnPropertyQStringFileLineEditBttnHandler(this, *editor);
	handler->applyAttributes(m_editorAttributes);

	qtnInitLineEdit(editor->lineEdit, inplaceInfo);

	return editor;
}

bool QtnPropertyDelegateQStringFile::isPropertyValid() const
{
	auto filePath = absoluteFilePath();

	if (filePath.isEmpty())
		return true;

	auto fileMode = QFileDialog::FileMode(m_editorAttributes.getAttribute(
		qtnFileModeAttr(), QFileDialog::AnyFile));

	switch (fileMode)
	{
		case QFileDialog::AnyFile:
			return true;

		case QFileDialog::ExistingFile:
		case QFileDialog::ExistingFiles:
			return QFileInfo(filePath).isFile();

		case QFileDialog::Directory:
		case QFileDialog::DirectoryOnly:
			return QFileInfo(filePath).isDir();
	}

	return false;
}

QString QtnPropertyDelegateQStringFile::defaultDirectory() const
{
	return m_editorAttributes.getAttribute(qtnDefaultDirAttr(), QString());
}

QString QtnPropertyDelegateQStringFile::absoluteFilePath() const
{
	QString result = owner().value();

	if (!result.isEmpty() && QDir::isRelativePath(result))
	{
		auto defaultDir = defaultDirectory();
		if (!defaultDir.isEmpty())
			result = QDir(defaultDir).filePath(result);
	}

	return result;
}

QString QtnPropertyDelegateQStringFile::relativeFilePath() const
{
	QString result = owner().value();

	if (!result.isEmpty() && QDir::isAbsolutePath(result))
	{
		auto defaultDir = defaultDirectory();

		if (!defaultDir.isEmpty())
			return QDir(defaultDir).relativeFilePath(result);
	}

	return result;
}

bool QtnPropertyDelegateQStringFile::shouldShowRelativePath() const
{
	if (defaultDirectory().isEmpty())
		return false;

	return m_editorAttributes.getAttribute(qtnShowRelativePathAttr(), false);
}

class QtnPropertyQStringListComboBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyQStringBase, QComboBox>
{
public:
	QtnPropertyQStringListComboBoxHandler(QtnPropertyDelegate *delegate,
		QComboBox &editor, const QtnPropertyDelegateInfo &info);

private:
	virtual void updateEditor() override;
	virtual void updateValue() override;
};

QtnPropertyDelegateQStringList::QtnPropertyDelegateQStringList(
	QtnPropertyQStringBase &owner)
	: QtnPropertyDelegateQString(owner)
{
}

void QtnPropertyDelegateQStringList::Register(
	QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegate(&QtnPropertyQStringBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQStringList,
			QtnPropertyQStringBase>,
		qtnComboBoxDelegate());
}

void QtnPropertyDelegateQStringList::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	m_editorAttributes = info;
}

QWidget *QtnPropertyDelegateQStringList::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QComboBox *editor = new QtnPropertyComboBox(this, parent);
	editor->setGeometry(rect);

	auto handler = new QtnPropertyQStringListComboBoxHandler(
		this, *editor, m_editorAttributes);
	Q_UNUSED(handler);

	if (inplaceInfo && stateProperty()->isEditableByUser())
	{
		editor->showPopup();
	}

	return editor;
}

QtnPropertyQStringListComboBoxHandler::QtnPropertyQStringListComboBoxHandler(
	QtnPropertyDelegate *delegate, QComboBox &editor,
	const QtnPropertyDelegateInfo &info)
	: QtnPropertyEditorHandlerVT(delegate, editor)
{
	bool editable = false;
	info.loadAttribute(qtnEditableAttr(), editable);
	QStringList items;
	info.loadAttribute(qtnItemsAttr(), items);

	editor.clear();
	editor.addItems(items);
	editor.setEditable(editable);
	editor.setAutoCompletion(false);

	if (editable)
		editor.installEventFilter(this);
	updateEditor();

	QObject::connect(&editor, &QComboBox::currentTextChanged, this,
		&QtnPropertyQStringListComboBoxHandler::onValueChanged);
}

void QtnPropertyQStringListComboBoxHandler::updateEditor()
{
	updating++;

	editor().setEnabled(stateProperty()->isEditableByUser());

	auto lineEdit = editor().lineEdit();
	if (stateProperty()->isMultiValue())
	{
		editor().clearEditText();
		if (lineEdit)
		{
			lineEdit->setPlaceholderText(
				QtnMultiProperty::getMultiValuePlaceholder());
		}
	} else
	{
		editor().setCurrentText(property());
		if (editor().currentText() != property().value())
			editor().setCurrentIndex(-1);
		if (lineEdit)
		{
			lineEdit->setPlaceholderText(QString());
		}
	}

	if (lineEdit)
		lineEdit->selectAll();

	updating--;
}

void QtnPropertyQStringListComboBoxHandler::updateValue()
{
	if (!editor().isEditable() || canApply())
	{
		property().setValue(toSingleLine(newValue), delegate()->editReason());
	}

	applyReset();
}

QtnPropertyQStringFileLineEditBttnHandler::
	QtnPropertyQStringFileLineEditBttnHandler(
		QtnPropertyDelegate *delegate, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(delegate, editor)
	, dialog(new QFileDialog(&editor))
{
	dialogContainer = connectDialog(dialog);

	updateEditor();

	editor.lineEdit->installEventFilter(this);
	QObject::connect(editor.toolButton, &QToolButton::clicked, this,
		&QtnPropertyQStringFileLineEditBttnHandler::onToolButtonClicked);
	QObject::connect(editor.lineEdit, &QLineEdit::editingFinished, this,
		&QtnPropertyQStringFileLineEditBttnHandler::onEditingFinished);
}

void QtnPropertyQStringFileLineEditBttnHandler::applyAttributes(
	const QtnPropertyDelegateInfo &info)
{
	int option = 0;

	if (info.loadAttribute(qtnAcceptModeAttr(), option))
		dialog->setAcceptMode(QFileDialog::AcceptMode(option));

	QString str;

	if (info.loadAttribute(qtnDefaultSuffixAttr(), str))
		dialog->setDefaultSuffix(str);

	if (info.loadAttribute(qtnDefaultDirAttr(), str))
		defaultDirectory = str;

	if (info.loadAttribute(qtnFileModeAttr(), option))
		dialog->setFileMode(QFileDialog::FileMode(option));

	if (info.loadAttribute(qtnOptionsAttr(), option))
		dialog->setOptions(QFileDialog::Options(QFlag(option)));

	if (info.loadAttribute(qtnViewModeAttr(), option))
		dialog->setViewMode(QFileDialog::ViewMode(option));

	if (info.loadAttribute(qtnFileNameFilterAttr(), str))
		dialog->setNameFilter(str);

	QStringList list;

	if (info.loadAttribute(qtnFileNameFiltersAttr(), list))
		dialog->setNameFilters(list);
}

void QtnPropertyQStringFileLineEditBttnHandler::onToolButtonClick()
{
	onToolButtonClicked(false);
}

void QtnPropertyQStringFileLineEditBttnHandler::updateEditor()
{
	bool editable = stateProperty()->isEditableByUser();
	editor().lineEdit->setReadOnly(!editable);
	editor().toolButton->setEnabled(editable);

	auto path = QDir::toNativeSeparators(property().value());
	editor().setTextForProperty(stateProperty(), path);

	if (!stateProperty()->isMultiValue())
	{
		auto edit = editor().lineEdit;
		edit->setPlaceholderText(
			QtnPropertyQString::getPlaceholderStr(edit->text(), false));

		edit->selectAll();
	}
}

void QtnPropertyQStringFileLineEditBttnHandler::onToolButtonClicked(bool)
{
	auto property = &this->property();
	volatile bool destroyed = false;
	auto connection = QObject::connect(property, &QObject::destroyed,
		[&destroyed]() mutable { destroyed = true; });
	reverted = true;
	auto dialogContainer = this->dialogContainer;
	QString filePath = property->value();
	QString dirPath = this->defaultDirectory;
	QFileInfo fileInfo(filePath);

	if (!filePath.isEmpty())
	{
		filePath = QDir(dirPath).filePath(filePath);
		fileInfo.setFile(filePath);
		dirPath = fileInfo.path();
	}

	dialog->setDirectory(dirPath);
	dialog->selectFile(filePath);

	if (dialog->exec() == QDialog::Accepted && !destroyed)
	{
		QStringList files = dialog->selectedFiles();

		if (files.size() == 1)
			property->setValue(QDir::toNativeSeparators(files.first()),
				delegate()->editReason());
	}

	if (!destroyed)
		QObject::disconnect(connection);

	Q_UNUSED(dialogContainer);
}

void QtnPropertyQStringFileLineEditBttnHandler::onEditingFinished()
{
	if (canApply())
	{
		property().setValue(
			editor().lineEdit->text(), delegate()->editReason());
	}

	applyReset();
}

QtnPropertyQStringMultilineEditBttnHandler::
	QtnPropertyQStringMultilineEditBttnHandler(QtnPropertyDelegate *delegate,
		QtnLineEditBttn &editor, const QString &placeholder)
	: QtnPropertyEditorHandlerType(delegate, editor)
	, dialog(new MultilineTextDialog(&editor))
	, multiline(false)
	, placeholder(placeholder)
{
	dialogContainer = connectDialog(dialog);
	updateEditor();

	editor.lineEdit->installEventFilter(this);

	QObject::connect(editor.toolButton, &QToolButton::clicked, this,
		&QtnPropertyQStringMultilineEditBttnHandler::onToolButtonClicked);

	QObject::connect(editor.lineEdit, &QLineEdit::editingFinished, this,
		&QtnPropertyQStringMultilineEditBttnHandler::onEditingFinished);
}

void QtnPropertyQStringMultilineEditBttnHandler::updateEditor()
{
	auto edit = editor().lineEdit;
	edit->setReadOnly(!stateProperty()->isEditableByUser());

	if (stateProperty()->isMultiValue())
	{
		edit->clear();
		edit->setPlaceholderText(QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		auto text = property().value();

		if (QtnPropertyQString::isMultilineText(text))
		{
			multiline = true;
			edit->setText(QString());
		} else
		{
			multiline = false;
			edit->setText(text);
		}

		if (text.isEmpty() && !placeholder.isEmpty())
		{
			edit->setPlaceholderText(placeholder);
		} else
		{
			edit->setPlaceholderText(
				QtnPropertyQString::getPlaceholderStr(text, true));
		}
		edit->selectAll();
	}
}

void QtnPropertyQStringMultilineEditBttnHandler::revertInput()
{
	reverted = true;
}

void QtnPropertyQStringMultilineEditBttnHandler::onToolButtonClick()
{
	onToolButtonClicked(false);
}

void QtnPropertyQStringMultilineEditBttnHandler::onEditingFinished()
{
	if (canApply())
	{
		auto text = editor().lineEdit->text();

		if (!multiline || !text.isEmpty())
		{
			property().setValue(text, delegate()->editReason());
			updateEditor();
		}
	}

	applyReset();
}

void QtnPropertyQStringMultilineEditBttnHandler::onToolButtonClicked(bool)
{
	auto text = editor().lineEdit->text();

	auto property = &this->property();

	if (text.isEmpty() && multiline)
	{
		text = property->value();
	}

	reverted = true;
	bool readonly = !stateProperty()->isEditableByUser();
	auto dialogContainer = this->dialogContainer;
	dialog->setReadOnly(readonly);

	if (readonly)
	{
		dialog->setWindowTitle(
			QtnPropertyQString::getReadOnlyPropertyTitleFormat().arg(
				property->displayName()));
	} else
	{
		dialog->setWindowTitle(property->displayName());
	}

	dialog->setText(text);
	dialog->show();
	dialog->raise();
	volatile bool destroyed = false;
	auto connection = QObject::connect(this, &QObject::destroyed,
		[&destroyed]() mutable { destroyed = true; });

	if (dialog->exec() == QDialog::Accepted && !destroyed)
		property->setValue(dialog->getText(), delegate()->editReason());

	if (!destroyed)
	{
		QObject::disconnect(connection);
		updateEditor();
	}

	Q_UNUSED(dialogContainer);
}

QtnPropertyQStringLineEditHandler::QtnPropertyQStringLineEditHandler(
	QtnPropertyDelegate *delegate, QLineEdit &editor,
	const QString &placeholder)
	: QtnPropertyEditorHandler(delegate, editor)
	, placeholder(placeholder)
{
	updateEditor();

	editor.installEventFilter(this);
	QObject::connect(&editor, &QLineEdit::editingFinished, this,
		&QtnPropertyQStringLineEditHandler::updateValue);
}

void QtnPropertyQStringLineEditHandler::updateEditor()
{
	editor().setReadOnly(!stateProperty()->isEditableByUser());

	if (stateProperty()->isMultiValue())
	{
		editor().clear();
		editor().setPlaceholderText(
			QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		auto text = property().value();
		editor().setText(text);
		editor().setPlaceholderText(text.isEmpty() && !placeholder.isEmpty()
				? placeholder
				: QtnPropertyQString::getPlaceholderStr(text, false));
		editor().selectAll();
	}
}

void QtnPropertyQStringLineEditHandler::updateValue()
{
	if (canApply())
	{
		property().setValue(
			toSingleLine(editor().text()), delegate()->editReason());
	}

	applyReset();
}

class QtnPropertyQStringCandidatesComboBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyQStringBase, QtnLineEditBttn>
{
public:
	QtnPropertyQStringCandidatesComboBoxHandler(
		QtnPropertyDelegateQStringCallback *delegate,
		QtnCompleterLineEdit *lineEdit, QtnLineEditBttn &editor,
		const QString &placeholder);

	void shouldFinishEdit();

private:
	void updateCandidates();
	void updatePlaceholder(const QString &text);

	virtual bool canApply() const override;
	virtual void updateEditor() override;
	virtual void revertInput() override;
	virtual bool eventFilter(QObject *watched, QEvent *event) override;

	void onItemActivated();

	bool createNewCandidate(QString candidate);

	void onEditingFinished();
	void onToolButtonClicked();

	void connectLineEdit();
	void disconnectLineEdit();

private:
	QtnGetCandidatesFn m_getCandidatesFn;
	QtnCreateCandidateFn m_createCandidateFn;
	QtnCompleterLineEdit *mLineEdit;

	QStringList m_candidates;
	QString m_placeholder;
};

QtnPropertyDelegateQStringCallback::QtnPropertyDelegateQStringCallback(
	QtnPropertyQStringBase &owner)
	: QtnPropertyDelegateQString(owner)
{
}

void QtnPropertyDelegateQStringCallback::Register(
	QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegate(&QtnPropertyQStringBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQStringCallback,
			QtnPropertyQStringBase>,
		qtnCallbackDelegate());
}

void QtnPropertyDelegateQStringCallback::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	m_editorAttributes = info;
}

QWidget *QtnPropertyDelegateQStringCallback::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto lineEdit = new QtnCompleterLineEdit;
	auto editor = new QtnLineEditBttn(parent, QStringLiteral("*"), lineEdit);
	editor->setGeometry(rect);
	editor->lineEdit->setPlaceholderText(m_placeholder);

	new QtnPropertyQStringCandidatesComboBoxHandler(
		this, lineEdit, *editor, m_placeholder);

	qtnInitLineEdit(lineEdit, inplaceInfo);

	return editor;
}

QtnPropertyQStringCandidatesComboBoxHandler::
	QtnPropertyQStringCandidatesComboBoxHandler(
		QtnPropertyDelegateQStringCallback *delegate,
		QtnCompleterLineEdit *lineEdit, QtnLineEditBttn &editor,
		const QString &placeholder)
	: QtnPropertyEditorHandlerVT(delegate, editor)
	, mLineEdit(lineEdit)
	, m_placeholder(placeholder)
{
	auto model = new QStringListModel(&editor);
	auto completer = lineEdit->completer();
	Q_ASSERT(completer);
	lineEdit->setCompleterModel(model);

	auto &attributes = delegate->m_editorAttributes;
	editor.toolButton->setIcon(
		attributes.getAttribute<QIcon>(qtnCreateCandidateIconAttr()));

	auto text = attributes.getAttribute<QString>(qtnCreateCandidateIconAttr());

	if (!text.isEmpty())
	{
		editor.toolButton->setText(text);
	}

	editor.toolButton->setToolTip(
		attributes.getAttribute<QString>(qtnCreateCandidateToolTipAttr()));

	attributes.loadAttribute(qtnCreateCandidateFnAttr(), m_createCandidateFn);
	attributes.loadAttribute(qtnGetCandidatesFnAttr(), m_getCandidatesFn);

	updateCandidates();
	QtnPropertyQStringCandidatesComboBoxHandler::updateEditor();

	mLineEdit->installEventFilter(this);
	completer->popup()->installEventFilter(this);
	completer->popup()->viewport()->installEventFilter(this);

	QObject::connect(editor.toolButton, &QToolButton::clicked, this,
		&QtnPropertyQStringCandidatesComboBoxHandler::onToolButtonClicked);

	connectLineEdit();
}

void QtnPropertyQStringCandidatesComboBoxHandler::updateCandidates()
{
	updating++;

	if (m_getCandidatesFn)
		m_candidates = m_getCandidatesFn();

	auto completer = editor().lineEdit->completer();
	Q_ASSERT(completer);
	Q_ASSERT(qobject_cast<QStringListModel *>(completer->model()));
	auto model = static_cast<QStringListModel *>(completer->model());
	model->setStringList(m_candidates);

	updating--;
}

void QtnPropertyQStringCandidatesComboBoxHandler::updatePlaceholder(
	const QString &text)
{
	QString placeholderStr;
	if (stateProperty()->isMultiValue())
	{
		placeholderStr = QtnMultiProperty::getMultiValuePlaceholder();
	} else if (text.isEmpty())
	{
		if (m_placeholder.isEmpty())
		{
			placeholderStr = QtnPropertyQString::getEmptyPlaceholderStr();
		} else
		{
			placeholderStr = m_placeholder;
		}
	} else
	{
		placeholderStr = QtnPropertyQString::getPlaceholderStr(text, false);
	}
	editor().lineEdit->setPlaceholderText(placeholderStr);
}

bool QtnPropertyQStringCandidatesComboBoxHandler::canApply() const
{
	return !reverted && returned && stateProperty() &&
		stateProperty()->isEditableByUser();
}

void QtnPropertyQStringCandidatesComboBoxHandler::updateEditor()
{
	updating++;

	bool enabled = stateProperty()->isEditableByUser();

	mLineEdit->setReadOnly(!enabled);
	if (stateProperty()->isMultiValue())
	{
		mLineEdit->clear();
	} else
	{
		mLineEdit->setText(property());
	}

	updatePlaceholder(mLineEdit->text());

	QMetaObject::invokeMethod(mLineEdit, "complete", Qt::QueuedConnection);
	mLineEdit->selectAll();

	updating--;
}

void QtnPropertyQStringCandidatesComboBoxHandler::shouldFinishEdit()
{
	QMetaObject::invokeMethod(
		mLineEdit, "editingFinished", Qt::QueuedConnection);
}

void QtnPropertyQStringCandidatesComboBoxHandler::revertInput()
{
	if (updating)
		return;
	reverted = true;
	shouldFinishEdit();
}

bool QtnPropertyQStringCandidatesComboBoxHandler::eventFilter(
	QObject *watched, QEvent *event)
{
	switch (event->type())
	{
		case QEvent::MouseButtonPress:
		case QEvent::MouseButtonRelease:
		case QEvent::MouseMove:
		case QEvent::MouseButtonDblClick:
		{
			auto me = static_cast<QMouseEvent *>(event);
			auto toolButton = editor().toolButton;
			auto localPos = toolButton->mapFromGlobal(me->globalPos());
			if (toolButton->rect().contains(localPos))
			{
				QObject *toolButtonObject = toolButton;
				QMouseEvent buttonEvent(event->type(), localPos,
					me->windowPos(), me->globalPos(), me->button(),
					me->buttons(), me->modifiers(), me->source());
				toolButtonObject->event(&buttonEvent);
				return true;
			}
			break;
		}

		default:
			break;
	}

	return QObject::eventFilter(watched, event);
}

void QtnPropertyQStringCandidatesComboBoxHandler::onItemActivated()
{
	if (updating)
		return;

	returned = true;
	shouldFinishEdit();
}

bool QtnPropertyQStringCandidatesComboBoxHandler::createNewCandidate(
	QString candidate)
{
	if (!m_createCandidateFn || updating)
		return false;

	qtnRetainInplaceEditor();
	updating++;

	candidate = m_createCandidateFn(&editor(), candidate);

	updating--;
	if (!candidate.isEmpty())
	{
		onValueChanged(candidate);
		updateCandidates();
	}

	qtnReleaseInplaceEditor();
	return !candidate.isEmpty();
}

void QtnPropertyQStringCandidatesComboBoxHandler::onEditingFinished()
{
	if (updating)
		return;

	disconnectLineEdit();
	if (canApply())
	{
		auto text = toSingleLine(mLineEdit->text());

		if (!m_createCandidateFn || text.isEmpty() ||
			m_candidates.contains(text, Qt::CaseInsensitive))
		{
			property().setValue(text, delegate()->editReason());
		} else
		{
			if (!createNewCandidate(text))
			{
				applyReset();
				connectLineEdit();
				return;
			}
		}
	}

	applyReset();
	qtnStopInplaceEdit();
}

void QtnPropertyQStringCandidatesComboBoxHandler::onToolButtonClicked()
{
	createNewCandidate(mLineEdit->text());
	applyReset();
}

void QtnPropertyQStringCandidatesComboBoxHandler::connectLineEdit()
{
	QObject::connect(mLineEdit, &QLineEdit::textChanged, this,
		&QtnPropertyQStringCandidatesComboBoxHandler::updatePlaceholder);
	QObject::connect(mLineEdit, &QtnCompleterLineEdit::returnPressed, this,
		&QtnPropertyQStringCandidatesComboBoxHandler::onItemActivated);
	QObject::connect(mLineEdit, &QtnCompleterLineEdit::escaped, this,
		&QtnPropertyQStringCandidatesComboBoxHandler::revertInput);
	QObject::connect(mLineEdit, &QLineEdit::editingFinished, this,
		&QtnPropertyQStringCandidatesComboBoxHandler::onEditingFinished);
	QObject::connect(mLineEdit->completer(),
		static_cast<void (QCompleter::*)(const QModelIndex &)>(
			&QCompleter::activated),
		this, &QtnPropertyQStringCandidatesComboBoxHandler::onItemActivated);
}

void QtnPropertyQStringCandidatesComboBoxHandler::disconnectLineEdit()
{
	QObject::disconnect(mLineEdit, &QtnCompleterLineEdit::returnPressed, this,
		&QtnPropertyQStringCandidatesComboBoxHandler::onItemActivated);
	QObject::disconnect(mLineEdit, &QtnCompleterLineEdit::escaped, this,
		&QtnPropertyQStringCandidatesComboBoxHandler::revertInput);
	QObject::disconnect(mLineEdit, &QLineEdit::editingFinished, this,
		&QtnPropertyQStringCandidatesComboBoxHandler::onEditingFinished);
	QObject::disconnect(mLineEdit->completer(),
		static_cast<void (QCompleter::*)(const QModelIndex &)>(
			&QCompleter::activated),
		this, &QtnPropertyQStringCandidatesComboBoxHandler::onItemActivated);
}
