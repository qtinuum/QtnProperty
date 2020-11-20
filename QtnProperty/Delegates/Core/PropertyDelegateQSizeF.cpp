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
#include "PropertyDelegateQSizeF.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Core/PropertyQSize.h"
#include "QtnProperty/PropertyDelegateAttrs.h"
#include "QtnProperty/Utils/DoubleSpinBox.h"

#include <QLineEdit>
#include <QLocale>

QtnPropertyDelegateQSizeF::QtnPropertyDelegateQSizeF(
	QtnPropertyQSizeFBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQSizeFBase>(owner)
	, m_precision(std::numeric_limits<qreal>::digits10 - 1)
{
	addSubProperty(owner.createWidthProperty());
	addSubProperty(owner.createHeightProperty());
}

void QtnPropertyDelegateQSizeF::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQSizeFBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQSizeF, QtnPropertyQSizeFBase>,
		"WH");
}

extern void qtnApplyQSizeDelegateAttributes(
	QtnPropertyDelegate *to, const QtnPropertyDelegateInfo &info);

void QtnPropertyDelegateQSizeF::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnPrecisionAttr(), m_precision);
	m_precision = qBound(0, m_precision, std::numeric_limits<qreal>::digits10);
	qtnApplyQSizeDelegateAttributes(this, info);
}

QWidget *QtnPropertyDelegateQSizeF::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQSizeF::propertyValueToStrImpl(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQSize::getToStringFormat().arg(
		QtnDoubleSpinBox::valueToText(value.width(), locale, m_precision, true),
		QtnDoubleSpinBox::valueToText(
			value.height(), locale, m_precision, true));

	return true;
}
