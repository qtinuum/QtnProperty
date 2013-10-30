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

#ifndef QTN_PROPERTY_H
#define QTN_PROPERTY_H

#include "PropertyBase.h"
#include "Aux/PropertyDelegateInfo.h"
#include <functional>

namespace Qtinuum
{

class PropertyDelegateInfoGetter;

class QTN_PE_CORE_EXPORT Property: public PropertyBase
{
    Q_OBJECT
    Q_DISABLE_COPY(Property)

public:
    virtual ~Property();

    // delegates
    const PropertyDelegateInfo* delegate() const;
    void setDelegate(const PropertyDelegateInfo& delegate);
    void setDelegateCallback(const std::function<const PropertyDelegateInfo*()>& callback);

    // casts
    Property* asProperty() override { return this; }
    const Property* asProperty() const override { return this; }

Q_SIGNALS:
    void propertyValueAccept(const Property* property, PropertyValuePtr valueToAccept, bool* accept);

protected:
    explicit Property(QObject* parent);

private:
    QScopedPointer<PropertyDelegateInfoGetter> m_delegateInfoGetter;
};

} // end namespace Qtinuum

#endif // QTN_PROPERTY_H
