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

#ifndef PROPERTYMACRO_H
#define PROPERTYMACRO_H

#define P_PROPERTY_DECL_CMP_OPERATOR(ClassName, ValueType, Op) \
    inline bool operator Op(const ClassName& left, const ClassName& right)\
    {\
        return left.value() Op right.value();\
    }\
    inline bool operator Op(const ClassName& left, ValueType right)\
    {\
        return left.value() Op right;\
    }\
    inline bool operator Op(ValueType left, const ClassName& right)\
    {\
        return left Op right.value();\
    }

#define P_PROPERTY_DECL_EQ_OPERATORS(ClassName, ValueType) \
    P_PROPERTY_DECL_CMP_OPERATOR(ClassName, ValueType, ==)\
    P_PROPERTY_DECL_CMP_OPERATOR(ClassName, ValueType, !=)

#define P_PROPERTY_DECL_ALL_OPERATORS(ClassName, ValueType) \
    P_PROPERTY_DECL_EQ_OPERATORS(ClassName, ValueType)\
    P_PROPERTY_DECL_CMP_OPERATOR(ClassName, ValueType, <)\
    P_PROPERTY_DECL_CMP_OPERATOR(ClassName, ValueType, <=)\
    P_PROPERTY_DECL_CMP_OPERATOR(ClassName, ValueType, >)\
    P_PROPERTY_DECL_CMP_OPERATOR(ClassName, ValueType, >=)

#define P_PROPERTY_DECL_MEMBER_OPERATORS(ClassName) \
public:\
    operator ValueType() const { return value(); }\
    ClassName& operator=(const ClassName& newValue)\
    {\
        setValue(newValue.value());\
        return *this;\
    }\
    ClassName& operator=(ValueType newValue)\
    {\
        setValue(newValue);\
        return *this;\
    }

#define P_PROPERTY_DECL_MEMBER_OPERATORS2(ClassName, BaseClassName) \
public:\
    operator ValueType() const { return value(); }\
    ClassName& operator=(const ClassName& newValue)\
    {\
        setValue(newValue.value());\
        return *this;\
    }\
    ClassName& operator=(const BaseClassName& newValue)\
    {\
        setValue(newValue.value());\
        return *this;\
    }\
    ClassName& operator=(ValueType newValue)\
    {\
        setValue(newValue);\
        return *this;\
    }

#endif // PROPERTYMACRO_H
