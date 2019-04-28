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

#include "PropertyEditorHandler.h"

#include <QDebug>

QtnPropertyEditorHandlerBase::QtnPropertyEditorHandlerBase(QtnProperty& property, QWidget& editor)
{
    QObject::connect(&editor, &QObject::destroyed, this, &QtnPropertyEditorHandlerBase::onObjectDestroyed);
    QObject::connect(&property, &QObject::destroyed, this, &QtnPropertyEditorHandlerBase::onObjectDestroyed);
    QObject::connect(&property, &QtnPropertyBase::propertyDidChange, this, &QtnPropertyEditorHandlerBase::onPropertyDidChange, Qt::QueuedConnection);
}

QtnPropertyEditorHandlerBase::~QtnPropertyEditorHandlerBase()
{
}

void QtnPropertyEditorHandlerBase::onObjectDestroyed(QObject *object)
{
    Q_UNUSED(object);
    Q_ASSERT((object == &propertyBase()) || (object == &editorBase()));
    delete this;
}

void QtnPropertyEditorHandlerBase::onPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
    Q_UNUSED(changedProperty);

    if ((reason & QtnPropertyChangeReasonValue) && (&propertyBase() == firedProperty))
        updateEditor();
}
