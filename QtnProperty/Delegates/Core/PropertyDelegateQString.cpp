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

QByteArray qtnCreateCandidateLabelAttr()
{
	return QByteArrayLiteral("CreateCandidateLabel");
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
	QtnPropertyQStringLineEditHandler(
		QtnPropertyDelegate *delegate, QLineEdit &editor);

protected:
	virtual void updateEditor() override;
	void updateValue();
};

class QtnPropertyQStringMultilineEditBttnHandler
	: public QtnPropertyEditorBttnHandler<QtnPropertyQStringBase,
		  QtnLineEditBttn>
{
public:
	QtnPropertyQStringMultilineEditBttnHandler(
		QtnPropertyDelegate *delegate, QtnLineEditBttn &editor);

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

		new QtnPropertyQStringMultilineEditBttnHandler(this, *editor);

		qtnInitLineEdit(editor->lineEdit, inplaceInfo);
		return editor;
	}

	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setMaxLength(m_maxLength);

	lineEdit->setGeometry(rect);

	new QtnPropertyQStringLineEditHandler(this, *lineEdit);

	qtnInitLineEdit(lineEdit, inplaceInfo);

	return lineEdit;
}

bool QtnPropertyDelegateQString::propertyValueToStrImpl(QString &strValue) const
{
	strValue = owner().value();
	auto placeholder =
		QtnPropertyQString::getPlaceholderStr(strValue, m_multiline);

	if (!placeholder.isEmpty())
		strValue.swap(placeholder);

	return true;
}

void QtnPropertyDelegateQString::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	if (stateProperty()->isMultiValue())
	{
		QtnPropertyDelegateTyped::drawValueImpl(painter, rect);
		return;
	}

	QPen oldPen = painter.pen();

	if (isPlaceholderColor())
	{
		auto palette = painter.style()->standardPalette();
		if (palette.currentColorGroup() != QPalette::Disabled &&
			oldPen.color() != palette.color(QPalette::HighlightedText))
		{
			painter.setPen(palette.color(QPalette::Disabled, QPalette::Text));
		}
	}

	Inherited::drawValueImpl(painter, rect);
	painter.setPen(oldPen);
}

bool QtnPropertyDelegateQString::isPlaceholderColor() const
{
	return stateProperty()->isEditableByUser() &&
		(stateProperty()->isMultiValue() ||
			!QtnPropertyQString::getPlaceholderStr(owner().value(), m_multiline)
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

	if ((m_invalidColor.alpha() != 0) && !isPlaceholderColor() &&
		!isPropertyValid() && stateProperty()->isEditableByUser())
	{
		auto palette = painter.style()->standardPalette();
		if (palette.currentColorGroup() != QPalette::Disabled &&
			oldPen.color() != palette.color(QPalette::HighlightedText))
		{
			painter.setPen(m_invalidColor.rgb());
		}
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
	QtnPropertyQStringMultilineEditBttnHandler(
		QtnPropertyDelegate *delegate, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(delegate, editor)
	, dialog(new MultilineTextDialog(&editor))
	, multiline(false)
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

		edit->setPlaceholderText(
			QtnPropertyQString::getPlaceholderStr(text, true));
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
	QtnPropertyDelegate *delegate, QLineEdit &editor)
	: QtnPropertyEditorHandler(delegate, editor)
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
		editor().setText(property().value());
		editor().setPlaceholderText(
			QtnPropertyQString::getPlaceholderStr(editor().text(), false));
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
		QtnPropertyDelegateQStringCallback *delegate, QtnLineEditBttn &editor,
		const QtnPropertyDelegateInfo &info);

private:
	void updateCandidates();

	void updateEditor() override;

	void createNewCandidate(QString candidate);

	void onToolButtonClicked(bool checked);

	void onEditingFinished();

private:
	QtnGetCandidatesFn m_getCandidatesFn;
	QtnCreateCandidateFn m_createCandidateFn;

	QStringList m_candidates;
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
	auto editor =
		new QtnLineEditBttn(parent, QString(), new QtnCompleterLineEdit);
	editor->setGeometry(rect);

	new QtnPropertyQStringCandidatesComboBoxHandler(
		this, *editor, m_editorAttributes);

	qtnInitLineEdit(editor->lineEdit, inplaceInfo);

	return editor;
}

QtnPropertyQStringCandidatesComboBoxHandler::
	QtnPropertyQStringCandidatesComboBoxHandler(
		QtnPropertyDelegateQStringCallback *delegate, QtnLineEditBttn &editor,
		const QtnPropertyDelegateInfo &info)
	: QtnPropertyEditorHandlerVT(delegate, editor)
{
	auto model = new QStringListModel(editor.lineEdit);
	auto completer = editor.lineEdit->completer();
	Q_ASSERT(completer);
	completer->setModel(model);

	info.loadAttribute(qtnGetCandidatesFnAttr(), m_getCandidatesFn);
	info.loadAttribute(qtnCreateCandidateFnAttr(), m_createCandidateFn);

	{
		QString createCandidateLabel(QChar('*'));
		info.loadAttribute(qtnCreateCandidateLabelAttr(), createCandidateLabel);
		editor.toolButton->setText(createCandidateLabel);
	}

	editor.installEventFilter(this);

	updateCandidates();
	updateEditor();

	QObject::connect(editor.lineEdit, &QLineEdit::editingFinished, this,
		&QtnPropertyQStringCandidatesComboBoxHandler::onEditingFinished);

	if (m_createCandidateFn)
	{
		QObject::connect(editor.toolButton, &QToolButton::clicked, this,
			&QtnPropertyQStringCandidatesComboBoxHandler::onToolButtonClicked);
	} else
	{
		editor.toolButton->setVisible(false);
	}
}

void QtnPropertyQStringCandidatesComboBoxHandler::updateCandidates()
{
	updating++;

	if (m_getCandidatesFn)
		m_candidates = m_getCandidatesFn();

	auto lineEdit = editor().lineEdit;
	auto completer = lineEdit->completer();
	Q_ASSERT(completer);
	Q_ASSERT(qobject_cast<QStringListModel *>(completer->model()));
	auto model = static_cast<QStringListModel *>(completer->model());
	model->setStringList(m_candidates);

	if (stateProperty()->isEditableByUser())
	{
		QMetaObject::invokeMethod(completer, "complete", Qt::QueuedConnection);
	}

	updating--;
}

void QtnPropertyQStringCandidatesComboBoxHandler::updateEditor()
{
	updating++;

	bool enabled = stateProperty()->isEditableByUser();

	editor().toolButton->setEnabled(enabled);

	auto lineEdit = editor().lineEdit;
	lineEdit->setReadOnly(!enabled);
	if (stateProperty()->isMultiValue())
	{
		lineEdit->clear();
		lineEdit->setPlaceholderText(
			QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		lineEdit->setText(property());
		lineEdit->setPlaceholderText(
			QtnPropertyQString::getPlaceholderStr(lineEdit->text(), false));
	}

	lineEdit->completer()->setCompletionPrefix(lineEdit->text());
	if (lineEdit)
		lineEdit->selectAll();

	updating--;
}

void QtnPropertyQStringCandidatesComboBoxHandler::createNewCandidate(
	QString candidate)
{
	if (!m_createCandidateFn || updating)
		return;

	qtnRetainInplaceEditor();
	updating++;

	candidate = m_createCandidateFn(&editor(), candidate);
	if (candidate.isEmpty())
	{
		qtnReleaseInplaceEditor();
		return;
	}

	updating--;
	onValueChanged(candidate);

	updateCandidates();

	qtnReleaseInplaceEditor();
}

void QtnPropertyQStringCandidatesComboBoxHandler::onToolButtonClicked(
	bool checked)
{
	Q_UNUSED(checked);
	createNewCandidate(QString());
}

void QtnPropertyQStringCandidatesComboBoxHandler::onEditingFinished()
{
	if (canApply())
	{
		auto text = toSingleLine(editor().lineEdit->text());

		if (text.isEmpty() || m_candidates.contains(text, Qt::CaseInsensitive))
			property().setValue(text, delegate()->editReason());
		else
			createNewCandidate(text);
	}

	applyReset();
}
