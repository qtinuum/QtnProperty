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
