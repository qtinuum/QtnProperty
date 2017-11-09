/*******************************************************************************
Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
#include "Core/PropertyQRect.h"
#include "Delegates/PropertyDelegateFactory.h"

#include <QLineEdit>
#include <QLocale>

QtnPropertyDelegateQRect::QtnPropertyDelegateQRect(QtnPropertyQRectBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQRectBase>(owner)
{
	addSubProperty(qtnCreateLeftProperty(0, &owner));
	addSubProperty(qtnCreateTopProperty(0, &owner));
	addSubProperty(qtnCreateWidthProperty(0, &owner));
	addSubProperty(qtnCreateHeightProperty(0, &owner));
}

bool QtnPropertyDelegateQRect::Register()
{
	return QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
		&QtnPropertyQRectBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQRect, QtnPropertyQRectBase>,
		QByteArrayLiteral("LTWH"));
}

QWidget *QtnPropertyDelegateQRect::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQRect::propertyValueToStr(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQRect::getToStringFormat().arg(
		locale.toString(value.left()), locale.toString(value.top()),
		locale.toString(value.width()), locale.toString(value.height()));

	return true;
}
