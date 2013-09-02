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

#include "../PropertyBasis.h"

namespace Qtinuum
{

class QTN_PE_CORE_EXPORT PropertyQStringBase: public SinglePropertyBase<QString>
{
    Q_OBJECT

public:
    explicit PropertyQStringBase(QObject *parent)
        : SinglePropertyBase<QString>(parent)
    {
    }

    operator QString() const { return value(); }

    PropertyQStringBase &operator=(const PropertyQStringBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQStringBase &operator=(const QString &newValue)
    {
        setValue(newValue);
        return *this;
    }

    PropertyQStringBase &operator=(const char* newValue)
    {
        setValue(QString(newValue));
        return *this;
    }

private:
    PropertyQStringBase(const PropertyQStringBase &) Q_DECL_EQ_DELETE;
};
P_PROPERTY_DECL_ALL_OPERATORS(PropertyQStringBase, QString)

class QTN_PE_CORE_EXPORT PropertyQStringCallback: public SinglePropertyCallback<PropertyQStringBase>
{
    Q_OBJECT

public:
    explicit PropertyQStringCallback(QObject *parent)
        : SinglePropertyCallback<PropertyQStringBase>(parent)
    {
    }

    operator QString() const { return value(); }

    PropertyQStringCallback &operator=(const PropertyQStringCallback &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQStringCallback &operator=(const PropertyQStringBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQStringCallback &operator=(const QString &newValue)
    {
        setValue(newValue);
        return *this;
    }

    PropertyQStringCallback &operator=(const char* newValue)
    {
        setValue(QString(newValue));
        return *this;
    }
};

class QTN_PE_CORE_EXPORT PropertyQString: public SinglePropertyValue<PropertyQStringBase>
{
    Q_OBJECT

public:
    explicit PropertyQString(QObject *parent)
        : SinglePropertyValue<PropertyQStringBase>(parent)
    {
    }

    operator QString() const { return value(); }

    PropertyQString &operator=(const PropertyQString &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQString &operator=(const PropertyQStringBase &newValue)
    {
        setValue(newValue.value());
        return *this;
    }

    PropertyQString &operator=(const QString &newValue)
    {
        setValue(newValue);
        return *this;
    }

    PropertyQString &operator=(const char* newValue)
    {
        setValue(QString(newValue));
        return *this;
    }
};

} // end namespace Qtinuum

#endif // PROPERTYSTRING_H
