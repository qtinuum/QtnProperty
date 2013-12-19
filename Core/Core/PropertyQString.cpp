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

#include "PropertyQString.h"

namespace Qtinuum
{

bool PropertyQStringBase::fromStrImpl(const QString& str)
{
    QString strValue = str;

    QString quotedStr = str.trimmed();
    if (quotedStr.size() > 1)
    {
        if (quotedStr.startsWith('"') && quotedStr.endsWith('"'))
        {
            strValue = quotedStr.mid(1, quotedStr.size() - 2);
        }
    }

    return setValue(strValue);
}

bool PropertyQStringBase::toStrImpl(QString& str) const
{
    str = value();
    return true;
}

} // end namespace Qtinuum
