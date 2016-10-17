/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "QtnProperty/CoreAPI.h"

#include <QLineEdit>
#include <QToolButton>

class QKeyEvent;
class QtnInplaceInfo;
class QtnProperty;

class QTN_IMPORT_EXPORT QtnLineEditBttn: public QWidget
{
public:
	QtnLineEditBttn(QWidget *parent);

	void setTextForProperty(QtnProperty *property,
							const QString &text);

	QLineEdit *lineEdit;
	QToolButton *toolButton;
};

enum QtnNumType
{
	NUM_SIGNED_INT,
	NUM_UNSIGNED_INT,
	NUM_FLOAT
};

QTN_IMPORT_EXPORT bool qtnAcceptForLineEdit(QKeyEvent *keyEvent);
QTN_IMPORT_EXPORT bool qtnAcceptForNumEdit(QKeyEvent *keyEvent, QtnNumType type);

QTN_IMPORT_EXPORT void qtnInitLineEdit(QLineEdit *lineEdit, QtnInplaceInfo *inplaceInfo);
QTN_IMPORT_EXPORT void qtnInitNumEdit(QWidget *numEdit, QtnInplaceInfo *inplaceInfo, QtnNumType type);

#endif // PROPERTY_EDITOR_AUX_H
