/*******************************************************************************
Copyright (c) 2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyDelegateQVector3D.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Core/PropertyQPoint.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QLineEdit>
#include <QLocale>

QByteArray qtnZDisplayNameAttr()
{
	return QByteArrayLiteral("zDisplayName");
}

QByteArray qtnZDescriptionAttr()
{
	return QByteArrayLiteral("zDescription");
}

QtnPropertyDelegateQVector3D::QtnPropertyDelegateQVector3D(
	QtnPropertyQVector3DBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQVector3DBase>(owner)
	, m_multiplier(1.0)
	, m_precision(std::numeric_limits<float>::digits10)
{
	addSubProperty(owner.createXProperty());
	addSubProperty(owner.createYProperty());
	addSubProperty(owner.createZProperty());
}

void QtnPropertyDelegateQVector3D::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQVector3DBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQVector3D,
			QtnPropertyQVector3DBase>,
		QByteArrayLiteral("QVector3D"));
}

extern void qtnApplyQPointDelegateAttributes(
	QtnPropertyDelegate *to, const QtnPropertyDelegateInfo &info);

void QtnPropertyDelegateQVector3D::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
	info.loadAttribute(qtnMultiplierAttr(), m_multiplier);
	info.loadAttribute(qtnPrecisionAttr(), m_precision);
	m_precision = qBound(0, m_precision, std::numeric_limits<float>::digits10);
	if (!qIsFinite(m_multiplier) || qFuzzyCompare(m_multiplier, 0.0))
	{
		m_multiplier = 1.0;
	}

	enum
	{
		X,
		Y,
		Z,
		TOTAL
	};
	Q_ASSERT(subPropertyCount() == TOTAL);
	static const QtnSubPropertyInfo KEYS[TOTAL] = {
		{ X, QtnPropertyQPoint::xKey(), qtnXDisplayNameAttr(),
			qtnXDescriptionAttr() },
		{ Y, QtnPropertyQPoint::yKey(), qtnYDisplayNameAttr(),
			qtnYDescriptionAttr() },
		{ Z, QtnPropertyQVector3D::zKey(), qtnZDisplayNameAttr(),
			qtnZDescriptionAttr() },
	};

	applySubPropertyInfos(info, KEYS, TOTAL);
}

QWidget *QtnPropertyDelegateQVector3D::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQVector3D::propertyValueToStrImpl(
	QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQVector3D::getToStringFormat().arg(
		locale.toString(value.x() * m_multiplier, 'g', m_precision) + m_suffix,
		locale.toString(value.y() * m_multiplier, 'g', m_precision) + m_suffix,
		locale.toString(value.z() * m_multiplier, 'g', m_precision) + m_suffix);

	return true;
}
