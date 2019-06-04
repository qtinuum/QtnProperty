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

#include "PropertyDelegateQPoint.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "PropertyDelegateAttrs.h"

#include <QLocale>
#include <QLineEdit>

QByteArray qtnXDisplayNameAttr()
{
	return QByteArrayLiteral("xDisplayName");
}

QByteArray qtnXDescriptionAttr()
{
	return QByteArrayLiteral("xDescription");
}

QByteArray qtnYDisplayNameAttr()
{
	return QByteArrayLiteral("yDisplayName");
}

QByteArray qtnYDescriptionAttr()
{
	return QByteArrayLiteral("yDescription");
}

QtnPropertyDelegateQPoint::QtnPropertyDelegateQPoint(
	QtnPropertyQPointBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQPointBase>(owner)
{
	auto xProperty = owner.createXProperty();
	addSubProperty(xProperty);

	auto yProperty = owner.createYProperty();
	addSubProperty(yProperty);
}

void QtnPropertyDelegateQPoint::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQPointBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQPoint, QtnPropertyQPointBase>,
		"XY");
}

void qtnApplyQPointDelegateAttributes(
	QtnPropertyDelegate *to, const QtnPropertyDelegateInfo &info)
{
	enum
	{
		X,
		Y,
		TOTAL
	};
	Q_ASSERT(to->subPropertyCount() == TOTAL);
	static const QtnSubPropertyInfo KEYS[TOTAL] = {
		{ X, QtnPropertyQPoint::xKey(), qtnXDisplayNameAttr(),
			qtnXDescriptionAttr() },
		{ Y, QtnPropertyQPoint::yKey(), qtnYDisplayNameAttr(),
			qtnYDescriptionAttr() },
	};

	for (auto &keys : KEYS)
	{
		to->applySubPropertyInfo(info, keys);
	}
}

void QtnPropertyDelegateQPoint::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);

	qtnApplyQPointDelegateAttributes(this, info);
}

QWidget *QtnPropertyDelegateQPoint::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQPoint::propertyValueToStrImpl(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQPoint::getToStringFormat().arg(
		locale.toString(value.x()) + m_suffix,
		locale.toString(value.y()) + m_suffix);

	return true;
}
