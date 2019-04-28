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

#ifndef PROPERTYBOOL_H
#define PROPERTYBOOL_H

#include "../Auxiliary/PropertyTemplates.h"

class QTN_PE_CORE_EXPORT QtnPropertyBoolBase: public QtnSinglePropertyBase<bool>
{
    Q_OBJECT
    QtnPropertyBoolBase(const QtnPropertyBoolBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyBoolBase(QObject *parent)
        : QtnSinglePropertyBase<bool>(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyBoolBase)
};

P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyBoolBase, bool)

class QTN_PE_CORE_EXPORT QtnPropertyBoolCallback: public QtnSinglePropertyCallback<QtnPropertyBoolBase>
{
    Q_OBJECT
    QtnPropertyBoolCallback(const QtnPropertyBoolCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyBoolCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyBoolBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyBoolCallback, QtnPropertyBoolBase)
};

class QTN_PE_CORE_EXPORT QtnPropertyBool: public QtnSinglePropertyValue<QtnPropertyBoolBase>
{
    Q_OBJECT
    QtnPropertyBool(const QtnPropertyBool& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyBool(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyBoolBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyBool, QtnPropertyBoolBase)
};

#endif // PROPERTYBOOL_H
