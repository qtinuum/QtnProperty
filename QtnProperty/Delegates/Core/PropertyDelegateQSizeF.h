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

#ifndef PROPERTY_DELEGATE_QSIZEF_H
#define PROPERTY_DELEGATE_QSIZEF_H

#include "../Utils/PropertyDelegateMisc.h"

class QtnPropertyQSizeFBase;

class QTN_PW_EXPORT QtnPropertyDelegateQSizeF: public QtnPropertyDelegateTypedEx<QtnPropertyQSizeFBase>
{
    Q_DISABLE_COPY(QtnPropertyDelegateQSizeF)

public:
    QtnPropertyDelegateQSizeF(QtnPropertyQSizeFBase& owner);

protected:
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
    bool propertyValueToStrImpl(QString& strValue) const override;
};

#endif // PROPERTY_DELEGATE_QSIZEF_H
