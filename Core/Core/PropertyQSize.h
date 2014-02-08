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

#ifndef PROPERTYQSIZE_H
#define PROPERTYQSIZE_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QSize>

class QTN_PE_CORE_EXPORT QtnPropertyQSizeBase: public QtnSinglePropertyBase<QSize>
{
    Q_OBJECT
    QtnPropertyQSizeBase(const QtnPropertyQSizeBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQSizeBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQSizeBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQSizeBase, QSize)

QTN_PE_CORE_EXPORT QtnProperty* qtnCreateWidthProperty(QObject* parent, QtnPropertyQSizeBase* propertySize);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateHeightProperty(QObject* parent, QtnPropertyQSizeBase* propertySize);

class QTN_PE_CORE_EXPORT QtnPropertyQSizeCallback: public QtnSinglePropertyCallback<QtnPropertyQSizeBase>
{
    Q_OBJECT
    QtnPropertyQSizeCallback(const QtnPropertyQSizeCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQSizeCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQSizeBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSizeCallback, QtnPropertyQSizeBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQSize: public QtnSinglePropertyValue<QtnPropertyQSizeBase>
{
    Q_OBJECT
    QtnPropertyQSize(const QtnPropertyQSize& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQSize(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQSizeBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQSize, QtnPropertyQSizeBase)
};

#endif // PROPERTYQSIZE_H
