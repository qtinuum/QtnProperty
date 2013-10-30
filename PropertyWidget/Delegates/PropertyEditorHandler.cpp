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

namespace Qtinuum
{

PropertyEditorHandlerBase::PropertyEditorHandlerBase(Property& property, QWidget& editor)
{
    QObject::connect(&editor, &QObject::destroyed, this, &PropertyEditorHandlerBase::onObjectDestroyed);
    QObject::connect(&property, &QObject::destroyed, this, &PropertyEditorHandlerBase::onObjectDestroyed);
    QObject::connect(&property, &Property::propertyDidChange, this, &PropertyEditorHandlerBase::onPropertyDidChange, Qt::QueuedConnection);
    qDebug() << "Create editor handler: " << this;
}

PropertyEditorHandlerBase::~PropertyEditorHandlerBase()
{
    qDebug() << "Delete editor handler: " << this;
}

void PropertyEditorHandlerBase::onObjectDestroyed(QObject *object)
{
    Q_ASSERT((object == &propertyBase()) || (object == &editorBase()));
    delete this;
}

void PropertyEditorHandlerBase::onPropertyDidChange(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason)
{
    if ((reason == PropertyChangeReasonValue) && (&propertyBase() == firedProperty))
        updateEditor();
}

} // end namespace Qtinuum

