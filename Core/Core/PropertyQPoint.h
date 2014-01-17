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

#ifndef PROPERTYQPOINT_H
#define PROPERTYQPOINT_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QPoint>

class QTN_PE_CORE_EXPORT QtnPropertyQPointBase: public QtnSinglePropertyBase<QPoint>
{
    Q_OBJECT
    QtnPropertyQPointBase(const QtnPropertyQPointBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPointBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQPointBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQPointBase, QPoint)

QTN_PE_CORE_EXPORT QtnProperty* qtnCreateXProperty(QObject *parent, QtnPropertyQPointBase *propertyPoint);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateYProperty(QObject *parent, QtnPropertyQPointBase *propertyPoint);

class QTN_PE_CORE_EXPORT QtnPropertyQPointCallback: public QtnSinglePropertyCallback<QtnPropertyQPointBase>
{
    Q_OBJECT
    QtnPropertyQPointCallback(const QtnPropertyQPointCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPointCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQPointBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPointCallback, QtnPropertyQPointBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQPoint: public QtnSinglePropertyValue<QtnPropertyQPointBase>
{
    Q_OBJECT
    QtnPropertyQPoint(const QtnPropertyQPoint& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQPoint(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQPointBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQPoint, QtnPropertyQPointBase)
};

#endif // PROPERTYQPOINT_H
