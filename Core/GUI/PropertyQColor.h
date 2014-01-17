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

#ifndef PROPERTYCOLOR_H
#define PROPERTYCOLOR_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QtGui/QColor>

enum QtnColorDelegateShape
{
    QtnColorDelegateShapeNone = 0x1,
    QtnColorDelegateShapeSquare = 0x2,
    QtnColorDelegateShapeCircle = 0x3
};

class QTN_PE_CORE_EXPORT QtnPropertyQColorBase: public QtnSinglePropertyBase<QColor>
{
    Q_OBJECT
    QtnPropertyQColorBase(const QtnPropertyQColorBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQColorBase(QObject *parent)
        : QtnSinglePropertyBase<QColor>(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQColorBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQColorBase, QColor)

class QTN_PE_CORE_EXPORT QtnPropertyQColorCallback: public QtnSinglePropertyCallback<QtnPropertyQColorBase>
{
    Q_OBJECT
    QtnPropertyQColorCallback(const QtnPropertyQColorCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQColorCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQColorBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQColorCallback, QtnPropertyQColorBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQColor: public QtnSinglePropertyValue<QtnPropertyQColorBase>
{
    Q_OBJECT
    QtnPropertyQColor(const QtnPropertyQColor& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQColor(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQColorBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQColor, QtnPropertyQColorBase)
};

#endif // PROPERTYCOLOR_H
