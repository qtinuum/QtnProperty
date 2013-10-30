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

#include "InplaceEditing.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

namespace Qtinuum
{

class InplaceEditorHandler: public QObject
{
public:

    bool eventFilter(QObject* watched, QEvent* event) override;

    void OnEditorDestroyed(QObject* obj);
};

static QWidget* g_inplaceEditor = 0;
static InplaceEditorHandler* g_inplaceEditorHandler = 0;

bool startInplaceEdit(QWidget* editor)
{
    if (!editor)
        return false;

    if (g_inplaceEditor)
    {
        Q_ASSERT(false);
        stopInplaceEdit();
    }

    QCoreApplication* app = QCoreApplication::instance();
    if (!app)
    {
        Q_ASSERT(false);
        return false;
    }

    g_inplaceEditor = editor;
    g_inplaceEditorHandler = new InplaceEditorHandler();

    // move focus to editor
    if (QApplication::focusWidget() != g_inplaceEditor->focusWidget())
        g_inplaceEditor->setFocus();

    // connect to editor destroyed signal
    QObject::connect(  g_inplaceEditor, &QObject::destroyed
                     , g_inplaceEditorHandler, &InplaceEditorHandler::OnEditorDestroyed);

    // install application event filter
    app->installEventFilter(g_inplaceEditorHandler);

    return true;
}

QWidget* getInplaceEdit()
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

bool stopInplaceEdit()
{
    if (!g_inplaceEditor)
        return false;

    delete g_inplaceEditorHandler;
    g_inplaceEditorHandler = 0;

    QObject::connect(g_inplaceEditor, &QObject::destroyed, &onInplaceWidgetDestroyed);

//    delete g_inplaceEditor;
    g_inplaceEditor->deleteLater();
    g_inplaceEditor = 0;

    qDebug() << "stopInplaceEdit";

    return true;
}

bool hasParent(QObject* child, QObject* parent)
{
//    qDebug() << "hasParent(" << child << ", " << parent << ")";
    if (!child)
        return false;

    if (child == parent)
        return true;

    return hasParent(child->parent(), parent);
}

bool InplaceEditorHandler::eventFilter(QObject* watched, QEvent* event)
{
    Q_ASSERT(g_inplaceEditor);

    if (!event)
        return false;

    // try handle by base class
    if (QObject::eventFilter(watched, event))
        return true;

    if (event->type() == QEvent::FocusIn)
    {
        qDebug() << "FocusEvent: "
                 << "watched: " << watched << "; "
                 << "inplace: " << g_inplaceEditor << "; "
                 << "focus: " << QApplication::focusObject() << "; "
                 << "type: " << event->type();

        if (!hasParent(QApplication::focusObject(), g_inplaceEditor))
            stopInplaceEdit();

        return false;
    }
/*
    if (event->type() == QEvent::KeyPress)
    {
        qDebug() << "EditorHandler KeyEvent: " << event
                 << "watched: " << watched << "; ";

        if (hasParent(watched, g_inplaceEditor))
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
             if (keyEvent->key() == Qt::Key_Escape
                     || keyEvent->key() == Qt::Key_Enter
                     || keyEvent->key() == Qt::Key_Return)
             {
                 // stop edititng
                 g_inplaceEditor->deleteLater();
             }
        }
        return false;
    }
*/
    return false;
}

void InplaceEditorHandler::OnEditorDestroyed(QObject* obj)
{
    Q_ASSERT(obj == g_inplaceEditor);

    delete g_inplaceEditorHandler;
    g_inplaceEditorHandler = 0;
    g_inplaceEditor = 0;
}

} // end namespace Qtinuum

