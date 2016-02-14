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

QtnPropertyEditorHandlerBase::QtnPropertyEditorHandlerBase(QtnProperty& property, QWidget& editor)
{
    QObject::connect(&editor, &QObject::destroyed, this, &QtnPropertyEditorHandlerBase::onObjectDestroyed);
    QObject::connect(&property, &QObject::destroyed, this, &QtnPropertyEditorHandlerBase::onObjectDestroyed);
	QObject::connect(&property, &QtnPropertyBase::propertyDidChange, this, &QtnPropertyEditorHandlerBase::onPropertyDidChange, Qt::QueuedConnection);
}

void QtnPropertyEditorHandlerBase::revertInput()
{
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
			if (keyEvent->key() == Qt::Key_Escape)
				revertInput();

		}	break;

		default:
			break;
	}

	return QObject::eventFilter(obj, event);
}

void QtnPropertyEditorHandlerBase::onObjectDestroyed(QObject *object)
{
	Q_ASSERT((object == &propertyBase()) || (object == &editorBase()));
	delete this;
}

void QtnPropertyEditorHandlerBase::onPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
	if ((reason & QtnPropertyChangeReasonValue) && (&propertyBase() == firedProperty))
        updateEditor();
}
