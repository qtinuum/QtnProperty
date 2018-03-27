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

#include "PropertyDelegateQPoint.h"
#include "../../../Core/Core/PropertyQPoint.h"
#include "../PropertyDelegateFactory.h"

void regQPointDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyQPointBase::staticMetaObject
			     , &qtnCreateDelegate<QtnPropertyDelegateQPoint, QtnPropertyQPointBase>
			     , "XY");
}

QtnPropertyDelegateQPoint::QtnPropertyDelegateQPoint(QtnPropertyQPointBase& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyQPointBase>(owner)
{
    addSubProperty(qtnCreateXProperty(0, &owner));
    addSubProperty(qtnCreateYProperty(0, &owner));
}

QWidget* QtnPropertyDelegateQPoint::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQPoint::propertyValueToStrImpl(QString& strValue) const
{
    QPoint value = owner().value();
    strValue = QString("%1 x %2").arg(value.x()).arg(value.y());
    return true;
}
