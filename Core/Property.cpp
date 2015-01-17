/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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

#include "Property.h"

class QtnPropertyDelegateInfoGetter
{
    Q_DISABLE_COPY(QtnPropertyDelegateInfoGetter)

public:
    virtual const QtnPropertyDelegateInfo* delegate() const = 0;

    virtual ~QtnPropertyDelegateInfoGetter() {}

protected:
    QtnPropertyDelegateInfoGetter() {}
};

class QtnPropertyDelegateInfoGetterValue: public QtnPropertyDelegateInfoGetter
{
public:
    QtnPropertyDelegateInfoGetterValue(const QtnPropertyDelegateInfo& delegate)
        : m_delegate(delegate)
    {
    }

    const QtnPropertyDelegateInfo* delegate() const override
    {
        return &m_delegate;
    }

private:
    QtnPropertyDelegateInfo m_delegate;
};

class QtnPropertyDelegateInfoGetterCallback: public QtnPropertyDelegateInfoGetter
{
public:
    QtnPropertyDelegateInfoGetterCallback(const std::function<const QtnPropertyDelegateInfo* ()>& callback)
        : m_callback(callback)
    {
    }

    const QtnPropertyDelegateInfo* delegate() const override
    {
        if (m_delegate.isNull())
        {
            m_delegate.reset(m_callback());
        }

        return m_delegate.data();
    }

private:
    std::function<const QtnPropertyDelegateInfo* ()> m_callback;
    mutable QScopedPointer<const QtnPropertyDelegateInfo> m_delegate;
};

QtnProperty::QtnProperty(QObject* parent)
    : QtnPropertyBase(parent)
{
}

QtnProperty::~QtnProperty()
{
}

const QtnPropertyDelegateInfo* QtnProperty::delegate() const
{
    if (m_delegateInfoGetter.isNull())
        return 0;

    return m_delegateInfoGetter->delegate();
}

void QtnProperty::setDelegate(const QtnPropertyDelegateInfo& delegate)
{
    m_delegateInfoGetter.reset(new QtnPropertyDelegateInfoGetterValue(delegate));
}

void QtnProperty::setDelegateCallback(const std::function<const QtnPropertyDelegateInfo*()>& callback)
{
    m_delegateInfoGetter.reset(new QtnPropertyDelegateInfoGetterCallback(callback));
}
