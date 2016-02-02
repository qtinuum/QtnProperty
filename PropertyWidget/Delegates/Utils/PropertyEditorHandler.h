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

#ifndef PROPERTY_EDITOR_HANDLER_H
#define PROPERTY_EDITOR_HANDLER_H

#include "../../PropertyWidgetAPI.h"
#include "../../../Core/Property.h"
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
