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

#include "PropertyQColor.h"

bool QtnPropertyQColorBase::fromStrImpl(const QString& str)
{
    QColor color(str.trimmed());
    if (!color.isValid())
        return false;

    return setValue(color);
}

bool QtnPropertyQColorBase::toStrImpl(QString& str) const
{
    QColor v = value();

    if (v.alpha() < 255)
        str = v.name(QColor::HexArgb);
    else
        str = v.name();

    return true;
}
