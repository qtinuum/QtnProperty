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

#ifndef PROPERTYENUM_H
#define PROPERTYENUM_H

#include "../Auxiliary/PropertyTemplates.h"
#include "../Enum.h"

class QTN_PE_CORE_EXPORT QtnPropertyEnumBase: public QtnSinglePropertyBase<QtnEnumValueType>
{
    Q_OBJECT
    QtnPropertyEnumBase(const QtnPropertyEnumBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumBase(QObject* parent)
        : QtnSinglePropertyBase<QtnEnumValueType>(parent),
          m_enumInfo(nullptr)
    {
    }

    const QtnEnumInfo* enumInfo() const { return m_enumInfo; }
    void setEnumInfo(const QtnEnumInfo* enumInfo) { m_enumInfo = enumInfo; }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    bool isValueAcceptedImpl(ValueType valueToAccept) override;

private:
    const QtnEnumInfo* m_enumInfo;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyEnumBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyEnumBase, QtnEnumValueType)

class QTN_PE_CORE_EXPORT QtnPropertyEnumCallback: public QtnSinglePropertyCallback<QtnPropertyEnumBase>
{
    Q_OBJECT
    QtnPropertyEnumCallback(const QtnPropertyEnumCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnumCallback(QObject* parent)
        : QtnSinglePropertyCallback<QtnPropertyEnumBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyEnumCallback, QtnPropertyEnumBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyEnum: public QtnSinglePropertyValue<QtnPropertyEnumBase>
{
    Q_OBJECT
    QtnPropertyEnum(const QtnPropertyEnum& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyEnum(QObject* parent)
        : QtnSinglePropertyValue<QtnPropertyEnumBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyEnum, QtnPropertyEnumBase)
};

#endif // PROPERTYENUM_H
