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

#ifndef PROPERTY_DELEGATE_FREQ_H
#define PROPERTY_DELEGATE_FREQ_H

#include "Delegates/Utils/PropertyDelegateMisc.h"

class QtnPropertyFreqBase;

class QtnPropertyDelegateFreq: public QtnPropertyDelegateTyped<QtnPropertyFreqBase, QtnPropertyDelegateWithValues>
{
    Q_DISABLE_COPY(QtnPropertyDelegateFreq)

public:
    QtnPropertyDelegateFreq(QtnPropertyFreqBase& owner)
        : QtnPropertyDelegateTyped<QtnPropertyFreqBase, QtnPropertyDelegateWithValues>(owner)
    {
    }

protected:
    void createSubItemValuesImpl(QtnDrawContext& context, const QRect& rect, QList<QtnSubItem>& subItems) override;

private:
    void addValueSubItem(QtnDrawContext& context, const QRect& rect, QList<QtnSubItem>& subItems);
    void addUnitSubItem(QtnDrawContext& context, const QRect& rect, QList<QtnSubItem>& subItems);
};

#endif // PROPERTY_DELEGATE_FREQ_H
