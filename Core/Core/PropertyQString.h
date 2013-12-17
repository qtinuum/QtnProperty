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

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyQStringBase: public SinglePropertyBase<QString>
{
    Q_OBJECT
    PropertyQStringBase(const PropertyQStringBase& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyQStringBase(QObject *parent)
        : SinglePropertyBase<QString>(parent)
    {
    }

    PropertyQStringBase& operator=(const char* newValue)
    {
        setValue(QString(newValue));
        return *this;
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(PropertyQStringBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(PropertyQStringBase, QString)

class QTN_PE_CORE_EXPORT PropertyQStringCallback: public SinglePropertyCallback<PropertyQStringBase>
{
    Q_OBJECT
    PropertyQStringCallback(const PropertyQStringCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyQStringCallback(QObject *parent)
        : SinglePropertyCallback<PropertyQStringBase>(parent)
    {
    }

    PropertyQStringCallback& operator=(const char* newValue)
    {
        setValue(QString(newValue));
        return *this;
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQStringCallback, PropertyQStringBase)
};

class QTN_PE_CORE_EXPORT PropertyQString: public SinglePropertyValue<PropertyQStringBase>
{
    Q_OBJECT
    PropertyQString(const PropertyQString& other) Q_DECL_EQ_DELETE;

public:
    explicit PropertyQString(QObject *parent)
        : SinglePropertyValue<PropertyQStringBase>(parent)
    {
    }

    PropertyQString& operator=(const char* newValue)
    {
        setValue(QString(newValue));
        return *this;
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(PropertyQString, PropertyQStringBase)
};

} // end namespace Qtinuum

#endif // PROPERTYSTRING_H
