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
