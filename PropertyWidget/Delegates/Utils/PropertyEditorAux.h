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

#ifndef PROPERTY_EDITOR_AUX_H
#define PROPERTY_EDITOR_AUX_H

#include "../../PropertyWidgetAPI.h"
#include "PropertyDelegateMisc.h"

#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>

class QKeyEvent;

class QTN_PW_EXPORT QtnLineEditBttn: public QWidget
{
public:
    QtnLineEditBttn(QWidget *parent, QString bttnText = "...");

    QLineEdit *lineEdit = nullptr;
    QToolButton *toolButton = nullptr;
};

class QTN_PW_EXPORT QtnComboBoxBttn: public QWidget
{
public:
    QtnComboBoxBttn(QWidget *parent, QString bttnText = "...");

    QComboBox *comboBox = nullptr;
    QToolButton *toolButton = nullptr;
};

QTN_PW_EXPORT bool qtnAcceptForLineEdit(QKeyEvent *keyEvent);
QTN_PW_EXPORT void qtnInitLineEdit(QLineEdit *lineEdit, QtnInplaceInfo *inplaceInfo);

#endif // PROPERTY_EDITOR_AUX_H
