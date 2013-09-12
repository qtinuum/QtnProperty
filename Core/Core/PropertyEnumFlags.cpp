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

#include "PropertyEnumFlags.h"

namespace Qtinuum
{
PropertyEnumFlagsBase::PropertyEnumFlagsBase(QObject *parent)
    : SinglePropertyBase<EnumFlagsValueType>(parent),
      m_enumInfo(0)
{
    // collapsed by default
    addState(PropertyStateCollapsed);
}

} // end namespace Qtinuum
