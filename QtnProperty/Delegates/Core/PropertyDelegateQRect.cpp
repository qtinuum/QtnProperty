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

#include "PropertyDelegateQRect.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Core/PropertyQSize.h"
#include "PropertyDelegateAttrs.h"

#include <QLineEdit>

QByteArray qtnLTWHDelegateName()
{
	return QByteArrayLiteral("LTWH");
}

QByteArray qtnLTRBDelegateName()
{
	return QByteArrayLiteral("LTRB");
}

QByteArray qtnLeftDisplayNameAttr()
{
	return QByteArrayLiteral("leftDisplayName");
}

QByteArray qtnLeftDescriptionAttr()
{
	return QByteArrayLiteral("leftDescription");
}

QByteArray qtnTopDisplayNameAttr()
{
	return QByteArrayLiteral("topDisplayName");
}

QByteArray qtnTopDescriptionAttr()
{
	return QByteArrayLiteral("topDescription");
}

QByteArray qtnRightDisplayNameAttr()
{
	return QByteArrayLiteral("rightDisplayName");
}

QByteArray qtnRightDescriptionAttr()
{
	return QByteArrayLiteral("rightDescription");
}

QByteArray qtnBottomDisplayNameAttr()
{
	return QByteArrayLiteral("bottomDisplayName");
}

QByteArray qtnBottomDescriptionAttr()
{
	return QByteArrayLiteral("bottomDescription");
}

QByteArray qtnWidthDisplayNameAttr()
{
	return QByteArrayLiteral("widthDisplayName");
}

QByteArray qtnWidthDescriptionAttr()
{
	return QByteArrayLiteral("widthDescription");
}

QByteArray qtnHeightDisplayNameAttr()
{
	return QByteArrayLiteral("heightDisplayName");
}

QByteArray qtnHeightDescriptionAttr()
{
	return QByteArrayLiteral("heightDescription");
}

QtnPropertyDelegateQRect::QtnPropertyDelegateQRect(
	QtnPropertyQRectBase &owner, bool useCoordinates)
	: QtnPropertyDelegateTypedEx<QtnPropertyQRectBase>(owner)
	, m_coordinates(useCoordinates)
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
	auto theOwner = qobject_cast<QtnPropertyQRectBase *>(&owner);
	if (!theOwner)
		return nullptr;

	return new QtnPropertyDelegateQRect(*theOwner, true);
}

void QtnPropertyDelegateQRect::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQRectBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQRect, QtnPropertyQRectBase>,
		qtnLTWHDelegateName());

	factory.registerDelegate(&QtnPropertyQRectBase::staticMetaObject,
		&qtnCreateDelegateLTRB, qtnLTRBDelegateName());
}

void qtnApplyQRectDelegateAttributes(QtnPropertyDelegate *to,
	const QtnPropertyDelegateInfo &info, bool coordinates)
{
	enum
	{
		LEFT = 0,
		TOP = 1,
		WIDTH = 2,
		HEIGHT = 3,
		RIGHT = 2,
		BOTTOM = 3,
		TOTAL
	};
	Q_ASSERT(to->subPropertyCount() == TOTAL);
	static const QtnSubPropertyInfo LTRB[TOTAL] = {
		{ LEFT, QtnPropertyQRect::leftKey(), qtnLeftDisplayNameAttr(),
			qtnLeftDescriptionAttr() },
		{ TOP, QtnPropertyQRect::topKey(), qtnTopDisplayNameAttr(),
			qtnTopDescriptionAttr() },
		{ RIGHT, QtnPropertyQRect::rightKey(), qtnRightDisplayNameAttr(),
			qtnRightDescriptionAttr() },
		{ BOTTOM, QtnPropertyQRect::bottomKey(), qtnBottomDisplayNameAttr(),
			qtnBottomDescriptionAttr() },
	};

	static const QtnSubPropertyInfo LTWH[TOTAL] = {
		{ LEFT, QtnPropertyQRect::leftKey(), qtnLeftDisplayNameAttr(),
			qtnLeftDescriptionAttr() },
		{ TOP, QtnPropertyQRect::topKey(), qtnTopDisplayNameAttr(),
			qtnTopDescriptionAttr() },
		{ WIDTH, QtnPropertyQSize::widthKey(), qtnWidthDisplayNameAttr(),
			qtnWidthDescriptionAttr() },
		{ HEIGHT, QtnPropertyQSize::heightKey(), qtnHeightDisplayNameAttr(),
			qtnHeightDescriptionAttr() },
	};

	const QtnSubPropertyInfo *KEYS = coordinates ? LTRB : LTWH;
	for (int i = 0; i < TOTAL; i++)
	{
		auto &keys = KEYS[i];

		to->applySubPropertyInfo(info, keys);
	}
}

void QtnPropertyDelegateQRect::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	qtnApplyQRectDelegateAttributes(this, info, m_coordinates);
}

QWidget *QtnPropertyDelegateQRect::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQRect::propertyValueToStrImpl(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue =
		QtnPropertyQRect::getToStringFormat(m_coordinates)
			.arg(locale.toString(value.left()), locale.toString(value.top()),
				locale.toString(m_coordinates ? value.right() : value.width()),
				locale.toString(
					m_coordinates ? value.bottom() : value.height()));

	return true;
}
