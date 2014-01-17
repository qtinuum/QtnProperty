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

#ifndef PROPERTYSTRING_H
#define PROPERTYSTRING_H

#include "../Auxiliary/PropertyTemplates.h"

class QTN_PE_CORE_EXPORT QtnPropertyQStringBase: public QtnSinglePropertyBase<QString>
{
    Q_OBJECT
    QtnPropertyQStringBase(const QtnPropertyQStringBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQStringBase(QObject *parent)
        : QtnSinglePropertyBase<QString>(parent)
    {
    }

    QtnPropertyQStringBase& operator=(const char* newValue)
    {
        setValue(QString(newValue));
        return *this;
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyQStringBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyQStringBase, QString)

class QTN_PE_CORE_EXPORT QtnPropertyQStringCallback: public QtnSinglePropertyCallback<QtnPropertyQStringBase>
{
    Q_OBJECT
    QtnPropertyQStringCallback(const QtnPropertyQStringCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQStringCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyQStringBase>(parent)
    {
    }

    QtnPropertyQStringCallback& operator=(const char* newValue)
    {
        setValue(QString(newValue));
        return *this;
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQStringCallback, QtnPropertyQStringBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyQString: public QtnSinglePropertyValue<QtnPropertyQStringBase>
{
    Q_OBJECT
    QtnPropertyQString(const QtnPropertyQString& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyQString(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyQStringBase>(parent)
    {
    }

    QtnPropertyQString& operator=(const char* newValue)
    {
        setValue(QString(newValue));
        return *this;
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyQString, QtnPropertyQStringBase)
};

#endif // PROPERTYSTRING_H
