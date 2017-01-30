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

#ifndef PROPERTY_FREQ_H
#define PROPERTY_FREQ_H

#include "Auxiliary/PropertyTemplates.h"

enum class FreqUnit
{
    Hz,
    KHz,
    MHz
};


class QtnPropertyFreqBase: public QtnSinglePropertyBase<int>
{
    Q_OBJECT
    QtnPropertyFreqBase(const QtnPropertyFreqBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyFreqBase(QObject *parent)
        : QtnSinglePropertyBase<int>(parent),
          m_unit(FreqUnit::Hz)
    {
    }

    FreqUnit unit() const { return m_unit; }
    void setUnit(FreqUnit unit);


protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

private:
    FreqUnit m_unit;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyFreqBase)
};

class QtnPropertyFreqCallback: public QtnSinglePropertyCallback<QtnPropertyFreqBase>
{
    Q_OBJECT
    QtnPropertyFreqCallback(const QtnPropertyFreqCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyFreqCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyFreqBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyFreqCallback, QtnPropertyFreqBase)
};

class QtnPropertyFreq: public QtnSinglePropertyValue<QtnPropertyFreqBase>
{
    Q_OBJECT
    QtnPropertyFreq(const QtnPropertyFreq& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyFreq(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyFreqBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyFreq, QtnPropertyFreqBase)
};

#endif // PROPERTY_FREQ_H
