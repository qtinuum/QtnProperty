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

#include "PropertyDelegateGeoPoint.h"
#include "../../../Core/Core/PropertyQPointF.h"
#include "../PropertyDelegateFactory.h"

void regGeoPointDelegates()
{
    QtnPropertyDelegateFactory::staticInstance().registerDelegate(&QtnPropertyQPointFBase::staticMetaObject
                 , &qtnCreateDelegate<PropertyDelegateGeoPoint, QtnPropertyQPointFBase>
                 , "GeoPoint");
}

void regGeoPointDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegate(&QtnPropertyQPointFBase::staticMetaObject
                 , &qtnCreateDelegate<PropertyDelegateGeoPoint, QtnPropertyQPointFBase>
                 , "GeoPoint");
}


PropertyDelegateGeoPoint::PropertyDelegateGeoPoint(QtnPropertyQPointFBase& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyQPointFBase>(owner)
{
    QtnPropertyDelegateInfo geoDelegate;
    geoDelegate.name = "GeoCoord";
    m_longitudeSubProperty = qtnCreateXProperty(0, &owner);
    m_longitudeSubProperty->setDelegate(geoDelegate);
    m_longitudeSubProperty->setDisplayName(QObject::tr("Longitude"));

    m_latitudeSubProperty = qtnCreateYProperty(0, &owner);
    m_latitudeSubProperty->setDelegate(geoDelegate);
    m_latitudeSubProperty->setDisplayName(QObject::tr("Latitude"));
    addSubProperty(m_longitudeSubProperty);
    addSubProperty(m_latitudeSubProperty);
}

void PropertyDelegateGeoPoint::applyAttributesImpl(const QtnPropertyDelegateAttributes &attributes)
{
    QString tmp;

    if (qtnGetAttribute(attributes, "XDisplayName", tmp))
        m_longitudeSubProperty->setDisplayName(tmp);

    if (qtnGetAttribute(attributes, "YDisplayName", tmp))
        m_latitudeSubProperty->setDisplayName(tmp);
}

QWidget* PropertyDelegateGeoPoint::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}


QString val2strGeoCoord(const qreal);

bool PropertyDelegateGeoPoint::propertyValueToStrImpl(QString& strValue) const
{
    QPointF value = owner().value();
    strValue = QString("%1 x %2").arg(val2strGeoCoord(value.x())).arg(val2strGeoCoord(value.y()));
    return true;
}
