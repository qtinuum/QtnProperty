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

#ifndef PROPERTYENUMFLAGS_H
#define PROPERTYENUMFLAGS_H

#include "../Auxiliary/PropertyTemplates.h"
#include "../Enum.h"

// not unsigned to disable warnings
typedef /*unsigned*/ qint32 QtnEnumFlagsValueType;

class QTN_PE_CORE_EXPORT QtnPropertyEnumFlagsBase: public QtnSinglePropertyBase<QtnEnumFlagsValueType>
{
    Q_OBJECT
    QtnPropertyEnumFlagsBase(const QtnPropertyEnumFlagsBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumFlagsBase(QObject* parent);

    const QtnEnumInfo* enumInfo() const { return m_enumInfo; }
    void setEnumInfo(const QtnEnumInfo* enumInfo) { m_enumInfo = enumInfo; }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

private:
    const QtnEnumInfo* m_enumInfo;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyEnumFlagsBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyEnumFlagsBase, QtnEnumFlagsValueType)

class QTN_PE_CORE_EXPORT QtnPropertyEnumFlagsCallback: public QtnSinglePropertyCallback<QtnPropertyEnumFlagsBase>
{
    Q_OBJECT
    QtnPropertyEnumFlagsCallback(const QtnPropertyEnumFlagsCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumFlagsCallback(QObject* parent)
        : QtnSinglePropertyCallback<QtnPropertyEnumFlagsBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyEnumFlagsCallback, QtnPropertyEnumFlagsBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyEnumFlags: public QtnSinglePropertyValue<QtnPropertyEnumFlagsBase>
{
    Q_OBJECT
    QtnPropertyEnumFlags(const QtnPropertyEnumFlags& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumFlags(QObject* parent)
        : QtnSinglePropertyValue<QtnPropertyEnumFlagsBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyEnumFlags, QtnPropertyEnumFlagsBase)
};

#endif // PROPERTYENUMFLAGS_H
