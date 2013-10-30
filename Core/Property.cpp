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

namespace Qtinuum
{

class PropertyDelegateInfoGetter
{
    Q_DISABLE_COPY(PropertyDelegateInfoGetter)

public:
    virtual const PropertyDelegateInfo* delegate() const = 0;

    virtual ~PropertyDelegateInfoGetter() {}

protected:
    PropertyDelegateInfoGetter() {}
};

class PropertyDelegateInfoGetterValue: public PropertyDelegateInfoGetter
{
public:
    PropertyDelegateInfoGetterValue(const PropertyDelegateInfo& delegate)
        : m_delegate(delegate)
    {
    }

    const PropertyDelegateInfo* delegate() const override
    {
        return &m_delegate;
    }

private:
    PropertyDelegateInfo m_delegate;
};

class PropertyDelegateInfoGetterCallback: public PropertyDelegateInfoGetter
{
public:
    PropertyDelegateInfoGetterCallback(const std::function<const PropertyDelegateInfo* ()>& callback)
        : m_callback(callback)
    {
    }

    const PropertyDelegateInfo* delegate() const override
    {
        if (m_delegate.isNull())
        {
            m_delegate.reset(m_callback());
        }

        return m_delegate.data();
    }

private:
    std::function<const PropertyDelegateInfo* ()> m_callback;
    mutable QScopedPointer<const PropertyDelegateInfo> m_delegate;
};

Property::Property(QObject* parent)
    : PropertyBase(parent)
{
}

Property::~Property()
{
}

const PropertyDelegateInfo* Property::delegate() const
{
    if (m_delegateInfoGetter.isNull())
        return 0;

    return m_delegateInfoGetter->delegate();
}

void Property::setDelegate(const PropertyDelegateInfo& delegate)
{
    m_delegateInfoGetter.reset(new PropertyDelegateInfoGetterValue(delegate));
}

void Property::setDelegateCallback(const std::function<const PropertyDelegateInfo*()>& callback)
{
    m_delegateInfoGetter.reset(new PropertyDelegateInfoGetterCallback(callback));
}

} // end namespace Qtinuum
