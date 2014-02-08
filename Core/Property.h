/*
 * Copyright (c) 2012 - 2014, the Qtinuum project.
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
#include "Auxiliary/PropertyDelegateInfo.h"
#include <functional>

class QtnPropertyDelegateInfoGetter;

class QTN_PE_CORE_EXPORT QtnProperty: public QtnPropertyBase
{
    Q_OBJECT
    Q_DISABLE_COPY(QtnProperty)

public:
    virtual ~QtnProperty();

    // delegates
    const QtnPropertyDelegateInfo* delegate() const;
    void setDelegate(const QtnPropertyDelegateInfo& delegate);
    void setDelegateCallback(const std::function<const QtnPropertyDelegateInfo*()>& callback);

    // casts
    QtnProperty* asProperty() override { return this; }
    const QtnProperty* asProperty() const override { return this; }

Q_SIGNALS:
    void propertyValueAccept(const QtnProperty* property, QtnPropertyValuePtr valueToAccept, bool* accept);

protected:
    explicit QtnProperty(QObject* parent);

private:
    QScopedPointer<QtnPropertyDelegateInfoGetter> m_delegateInfoGetter;
};

#endif // QTN_PROPERTY_H
