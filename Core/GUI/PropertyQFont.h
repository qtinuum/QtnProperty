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

#ifndef PROPERTYFONT_H
#define PROPERTYFONT_H

#include "../Auxiliary/PropertyTemplates.h"
#include <QtGui/QFont>

class QTN_PE_CORE_EXPORT QtnPropertyQFontBase: public QtnSinglePropertyBase<QFont>
{
    Q_OBJECT
    QtnPropertyQFontBase(const QtnPropertyQFontBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQFontBase(QObject *parent);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQFontBase)
};

P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyQFontBase, QFont)

class QTN_PE_CORE_EXPORT QtnPropertyQFontCallback: public QtnSinglePropertyCallback<QtnPropertyQFontBase>
{
    Q_OBJECT
    QtnPropertyQFontCallback(const QtnPropertyQFontCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQFontCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQFontBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQFontCallback, QtnPropertyQFontBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQFont: public QtnSinglePropertyValue<QtnPropertyQFontBase>
{
    Q_OBJECT
    QtnPropertyQFont(const QtnPropertyQFont& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQFont(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQFontBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQFont, QtnPropertyQFontBase)
};

#endif // PROPERTYFONT_H
