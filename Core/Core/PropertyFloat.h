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

#ifndef PROPERTYFLOAT_H
#define PROPERTYFLOAT_H

#include "../Auxiliary/PropertyTemplates.h"

class QTN_PE_CORE_EXPORT QtnPropertyFloatBase: public QtnNumericPropertyBase<QtnSinglePropertyBase<float> >
{
    Q_OBJECT
    QtnPropertyFloatBase(const QtnPropertyFloatBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyFloatBase(QObject *parent)
        : QtnNumericPropertyBase<QtnSinglePropertyBase<float> >(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyFloatBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyFloatBase, float)

class QTN_PE_CORE_EXPORT QtnPropertyFloatCallback: public QtnSinglePropertyCallback<QtnPropertyFloatBase>
{
    Q_OBJECT
    QtnPropertyFloatCallback(const QtnPropertyFloatCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyFloatCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyFloatBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyFloatCallback, QtnPropertyFloatBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyFloat: public QtnSinglePropertyValue<QtnPropertyFloatBase>
{
    Q_OBJECT
    QtnPropertyFloat(const QtnPropertyFloat& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyFloat(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyFloatBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyFloat, QtnPropertyFloatBase)
};

#endif // PROPERTYFLOAT_H
