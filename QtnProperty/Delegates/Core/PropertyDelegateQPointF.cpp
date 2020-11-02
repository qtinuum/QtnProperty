/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyDelegateQPointF.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Core/PropertyQPoint.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QLineEdit>
#include <QLocale>

QtnPropertyDelegateQPointF::QtnPropertyDelegateQPointF(
	QtnPropertyQPointFBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQPointFBase>(owner)
	, m_multiplier(1.0)
	, m_precision(std::numeric_limits<qreal>::digits10)
{
	addSubProperty(owner.createXProperty());
	addSubProperty(owner.createYProperty());
}

void QtnPropertyDelegateQPointF::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQPointFBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQPointF, QtnPropertyQPointFBase>,
		"QPointF");
}

extern void qtnApplyQPointDelegateAttributes(
	QtnPropertyDelegate *to, const QtnPropertyDelegateInfo &info);

void QtnPropertyDelegateQPointF::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
	info.loadAttribute(qtnMultiplierAttr(), m_multiplier);
	info.loadAttribute(qtnPrecisionAttr(), m_precision);
	m_precision = qBound(0, m_precision, std::numeric_limits<qreal>::digits10);
	if (!qIsFinite(m_multiplier) || qFuzzyCompare(m_multiplier, 0.0))
	{
		m_multiplier = 1;
	}

	qtnApplyQPointDelegateAttributes(this, info);
}

QWidget *QtnPropertyDelegateQPointF::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQPointF::propertyValueToStrImpl(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQPoint::getToStringFormat().arg(
		locale.toString(value.x() * m_multiplier, 'g', m_precision) + m_suffix,
		locale.toString(value.y() * m_multiplier, 'g', m_precision) + m_suffix);

	return true;
}
