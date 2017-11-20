/*******************************************************************************
Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
#include "Core/PropertyQString.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorHandler.h"
#include "Delegates/PropertyEditorAux.h"
#include "Utils/MultilineTextDialog.h"
#include "MultiProperty.h"
#include "PropertyDelegateAttrs.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QFileInfo>
#include <QFileDialog>
#include <QComboBox>
#include <QDebug>

QByteArray qtnMultiLineEditAttr()
{
	returnQByteArrayLiteral("multiline_edit");
}

QByteArray qtnMaxLengthAttr()
{
	returnQByteArrayLiteral("max_length");
}

QByteArray qtnItemsAttr()
{
	returnQByteArrayLiteral("items");
}

QByteArray qtnEditableAttr()
{
	returnQByteArrayLiteral("editable");
}

QByteArray qtnInvalidColorAttr()
{
	returnQByteArrayLiteral("invalidColor");
}

QByteArray qtnShowRelativePathAttr()
{
	returnQByteArrayLiteral("showRelativePath");
}

QByteArray qtnFileNameFilterAttr()
{
	returnQByteArrayLiteral("nameFilter");
}

QByteArray qtnFileNameFiltersAttr()
{
	returnQByteArrayLiteral("nameFilters");
}

QByteArray qtnDefaultSuffixAttr()
{
	returnQByteArrayLiteral("defaultSuffix");
}

QByteArray qtnDefaultDirAttr()
{
	returnQByteArrayLiteral("defaultDirectory");
}

QByteArray qtnOptionsAttr()
{
	returnQByteArrayLiteral("options");
}

QByteArray qtnFileModeAttr()
{
	returnQByteArrayLiteral("fileMode");
}

QByteArray qtnViewModeAttr()
{
	returnQByteArrayLiteral("viewMode");
}

QByteArray qtnAcceptModeAttr()
{
	returnQByteArrayLiteral("acceptMode");
}

QByteArray qtnLineEditDelegate()
{
	returnQByteArrayLiteral("LineEdit");
}

QByteArray qtnSelectEditDelegate()
{
	returnQByteArrayLiteral("SelectEdit");
}

QByteArray qtnComboBoxDelegate()
{
	returnQByteArrayLiteral("ComboBox");
}

class QtnPropertyQStringLineEditHandler
	: public QtnPropertyEditorHandler<QtnPropertyQStringBase, QLineEdit>
{
public:
	QtnPropertyQStringLineEditHandler(
		QtnPropertyQStringBase &property, QLineEdit &editor);

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
		QtnPropertyQStringBase &property, QtnLineEditBttn &editor);

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
	, maxLength(0x1000000)
{
	owner.setMultilineEnabled(true);
}

bool QtnPropertyDelegateQString::Register()
{
	return QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
		&QtnPropertyQStringBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQString, QtnPropertyQStringBase>,
		qtnLineEditDelegate());
}

void QtnPropertyDelegateQString::applyAttributesImpl(
	const QtnPropertyDelegateAttributes &attributes)
{
	bool check_multiline;
	qtnGetAttribute(attributes, qtnMultiLineEditAttr(), check_multiline);
	qtnGetAttribute(attributes, qtnMaxLengthAttr(), maxLength);
	owner().setMultilineEnabled(check_multiline);
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
	if (owner().isMultilineEnabled())
	{
		QtnLineEditBttn *editor = new QtnLineEditBttn(parent);
		editor->setGeometry(rect);

		editor->lineEdit->setMaxLength(maxLength);

		new QtnPropertyQStringMultilineEditBttnHandler(owner(), *editor);

		qtnInitLineEdit(editor->lineEdit, inplaceInfo);
		return editor;
	}

	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setMaxLength(maxLength);

	lineEdit->setGeometry(rect);

	new QtnPropertyQStringLineEditHandler(owner(), *lineEdit);

	qtnInitLineEdit(lineEdit, inplaceInfo);

	return lineEdit;
}

bool QtnPropertyDelegateQString::propertyValueToStr(QString &strValue) const
{
	strValue = owner().value();
	auto placeholder = QtnPropertyQString::getPlaceholderStr(
		strValue, owner().isMultilineEnabled());

	if (!placeholder.isEmpty())
		strValue.swap(placeholder);

	return true;
}

void QtnPropertyDelegateQString::drawValueImpl(QStylePainter &painter,
	const QRect &rect, const QStyle::State &state, bool *needTooltip) const
{
	QPen oldPen = painter.pen();

	if (!owner().getPlaceholderText().isEmpty())
		painter.setPen(Qt::darkGray);

	Inherited::drawValueImpl(painter, rect, state, needTooltip);
	painter.setPen(oldPen);
}

class QtnPropertyQStringFileLineEditBttnHandler
	: public QtnPropertyEditorBttnHandler<QtnPropertyQStringBase,
		  QtnLineEditBttn>
{
public:
	QtnPropertyQStringFileLineEditBttnHandler(
		QtnPropertyQStringBase &property, QtnLineEditBttn &editor);

	void applyAttributes(const QtnPropertyDelegateAttributes &attributes);

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
	owner.setMultilineEnabled(false);
}

void QtnPropertyDelegateQStringInvalidBase::applyAttributesImpl(
	const QtnPropertyDelegateAttributes &attributes)
{
	qtnGetAttribute(attributes, qtnInvalidColorAttr(), m_invalidColor);
}

void QtnPropertyDelegateQStringInvalidBase::drawValueImpl(
	QStylePainter &painter, const QRect &rect, const QStyle::State &state,
	bool *needTooltip) const
{
	QPen oldPen = painter.pen();

	if ((m_invalidColor.alpha() != 0) && !isPropertyValid())
		painter.setPen(m_invalidColor);

	QtnPropertyDelegateQString::drawValueImpl(
		painter, rect, state, needTooltip);
	painter.setPen(oldPen);
}

QtnPropertyDelegateQStringFile::QtnPropertyDelegateQStringFile(
	QtnPropertyQStringBase &owner)
	: QtnPropertyDelegateQStringInvalidBase(owner)
{
}

bool QtnPropertyDelegateQStringFile::Register()
{
	return QtnPropertyDelegateFactory::staticInstance().registerDelegate(
		&QtnPropertyQStringBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQStringFile,
			QtnPropertyQStringBase>,
		qtnSelectEditDelegate());
}

void QtnPropertyDelegateQStringFile::applyAttributesImpl(
	const QtnPropertyDelegateAttributes &attributes)
{
	QtnPropertyDelegateQStringInvalidBase::applyAttributesImpl(attributes);
	m_editorAttributes = attributes;
}

bool QtnPropertyDelegateQStringFile::propertyValueToStr(QString &strValue) const
{
	if (!owner().value().isEmpty())
	{
		strValue = QDir::toNativeSeparators(
			shouldShowRelativePath() ? relativeFilePath() : absoluteFilePath());
		return true;
	}

	return QtnPropertyDelegateQStringInvalidBase::propertyValueToStr(strValue);
}

QString QtnPropertyDelegateQStringFile::toolTipImpl() const
{
	return QDir::toNativeSeparators(absoluteFilePath());
}

void QtnPropertyDelegateQStringFile::drawValueImpl(QStylePainter &painter,
	const QRect &rect, const QStyle::State &state, bool *needToolTip) const
{
	QtnPropertyDelegateQStringInvalidBase::drawValueImpl(
		painter, rect, state, needToolTip);

	if (needToolTip && shouldShowRelativePath())
	{
		*needToolTip = !owner().value().isEmpty();
	}
}

QWidget *QtnPropertyDelegateQStringFile::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QtnLineEditBttn *editor = new QtnLineEditBttn(parent);
	editor->setGeometry(rect);

	auto handler =
		new QtnPropertyQStringFileLineEditBttnHandler(owner(), *editor);
	handler->applyAttributes(m_editorAttributes);

	qtnInitLineEdit(editor->lineEdit, inplaceInfo);

	return editor;
}

bool QtnPropertyDelegateQStringFile::isPropertyValid() const
{
	auto filePath = absoluteFilePath();

	if (filePath.isEmpty())
		return false;

	int fileMode = QFileDialog::AnyFile;

	if (qtnGetAttribute(m_editorAttributes, qtnFileModeAttr(), fileMode))
		fileMode = QFileDialog::FileMode(fileMode);

	switch (fileMode)
	{
		case QFileDialog::AnyFile:
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
	auto it = m_editorAttributes.find(qtnDefaultDirAttr());
	if (it == m_editorAttributes.end())
		return QString();

	if (it.value().type() != QVariant::String)
		return QString();

	return it.value().toString();
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

	auto it = m_editorAttributes.find(qtnShowRelativePathAttr());
	if (it == m_editorAttributes.end())
		return false;

	return it.value().type() == QVariant::Bool && it.value().toBool();
}

class QtnPropertyQStringListComboBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyQStringBase, QComboBox>
{
public:
	QtnPropertyQStringListComboBoxHandler(
		QtnPropertyQStringBase &property, QComboBox &editor);

	void applyAttributes(const QtnPropertyDelegateAttributes &attributes);

private:
	virtual void updateEditor() override;
	virtual void updateValue() override;
};

QtnPropertyDelegateQStringList::QtnPropertyDelegateQStringList(
	QtnPropertyQStringBase &owner)
	: QtnPropertyDelegateQString(owner)
{
}

bool QtnPropertyDelegateQStringList::Register()
{
	return QtnPropertyDelegateFactory::staticInstance().registerDelegate(
		&QtnPropertyQStringBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQStringList,
			QtnPropertyQStringBase>,
		qtnComboBoxDelegate());
}

void QtnPropertyDelegateQStringList::applyAttributesImpl(
	const QtnPropertyDelegateAttributes &attributes)
{
	qtnGetAttribute(attributes, qtnItemsAttr(), m_items);
	m_editorAttributes = attributes;
}

QWidget *QtnPropertyDelegateQStringList::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	if (!owner().isEditableByUser())
	{
		return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
	}

	QComboBox *editor = new QComboBox(parent);
	editor->setGeometry(rect);

	editor->addItems(m_items);

	auto handler = new QtnPropertyQStringListComboBoxHandler(owner(), *editor);
	handler->applyAttributes(m_editorAttributes);

	if (inplaceInfo)
	{
		editor->showPopup();
	}

	return editor;
}

QtnPropertyQStringListComboBoxHandler::QtnPropertyQStringListComboBoxHandler(
	QtnPropertyQStringBase &property, QComboBox &editor)
	: QtnPropertyEditorHandlerVT(property, editor)
{
	updateEditor();

	if (!property.isEditableByUser())
		editor.setDisabled(true);

	QObject::connect(&editor, &QComboBox::currentTextChanged, this,
		&QtnPropertyQStringListComboBoxHandler::onValueChanged);
}

void QtnPropertyQStringListComboBoxHandler::applyAttributes(
	const QtnPropertyDelegateAttributes &attributes)
{
	bool editable = false;
	qtnGetAttribute(attributes, qtnEditableAttr(), editable);

	editor().setEditable(editable);
	editor().setAutoCompletion(false);

	if (editable)
		editor().installEventFilter(this);
}

void QtnPropertyQStringListComboBoxHandler::updateEditor()
{
	updating++;

	if (property().valueIsHidden())
		editor().clearEditText();
	else
		editor().setCurrentText(property());

	if (editor().isEditable())
		editor().lineEdit()->selectAll();

	updating--;
}

void QtnPropertyQStringListComboBoxHandler::updateValue()
{
	if (!editor().isEditable() || canApply())
		property().edit(newValue);

	applyReset();
}

QtnPropertyQStringFileLineEditBttnHandler::
	QtnPropertyQStringFileLineEditBttnHandler(
		QtnPropertyQStringBase &property, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(property, editor)
	, dialog(new QFileDialog(&editor))
{
	dialogContainer = connectDialog(dialog);

	if (!property.isEditableByUser())
	{
		editor.lineEdit->setReadOnly(true);
		editor.toolButton->setEnabled(false);
	}

	updateEditor();

	editor.lineEdit->installEventFilter(this);
	QObject::connect(editor.toolButton, &QToolButton::clicked, this,
		&QtnPropertyQStringFileLineEditBttnHandler::onToolButtonClicked);
	QObject::connect(editor.lineEdit, &QLineEdit::editingFinished, this,
		&QtnPropertyQStringFileLineEditBttnHandler::onEditingFinished);
}

void QtnPropertyQStringFileLineEditBttnHandler::applyAttributes(
	const QtnPropertyDelegateAttributes &attributes)
{
	int option = 0;

	if (qtnGetAttribute(attributes, qtnAcceptModeAttr(), option))
		dialog->setAcceptMode(QFileDialog::AcceptMode(option));

	QString str;

	if (qtnGetAttribute(attributes, qtnDefaultSuffixAttr(), str))
		dialog->setDefaultSuffix(str);

	if (qtnGetAttribute(attributes, qtnDefaultDirAttr(), str))
		defaultDirectory = str;

	if (qtnGetAttribute(attributes, qtnFileModeAttr(), option))
		dialog->setFileMode(QFileDialog::FileMode(option));

	if (qtnGetAttribute(attributes, qtnOptionsAttr(), option))
		dialog->setOptions(QFileDialog::Options(QFlag(option)));

	if (qtnGetAttribute(attributes, qtnViewModeAttr(), option))
		dialog->setViewMode(QFileDialog::ViewMode(option));

	if (qtnGetAttribute(attributes, qtnFileNameFilterAttr(), str))
		dialog->setNameFilter(str);

	QStringList list;

	if (qtnGetAttribute(attributes, qtnFileNameFiltersAttr(), list))
		dialog->setNameFilters(list);
}

void QtnPropertyQStringFileLineEditBttnHandler::onToolButtonClick()
{
	onToolButtonClicked(false);
}

void QtnPropertyQStringFileLineEditBttnHandler::updateEditor()
{
	auto path = QDir::toNativeSeparators(property().value());
	editor().setTextForProperty(&property(), path);

	if (!property().valueIsHidden())
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
			property->edit(QDir::toNativeSeparators(files.first()));
	}

	if (!destroyed)
		QObject::disconnect(connection);

	Q_UNUSED(dialogContainer);
}

void QtnPropertyQStringFileLineEditBttnHandler::onEditingFinished()
{
	if (canApply())
		property().edit(editor().lineEdit->text());

	applyReset();
}

QtnPropertyQStringMultilineEditBttnHandler::
	QtnPropertyQStringMultilineEditBttnHandler(
		QtnPropertyQStringBase &property, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(property, editor)
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
	edit->setReadOnly(!property().isEditableByUser());

	if (property().valueIsHidden())
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
			property().edit(text);
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
	bool readonly = !property->isEditableByUser();
	auto dialogContainer = this->dialogContainer;
	dialog->setReadOnly(readonly);

	if (readonly)
	{
		dialog->setWindowTitle(
			QtnPropertyQString::getReadOnlyPropertyTitleFormat().arg(
				property->name()));
	} else
	{
		dialog->setWindowTitle(property->name());
	}

	dialog->setText(text);
	dialog->show();
	dialog->raise();
	volatile bool destroyed = false;
	auto connection = QObject::connect(this, &QObject::destroyed,
		[&destroyed]() mutable { destroyed = true; });

	if (dialog->exec() == QDialog::Accepted && !destroyed)
		property->edit(dialog->getText());

	if (!destroyed)
	{
		QObject::disconnect(connection);
		updateEditor();
	}

	Q_UNUSED(dialogContainer);
}

QtnPropertyQStringLineEditHandler::QtnPropertyQStringLineEditHandler(
	QtnPropertyQStringBase &property, QLineEdit &editor)
	: QtnPropertyEditorHandler(property, editor)
{
	updateEditor();

	if (!property.isEditableByUser())
		editor.setReadOnly(true);

	editor.installEventFilter(this);
	QObject::connect(&editor, &QLineEdit::editingFinished, this,
		&QtnPropertyQStringLineEditHandler::updateValue);
}

void QtnPropertyQStringLineEditHandler::updateEditor()
{
	if (property().valueIsHidden())
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
		property().edit(editor().text());

	applyReset();
}
