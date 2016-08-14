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

#include "PropertyEditorHandler.h"

#include <QDebug>
#include <QKeyEvent>

QtnPropertyEditorHandlerBase::QtnPropertyEditorHandlerBase(QtnProperty& property,
														   QWidget& editor)
	: reverted(false)
	, returned(false)
{
	setParent(&editor);
	QObject::connect(&property, &QtnPropertyBase::propertyDidChange,
					 this, &QtnPropertyEditorHandlerBase::onPropertyDidChange, Qt::QueuedConnection);
	QObject::connect(&property, &QObject::destroyed,
					 this, &QtnPropertyEditorHandlerBase::onPropertyDestroyed);
}

void QtnPropertyEditorHandlerBase::revertInput()
{
	reverted = true;
	updateEditor();
}

bool QtnPropertyEditorHandlerBase::eventFilter(QObject *obj, QEvent *event)
{
	switch (event->type())
	{
		case QEvent::KeyPress:
		{
			auto keyEvent = static_cast<QKeyEvent*>(event);
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

		}	break;

		default:
			break;
	}

	return QObject::eventFilter(obj, event);
}

bool QtnPropertyEditorHandlerBase::canApply() const
{
	auto thiz = const_cast<QtnPropertyEditorHandlerBase *>(this);
	auto property = thiz->propertyBase();
	if (nullptr != property)
		return (!reverted && (returned || !property->valueIsHidden()));

	return false;
}

void QtnPropertyEditorHandlerBase::applyReset()
{
	reverted = false;
	returned = false;
}

void QtnPropertyEditorHandlerBase::onPropertyDestroyed()
{
	propertyBase() = nullptr;
}

void QtnPropertyEditorHandlerBase::onPropertyDidChange(const QtnPropertyBase *,
													   const QtnPropertyBase *firedProperty,
													   QtnPropertyChangeReason reason)
{
	if (0 != (reason & (QtnPropertyChangeReasonValue | QtnPropertyChangeReasonState))
	&&	propertyBase() == firedProperty)
	{
        updateEditor();
	}
}
