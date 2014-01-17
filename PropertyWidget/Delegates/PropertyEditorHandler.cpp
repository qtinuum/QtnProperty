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

#include "PropertyEditorHandler.h"

#include <QDebug>

QtnPropertyEditorHandlerBase::QtnPropertyEditorHandlerBase(QtnProperty& property, QWidget& editor)
{
    QObject::connect(&editor, &QObject::destroyed, this, &QtnPropertyEditorHandlerBase::onObjectDestroyed);
    QObject::connect(&property, &QObject::destroyed, this, &QtnPropertyEditorHandlerBase::onObjectDestroyed);
    QObject::connect(&property, &QtnPropertyBase::propertyDidChange, this, &QtnPropertyEditorHandlerBase::onPropertyDidChange, Qt::QueuedConnection);
    qDebug() << "Create editor handler: " << this;
}

QtnPropertyEditorHandlerBase::~QtnPropertyEditorHandlerBase()
{
    qDebug() << "Delete editor handler: " << this;
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
