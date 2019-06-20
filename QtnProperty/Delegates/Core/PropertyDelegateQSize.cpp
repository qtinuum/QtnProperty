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
#include "PropertyDelegateQSize.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QLineEdit>
#include <QLocale>

QtnPropertyDelegateQSize::QtnPropertyDelegateQSize(QtnPropertyQSizeBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQSizeBase>(owner)
{
	addSubProperty(owner.createWidthProperty());
	addSubProperty(owner.createHeightProperty());
}

void QtnPropertyDelegateQSize::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQSizeBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQSize, QtnPropertyQSizeBase>,
		"WH");
}

void qtnApplyQSizeDelegateAttributes(
	QtnPropertyDelegate *to, const QtnPropertyDelegateInfo &info)
{
	enum
	{
		WIDTH,
		HEIGHT,
		TOTAL
	};
	Q_ASSERT(to->subPropertyCount() == TOTAL);
	static const QtnSubPropertyInfo KEYS[TOTAL] = {
		{ WIDTH, QtnPropertyQSize::widthKey(), qtnWidthDisplayNameAttr(),
			qtnWidthDescriptionAttr() },
		{ HEIGHT, QtnPropertyQSize::heightKey(), qtnHeightDisplayNameAttr(),
			qtnHeightDescriptionAttr() },
	};

	for (auto &keys : KEYS)
	{
		to->applySubPropertyInfo(info, keys);
	}
}

void QtnPropertyDelegateQSize::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	qtnApplyQSizeDelegateAttributes(this, info);
}

QWidget *QtnPropertyDelegateQSize::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQSize::propertyValueToStrImpl(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQSize::getToStringFormat().arg(
		locale.toString(value.width()), locale.toString(value.height()));

	return true;
}
