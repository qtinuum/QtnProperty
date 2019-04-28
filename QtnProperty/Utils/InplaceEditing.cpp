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

#include "InplaceEditing.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

class QtnInplaceEditorHandler: public QObject
{
public:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void OnEditorDestroyed(QObject* obj);
};

static QWidget* g_inplaceEditor = 0;
static QtnInplaceEditorHandler* g_inplaceEditorHandler = 0;

bool qtnStartInplaceEdit(QWidget* editor)
{
    if (!editor)
        return false;

    if (g_inplaceEditor)
    {
        Q_ASSERT(false);
        qtnStopInplaceEdit();
    }

    QCoreApplication* app = QCoreApplication::instance();
    if (!app)
    {
        Q_ASSERT(false);
        return false;
    }

    g_inplaceEditor = editor;
    g_inplaceEditorHandler = new QtnInplaceEditorHandler();

    // move focus to editor
    if (QApplication::focusWidget() != g_inplaceEditor->focusWidget())
        g_inplaceEditor->setFocus();

    // connect to editor destroyed signal
    QObject::connect(  g_inplaceEditor, &QObject::destroyed
                     , g_inplaceEditorHandler, &QtnInplaceEditorHandler::OnEditorDestroyed);

    // install application event filter
    app->installEventFilter(g_inplaceEditorHandler);

    return true;
}

QWidget* qtnGetInplaceEdit()
{
    return g_inplaceEditor;
}


void onInplaceWidgetDestroyed(QObject* object)
{
    // set focus to parent of inplace widget
    QWidget* parent = qobject_cast<QWidget*>(object->parent());
    if (parent)
        parent->setFocus();
}

bool qtnStopInplaceEdit()
{
    if (!g_inplaceEditor)
        return false;

    delete g_inplaceEditorHandler;
    g_inplaceEditorHandler = 0;

    QObject::connect(g_inplaceEditor, &QObject::destroyed, &onInplaceWidgetDestroyed);

    g_inplaceEditor->deleteLater();
    g_inplaceEditor = 0;

    return true;
}

bool hasParent(QObject* child, QObject* parent)
{
    if (!child)
        return false;

    if (child == parent)
        return true;

    return hasParent(child->parent(), parent);
}

bool QtnInplaceEditorHandler::eventFilter(QObject* watched, QEvent* event)
{
    Q_ASSERT(g_inplaceEditor);

    if (!event)
        return false;

    // try handle by base class
    if (QObject::eventFilter(watched, event))
        return true;

    if (event->type() == QEvent::FocusIn)
    {
        if (!hasParent(QApplication::focusObject(), g_inplaceEditor))
            qtnStopInplaceEdit();

        return false;
    }

    return false;
}

void QtnInplaceEditorHandler::OnEditorDestroyed(QObject* obj)
{
    Q_UNUSED(obj);
    Q_ASSERT(obj == g_inplaceEditor);

    delete g_inplaceEditorHandler;
    g_inplaceEditorHandler = 0;
    g_inplaceEditor = 0;
}

