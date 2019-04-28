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

#include "PropertyDelegateQRectF.h"
#include "../../../Core/Core/PropertyQRectF.h"
#include "../PropertyDelegateFactory.h"

#include <QLineEdit>

void regQRectFDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyQRectFBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateQRectF, QtnPropertyQRectFBase>
			     , "LTWH");
}

QtnPropertyDelegateQRectF::QtnPropertyDelegateQRectF(QtnPropertyQRectFBase& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyQRectFBase>(owner)
{
    addSubProperty(qtnCreateLeftProperty(0, &owner));
    addSubProperty(qtnCreateTopProperty(0, &owner));
    addSubProperty(qtnCreateWidthProperty(0, &owner));
    addSubProperty(qtnCreateHeightProperty(0, &owner));
}

QWidget* QtnPropertyDelegateQRectF::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQRectF::propertyValueToStrImpl(QString& strValue) const
{
    QRectF value = owner().value();
    strValue = QString("[(%1, %2), %3 x %4]").arg(value.left()).arg(value.top()).arg(value.width()).arg(value.height());
    return true;
}
