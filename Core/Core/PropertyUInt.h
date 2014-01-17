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

#ifndef PROPERTYUINT_H
#define PROPERTYUINT_H

#include "../Auxiliary/PropertyTemplates.h"

class QTN_PE_CORE_EXPORT QtnPropertyUIntBase: public QtnNumericPropertyBase<QtnSinglePropertyBase<quint32> >
{
    Q_OBJECT
    QtnPropertyUIntBase(const QtnPropertyUIntBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyUIntBase(QObject *parent)
        : QtnNumericPropertyBase<QtnSinglePropertyBase<quint32> >(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyUIntBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyUIntBase, quint32)

class QTN_PE_CORE_EXPORT QtnPropertyUIntCallback: public QtnSinglePropertyCallback<QtnPropertyUIntBase>
{
    Q_OBJECT
    QtnPropertyUIntCallback(const QtnPropertyUIntCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyUIntCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyUIntBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyUIntCallback, QtnPropertyUIntBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyUInt: public QtnSinglePropertyValue<QtnPropertyUIntBase>
{
    Q_OBJECT
    QtnPropertyUInt(const QtnPropertyUInt& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyUInt(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyUIntBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyUInt, QtnPropertyUIntBase)
};

#endif // PROPERTYUINT_H
