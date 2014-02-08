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

#ifndef PROPERTY_DELEGATE_ENUM_FLAGS_H
#define PROPERTY_DELEGATE_ENUM_FLAGS_H

#include "../PropertyDelegate.h"

class QtnPropertyEnumFlagsBase;

class QTN_PW_EXPORT QtnPropertyDelegateEnumFlags: public QtnPropertyDelegateTypedEx<QtnPropertyEnumFlagsBase>
{
    Q_DISABLE_COPY(QtnPropertyDelegateEnumFlags)

public:
    QtnPropertyDelegateEnumFlags(QtnPropertyEnumFlagsBase& owner);

protected:
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
    bool propertyValueToStr(QString& strValue) const override;
};

#endif // PROPERTY_DELEGATE_ENUM_FLAGS_H
