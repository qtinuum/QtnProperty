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

#include "PropertyBool.h"

static QString getBoolText(bool value)
{
    static QString boolTexts[2] = {QObject::tr("False"), QObject::tr("True")};
    return boolTexts[value];
}

static bool getBoolValue(QString boolText, bool& success)
{
    success = true;
    if (QString::compare(boolText, getBoolText(false), Qt::CaseInsensitive) == 0)
        return false;

    if (QString::compare(boolText, getBoolText(true), Qt::CaseInsensitive) == 0)
        return true;

    success = false;
    return false;
}

bool QtnPropertyBoolBase::fromStrImpl(const QString& str)
{
    bool success = false;
    bool value = getBoolValue(str.trimmed(), success);

    if (!success)
        return false;

    return setValue(value);
}

bool QtnPropertyBoolBase::toStrImpl(QString& str) const
{
    bool boolValue = value();
    str = getBoolText(boolValue);
    return true;
}

