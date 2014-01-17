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

QtnPropertyEnumFlagsBase::QtnPropertyEnumFlagsBase(QObject *parent)
    : QtnSinglePropertyBase<QtnEnumFlagsValueType>(parent),
      m_enumInfo(0)
{
    // collapsed by default
    addState(QtnPropertyStateCollapsed);
}

bool QtnPropertyEnumFlagsBase::fromStrImpl(const QString& str)
{
    static QRegExp parserEnumFlags("^\\s*([^|\\s]+)\\s*\\|(.+)$", Qt::CaseInsensitive);

    if (!m_enumInfo)
        return false;

    QtnEnumFlagsValueType val = 0;
    QString enumStr = str.trimmed();

    if (enumStr != "0")
    {
        while (parserEnumFlags.exactMatch(enumStr))
        {
            QStringList params = parserEnumFlags.capturedTexts();
            if (params.size() != 3)
                return false;

            const QtnEnumValueInfo* enumValue = m_enumInfo->fromStr(params[1]);
            if (!enumValue)
                return false;

            val = val|enumValue->value();

            enumStr = params[2];
        }

        const QtnEnumValueInfo* enumValue = m_enumInfo->fromStr(enumStr);
        if (!enumValue)
            return false;

        val = val|enumValue->value();
    }

    return setValue(val);
}

bool QtnPropertyEnumFlagsBase::toStrImpl(QString& str) const
{
    if (!m_enumInfo)
        return false;

    QtnEnumFlagsValueType v = value();

    if (v == 0)
    {
        str = "0";
        return true;
    }

    QString strValues;
    m_enumInfo->forEachEnumValue([&strValues, v, this](const QtnEnumValueInfo& value)->bool {
        if (v & value.value())
        {
            if (!strValues.isEmpty())
                strValues += "|";

            QString enumStr;
            m_enumInfo->toStr(enumStr, &value);
            strValues += enumStr;
        }

        return true;
    });

    Q_ASSERT(!strValues.isEmpty());

    str = strValues;
    return true;
}
