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

#ifndef PROPERTYQRECT_H
#define PROPERTYQRECT_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QRect>

class QTN_PE_CORE_EXPORT QtnPropertyQRectBase: public QtnSinglePropertyBase<QRect>
{
    Q_OBJECT
    QtnPropertyQRectBase(const QtnPropertyQRectBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQRectBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQRectBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQRectBase, QRect)

QTN_PE_CORE_EXPORT QtnProperty* qtnCreateLeftProperty(QObject *parent, QtnPropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateRightProperty(QObject *parent, QtnPropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateTopProperty(QObject *parent, QtnPropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateBottomProperty(QObject *parent, QtnPropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateWidthProperty(QObject *parent, QtnPropertyQRectBase *propertyRect);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateHeightProperty(QObject *parent, QtnPropertyQRectBase *propertyRect);

class QTN_PE_CORE_EXPORT QtnPropertyQRectCallback: public QtnSinglePropertyCallback<QtnPropertyQRectBase>
{
    Q_OBJECT
    QtnPropertyQRectCallback(const QtnPropertyQRectCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQRectCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQRectBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRectCallback, QtnPropertyQRectBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQRect: public QtnSinglePropertyValue<QtnPropertyQRectBase>
{
    Q_OBJECT
    QtnPropertyQRect(const QtnPropertyQRect& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQRect(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQRectBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQRect, QtnPropertyQRectBase)
};

#endif // PROPERTYQRECT_H
