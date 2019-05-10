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

#include "PropertyEditorAux.h"
#include "../PropertyDelegate.h"

#include <QHBoxLayout>
#include <QKeyEvent>

QtnLineEditBttn::QtnLineEditBttn(QWidget *parent, QString bttnText)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    lineEdit = new QLineEdit(this);
    layout->addWidget(lineEdit);

    toolButton = new QToolButton(this);
    toolButton->setText(bttnText);
    toolButton->setFocusPolicy(Qt::StrongFocus);
    layout->addWidget(toolButton);

    setFocusProxy(lineEdit);
    setAutoFillBackground(true);
}

QtnComboBoxBttn::QtnComboBoxBttn(QWidget *parent, QString bttnText)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    comboBox = new QComboBox(this);
    layout->addWidget(comboBox);

    toolButton = new QToolButton(this);
    toolButton->setText(bttnText);
    toolButton->setFocusPolicy(Qt::StrongFocus);
    layout->addWidget(toolButton);

    setFocusProxy(comboBox);
    setAutoFillBackground(true);
}

bool qtnAcceptForLineEdit(QKeyEvent *keyEvent)
{
    if (keyEvent->type() != QEvent::KeyPress)
        return false;

    // any printable key press is acceptable
    QString text = keyEvent->text();
    return (text.size() == 1 && text[0].isPrint());
}

void qtnInitLineEdit(QLineEdit *lineEdit, QtnInplaceInfo *inplaceInfo)
{
    if (!lineEdit || !inplaceInfo)
        return;

    if (!lineEdit->isReadOnly() && (inplaceInfo->activationEvent->type() == QEvent::KeyPress))
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(inplaceInfo->activationEvent);
        if (qtnAcceptForLineEdit(keyEvent))
        {
            lineEdit->setText(keyEvent->text());
            return;
        }
    }
    else
    {
        lineEdit->selectAll();
    }
}
