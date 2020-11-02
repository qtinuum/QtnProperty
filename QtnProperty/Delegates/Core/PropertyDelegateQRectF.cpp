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

#include "PropertyDelegateQRectF.h"
#include "QtnProperty/Core/PropertyQRect.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QLineEdit>

QtnPropertyDelegateQRectF::QtnPropertyDelegateQRectF(
	QtnPropertyQRectFBase &owner, bool useCoordinates)
	: QtnPropertyDelegateTypedEx<QtnPropertyQRectFBase>(owner)
	, m_coordinates(useCoordinates)
	, m_precision(std::numeric_limits<qreal>::digits10)
{
	addSubProperty(owner.createLeftProperty(!m_coordinates));
	addSubProperty(owner.createTopProperty(!m_coordinates));

	if (m_coordinates)
	{
		addSubProperty(owner.createRightProperty(false));
		addSubProperty(owner.createBottomProperty(false));
	} else
	{
		addSubProperty(owner.createWidthProperty());
		addSubProperty(owner.createHeightProperty());
	}
}

static QtnPropertyDelegate *qtnCreateDelegateLTRB(QtnPropertyBase &owner)
{
	auto theOwner = qobject_cast<QtnPropertyQRectFBase *>(&owner);
	if (!theOwner)
		return nullptr;

	return new QtnPropertyDelegateQRectF(*theOwner, true);
}

void QtnPropertyDelegateQRectF::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQRectFBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQRectF, QtnPropertyQRectFBase>,
		qtnLTWHDelegateName());

	factory.registerDelegate(&QtnPropertyQRectFBase::staticMetaObject,
		&qtnCreateDelegateLTRB, qtnLTRBDelegateName());
}

extern void qtnApplyQRectDelegateAttributes(QtnPropertyDelegate *to,
	const QtnPropertyDelegateInfo &info, bool coordinates);

void QtnPropertyDelegateQRectF::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnPrecisionAttr(), m_precision);
	m_precision = qBound(0, m_precision, std::numeric_limits<qreal>::digits10);
	qtnApplyQRectDelegateAttributes(this, info, m_coordinates);
}

QWidget *QtnPropertyDelegateQRectF::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQRectF::propertyValueToStrImpl(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue =
		QtnPropertyQRect::getToStringFormat(m_coordinates)
			.arg(locale.toString(value.left(), 'g', m_precision),
				locale.toString(value.top(), 'g', m_precision),
				locale.toString(m_coordinates ? value.right() : value.width(),
					'g', m_precision),
				locale.toString(m_coordinates ? value.bottom() : value.height(),
					'g', m_precision));

	return true;
}
