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

#ifndef PROPERTY_EDITOR_HANDLER_H
#define PROPERTY_EDITOR_HANDLER_H

#include "../PropertyWidgetAPI.h"
#include "../../Core/Property.h"
#include <QWidget>

namespace Qtinuum
{

class QTN_PW_EXPORT PropertyEditorHandlerBase: public QObject
{
protected:
    PropertyEditorHandlerBase(Property& property, QWidget& editor);
    ~PropertyEditorHandlerBase();

    virtual Property& propertyBase() = 0;
    virtual QWidget& editorBase()  = 0;
    virtual void updateEditor() = 0;

private:
    void onObjectDestroyed(QObject* object);
    void onPropertyDidChange(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason);
};

template <typename PropertyClass, typename PropertyEditorClass>
class PropertyEditorHandler: public PropertyEditorHandlerBase
{
protected:
    typedef PropertyEditorHandler<PropertyClass, PropertyEditorClass> PropertyEditorHandlerType;

    PropertyEditorHandler(PropertyClass& property, PropertyEditorClass& editor)
        : PropertyEditorHandlerBase(property, editor),
          m_property(property),
          m_editor(editor)
    {
    }

    ~PropertyEditorHandler()
    {
    }

    PropertyClass& property() { return m_property;  }
    PropertyEditorClass& editor() { return m_editor; }

    Property& propertyBase() override { return property(); }
    QWidget& editorBase() override {return editor(); }

private:
    PropertyClass& m_property;
    PropertyEditorClass& m_editor;
};

} // end namespace Qtinuum

#endif // PROPERTY_EDITOR_HANDLER_H
