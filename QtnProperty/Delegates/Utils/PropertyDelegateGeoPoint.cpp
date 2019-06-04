/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include "PropertyDelegateGeoPoint.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "PropertyDelegateAttrs.h"

#include <QLineEdit>
#include <QCoreApplication>

QByteArray qtnGeoPointDelegateName()
{
	return QByteArrayLiteral("GeoPoint");
}

void QtnPropertyDelegateGeoPoint::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegate(&QtnPropertyQPointFBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateGeoPoint, QtnPropertyQPointFBase>,
		qtnGeoPointDelegateName());
}

QString QtnPropertyDelegateGeoPoint::longitudeKey()
{
	return QStringLiteral("longitude");
}

QString QtnPropertyDelegateGeoPoint::longitudeDisplayName()
{
	return QCoreApplication::instance()->translate(
		"GeoPoint", QT_TRANSLATE_NOOP("GeoPoint", "Longitude"));
}

QString QtnPropertyDelegateGeoPoint::longitudeDescriptionFmt()
{
	return QCoreApplication::instance()->translate(
		"GeoPoint", QT_TRANSLATE_NOOP("GeoPoint", "Longitude of %1"));
}

QString QtnPropertyDelegateGeoPoint::latitudeKey()
{
	return QStringLiteral("latitude");
}

QString QtnPropertyDelegateGeoPoint::latitudeDisplayName()
{
	return QCoreApplication::instance()->translate(
		"GeoPoint", QT_TRANSLATE_NOOP("GeoPoint", "Latitude"));
}

QString QtnPropertyDelegateGeoPoint::latitudeDescriptionFmt()
{
	return QCoreApplication::instance()->translate(
		"GeoPoint", QT_TRANSLATE_NOOP("GeoPoint", "Latitude of %1"));
}

QtnPropertyDelegateGeoPoint::QtnPropertyDelegateGeoPoint(
	QtnPropertyQPointFBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQPointFBase>(owner)
{
	QtnPropertyDelegateInfo geoDelegate;
	geoDelegate.name = qtnGeoCoordDelegateName();
	auto longitudeSubProperty =
		owner.createFieldProperty(longitudeKey(), longitudeDisplayName(),
			longitudeDescriptionFmt(), &QPointF::x, &QPointF::setX);
	longitudeSubProperty->setDelegateInfo(geoDelegate);

	auto latitudeSubProperty =
		owner.createFieldProperty(latitudeKey(), latitudeDisplayName(),
			latitudeDescriptionFmt(), &QPointF::y, &QPointF::setY);
	latitudeSubProperty->setDelegateInfo(geoDelegate);

	addSubProperty(longitudeSubProperty);
	addSubProperty(latitudeSubProperty);
}

void QtnPropertyDelegateGeoPoint::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	enum
	{
		X,
		Y,
		TOTAL
	};
	Q_ASSERT(subPropertyCount() == TOTAL);
	static const QtnSubPropertyInfo KEYS[TOTAL] = {
		{ X, longitudeKey(), qtnXDisplayNameAttr(), qtnXDescriptionAttr() },
		{ Y, latitudeKey(), qtnYDisplayNameAttr(), qtnYDescriptionAttr() },
	};

	for (auto &keys : KEYS)
	{
		applySubPropertyInfo(info, keys);
	}
}

QWidget *QtnPropertyDelegateGeoPoint::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

extern QString val2strGeoCoord(const double);

bool QtnPropertyDelegateGeoPoint::propertyValueToStrImpl(
	QString &strValue) const
{
	QPointF value = owner().value();
	strValue = QString("%1 x %2")
				   .arg(val2strGeoCoord(value.x()))
				   .arg(val2strGeoCoord(value.y()));
	return true;
}
