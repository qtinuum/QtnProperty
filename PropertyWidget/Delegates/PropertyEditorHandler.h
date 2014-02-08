/*
 * Copyright (c) 2012 - 2014, the Qtinuum project.
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

class QTN_PW_EXPORT QtnPropertyEditorHandlerBase: public QObject
{
protected:
    QtnPropertyEditorHandlerBase(QtnProperty& property, QWidget& editor);
    ~QtnPropertyEditorHandlerBase();

    virtual QtnProperty& propertyBase() = 0;
    virtual QWidget& editorBase()  = 0;
    virtual void updateEditor() = 0;

private:
    void onObjectDestroyed(QObject* object);
    void onPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);
};

template <typename PropertyClass, typename PropertyEditorClass>
class QtnPropertyEditorHandler: public QtnPropertyEditorHandlerBase
{
protected:
    typedef QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass> QtnPropertyEditorHandlerType;

    QtnPropertyEditorHandler(PropertyClass& property, PropertyEditorClass& editor)
        : QtnPropertyEditorHandlerBase(property, editor),
          m_property(property),
          m_editor(editor)
    {
    }

    ~QtnPropertyEditorHandler()
    {
    }

    PropertyClass& property() { return m_property;  }
    PropertyEditorClass& editor() { return m_editor; }

    QtnProperty& propertyBase() override { return property(); }
    QWidget& editorBase() override {return editor(); }

private:
    PropertyClass& m_property;
    PropertyEditorClass& m_editor;
};

#endif // PROPERTY_EDITOR_HANDLER_H
