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

#ifndef PROPERTYDOUBLE_H
#define PROPERTYDOUBLE_H

#include "../Auxiliary/PropertyTemplates.h"

class QTN_PE_CORE_EXPORT QtnPropertyDoubleBase: public QtnNumericPropertyBase<QtnSinglePropertyBase<double>>
{
    Q_OBJECT
    QtnPropertyDoubleBase(const QtnPropertyDoubleBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyDoubleBase(QObject *parent)
        : QtnNumericPropertyBase<QtnSinglePropertyBase<double>>(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyDoubleBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyDoubleBase, double)

class QTN_PE_CORE_EXPORT QtnPropertyDoubleCallback: public QtnSinglePropertyCallback<QtnPropertyDoubleBase>
{
    Q_OBJECT
    QtnPropertyDoubleCallback(const QtnPropertyDoubleCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyDoubleCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyDoubleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyDoubleCallback, QtnPropertyDoubleBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyDouble: public QtnSinglePropertyValue<QtnPropertyDoubleBase>
{
    Q_OBJECT
    QtnPropertyDouble(const QtnPropertyDouble& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyDouble(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyDoubleBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyDouble, QtnPropertyDoubleBase)
};

#endif // PROPERTYDOUBLE_H
