/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
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
