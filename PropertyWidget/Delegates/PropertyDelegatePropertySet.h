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

#ifndef QTN_PROPERTY_DELEGATE_PROPERTY_SET_H
#define QTN_PROPERTY_DELEGATE_PROPERTY_SET_H

#include "PropertyDelegate.h"

class QtnPropertySet;

class QTN_PW_EXPORT QtnPropertyDelegatePropertySet: public QtnPropertyDelegateTyped<QtnPropertySet, QtnPropertyDelegate>
{
    Q_DISABLE_COPY(QtnPropertyDelegatePropertySet)

public:
    QtnPropertyDelegatePropertySet(QtnPropertySet& owner)
        : QtnPropertyDelegateTyped<QtnPropertySet, QtnPropertyDelegate>(owner)
    {
    }

protected:
    int subPropertyCountImpl() const override;
    QtnPropertyBase* subPropertyImpl(int index) override;

    //virtual void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) { Q_UNUSED(attributes); }

    void createSubItemsImpl(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems) override;
};


#endif // QTN_PROPERTY_DELEGATE_PROPERTY_SET_H
