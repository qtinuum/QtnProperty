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

#ifndef PROPERTYBOOL_H
#define PROPERTYBOOL_H

#include "../Auxiliary/PropertyTemplates.h"

class QTN_PE_CORE_EXPORT QtnPropertyBoolBase: public QtnSinglePropertyBase<bool>
{
    Q_OBJECT
    QtnPropertyBoolBase(const QtnPropertyBoolBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyBoolBase(QObject *parent)
        : QtnSinglePropertyBase<bool>(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyBoolBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyBoolBase, bool)

class QTN_PE_CORE_EXPORT QtnPropertyBoolCallback: public QtnSinglePropertyCallback<QtnPropertyBoolBase>
{
    Q_OBJECT
    QtnPropertyBoolCallback(const QtnPropertyBoolCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyBoolCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyBoolBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyBoolCallback, QtnPropertyBoolBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyBool: public QtnSinglePropertyValue<QtnPropertyBoolBase>
{
    Q_OBJECT
    QtnPropertyBool(const QtnPropertyBool& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyBool(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyBoolBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyBool, QtnPropertyBoolBase)
};

#endif // PROPERTYBOOL_H
