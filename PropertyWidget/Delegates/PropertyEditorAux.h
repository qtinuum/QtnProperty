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

#ifndef PROPERTY_EDITOR_AUX_H
#define PROPERTY_EDITOR_AUX_H

#include "../PropertyWidgetAPI.h"

#include <QLineEdit>
#include <QToolButton>

class QKeyEvent;

namespace Qtinuum
{

class InplaceInfo;

class QTN_PW_EXPORT LineEditBttn: public QWidget
{
public:
    LineEditBttn(QWidget *parent);

    QLineEdit *lineEdit;
    QToolButton *toolButton;
};

QTN_PW_EXPORT bool acceptForLineEdit(QKeyEvent *keyEvent);
QTN_PW_EXPORT void initLineEdit(QLineEdit *lineEdit, InplaceInfo *inplaceInfo);

} // end namespace Qtinuum

#endif // PROPERTY_EDITOR_AUX_H
