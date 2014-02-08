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

#ifndef PROPERTY_EDITOR_AUX_H
#define PROPERTY_EDITOR_AUX_H

#include "../PropertyWidgetAPI.h"

#include <QLineEdit>
#include <QToolButton>

class QKeyEvent;
class QtnInplaceInfo;

class QTN_PW_EXPORT QtnLineEditBttn: public QWidget
{
public:
    QtnLineEditBttn(QWidget *parent);

    QLineEdit *lineEdit;
    QToolButton *toolButton;
};

QTN_PW_EXPORT bool qtnAcceptForLineEdit(QKeyEvent *keyEvent);
QTN_PW_EXPORT void qtnInitLineEdit(QLineEdit *lineEdit, QtnInplaceInfo *inplaceInfo);

#endif // PROPERTY_EDITOR_AUX_H
