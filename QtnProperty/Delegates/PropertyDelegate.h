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

#ifndef QTN_PROPERTY_DELEGATE_H
#define QTN_PROPERTY_DELEGATE_H

#include "../PropertyWidgetAPI.h"
#include "../../Core/Property.h"
#include "PropertyDelegateAux.h"

class QTN_PW_EXPORT QtnPropertyDelegate
{
    Q_DISABLE_COPY(QtnPropertyDelegate)

public:
    virtual ~QtnPropertyDelegate() {}

    QtnPropertyBase* property();
    const QtnPropertyBase* propertyImmutable() const;

    // for complex properties like PropertyQFont
    int subPropertyCount() const;
    QtnPropertyBase* subProperty(int index);

    // tune up with attributes
    void applyAttributes(const QtnPropertyDelegateAttributes& attributes);
    // create GUI sub elements to present property on PropertyView
    void createSubItems(QtnDrawContext& context, QList<QtnSubItem>& subItems);

protected:
    QtnPropertyDelegate() {}

    virtual QtnPropertyBase* propertyImpl() = 0;
    virtual const QtnPropertyBase* propertyImmutableImpl() const = 0;
    virtual int subPropertyCountImpl() const { return 0; }
    virtual QtnPropertyBase* subPropertyImpl(int index) { Q_UNUSED(index); return nullptr; }

    virtual void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) { Q_UNUSED(attributes); }
    virtual void createSubItemsImpl(QtnDrawContext& context, QList<QtnSubItem>& subItems) = 0;

    // helper functions
    QStyle::State state(bool isActive, QtnSubItemState subState) const;
};

#endif // QTN_PROPERTY_DELEGATE_H
