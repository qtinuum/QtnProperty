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

#ifndef PROPERTYINT_H
#define PROPERTYINT_H

#include "../Auxiliary/PropertyTemplates.h"

class QTN_PE_CORE_EXPORT QtnPropertyIntBase: public QtnNumericPropertyBase<QtnSinglePropertyBase<qint32>>
{
    Q_OBJECT
    QtnPropertyIntBase(const QtnPropertyIntBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyIntBase(QObject *parent)
        : QtnNumericPropertyBase<QtnSinglePropertyBase<qint32>>(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyIntBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyIntBase, qint32)

class QTN_PE_CORE_EXPORT QtnPropertyIntCallback: public QtnSinglePropertyCallback<QtnPropertyIntBase>
{
    Q_OBJECT
    QtnPropertyIntCallback(const QtnPropertyIntCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyIntCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyIntBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyIntCallback, QtnPropertyIntBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyInt: public QtnSinglePropertyValue<QtnPropertyIntBase>
{
    Q_OBJECT
    QtnPropertyInt(const QtnPropertyInt& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyInt(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyIntBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyInt, QtnPropertyIntBase)
};

#endif // PROPERTYINT_H
