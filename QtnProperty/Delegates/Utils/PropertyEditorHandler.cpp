/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
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

#include "PropertyEditorHandler.h"
#include "QtnProperty/Utils/InplaceEditing.h"
#include "QtnProperty/Utils/QtnConnections.h"

#include <QDebug>
#include <QKeyEvent>
#include <QDialog>

QtnPropertyEditorHandlerBase::QtnPropertyEditorHandlerBase(
	QtnPropertyDelegate *delegate, QWidget &editor)
	: QObject(&editor)
	, reverted(false)
	, returned(false)
	, m_delegate(delegate)
	, m_editor(&editor)
{
	Q_ASSERT(delegate);
	Q_ASSERT(!delegate->m_editorHandler);
	delegate->m_editorHandler = this;
	auto property = delegate->property();
	Q_ASSERT(property);
	QObject::connect(property, &QtnPropertyBase::propertyDidChange, this,
		&QtnPropertyEditorHandlerBase::onPropertyDidChange,
		Qt::QueuedConnection);
	QObject::connect(property, &QObject::destroyed, this,
		&QtnPropertyEditorHandlerBase::onPropertyDestroyed);

	auto stateProperty = delegate->stateProperty();
	if (stateProperty != property)
	{
		QObject::connect(stateProperty, &QtnPropertyBase::propertyDidChange,
			this, &QtnPropertyEditorHandlerBase::onPropertyDidChange,
			Qt::QueuedConnection);
	}
}

void QtnPropertyEditorHandlerBase::revertInput()
{
	reverted = true;
	updateEditor();
}

void QtnPropertyEditorHandlerBase::cleanup()
{
	if (m_delegate)
	{
		m_delegate->m_editorHandler = nullptr;
		m_delegate = nullptr;
	}
	if (m_editor)
		m_editor->removeEventFilter(this);
	qtnStopInplaceEdit();
}

QtnPropertyEditorHandlerBase::~QtnPropertyEditorHandlerBase()
{
	cleanup();
}

bool QtnPropertyEditorHandlerBase::eventFilter(QObject *obj, QEvent *event)
{
	switch (event->type())
	{
		case QEvent::KeyPress:
		{
			auto keyEvent = static_cast<QKeyEvent *>(event);

			// revert all changes
			switch (keyEvent->key())
			{
				case Qt::Key_Escape:
					revertInput();
					break;

				case Qt::Key_Enter:
				case Qt::Key_Return:
					returned = true;
					break;

				default:
					break;
			}
		}
		break;

		default:
			break;
	}

	return QObject::eventFilter(obj, event);
}

bool QtnPropertyEditorHandlerBase::canApply() const
{
	if (nullptr != stateProperty() && stateProperty()->isEditableByUser())
		return (!reverted && (returned || !stateProperty()->isMultiValue()));

	return false;
}

void QtnPropertyEditorHandlerBase::applyReset()
{
	reverted = false;
	returned = false;
}

QtnPropertyEditorHandlerBase::DialogContainerPtr
QtnPropertyEditorHandlerBase::connectDialog(QDialog *dialog)
{
	DialogContainerPtr result(new DialogContainer(dialog));
	connectDialog(result);
	return result;
}

void QtnPropertyEditorHandlerBase::connectDialog(
	const DialogContainerPtr &containerPtr)
{
	Q_ASSERT(nullptr != containerPtr);
	auto dialog = containerPtr->dialog;
	Q_ASSERT(nullptr != dialog);

	auto parent = dialog->parent();
	Q_ASSERT(nullptr != parent);

	QObject::connect(parent, &QObject::destroyed,
		[containerPtr]() { containerPtr->dialog->setParent(nullptr); });
}

void QtnPropertyEditorHandlerBase::onPropertyDestroyed()
{
	cleanup();
}

void QtnPropertyEditorHandlerBase::onPropertyDidChange(
	QtnPropertyChangeReason reason)
{
	if (reason & (QtnPropertyChangeReasonValue | QtnPropertyChangeReasonState))
	{
		if (propertyBase() == sender() || stateProperty() == sender())
			updateEditor();
	}
}

QtnPropertyEditorHandlerBase::DialogContainer::DialogContainer(QDialog *dialog)
	: dialog(dialog)
{
}

QtnPropertyEditorHandlerBase::DialogContainer::~DialogContainer()
{
	if (nullptr != dialog && nullptr == dialog->parent())
		delete dialog;
}
