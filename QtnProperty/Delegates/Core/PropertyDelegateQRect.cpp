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
	, coordinates(owner.coordinateMode())
{
	addSubProperty(owner.createLeftProperty(!coordinates));
	addSubProperty(owner.createTopProperty(!coordinates));

	if (coordinates)
	{
		addSubProperty(owner.createRightProperty(false));
		addSubProperty(owner.createBottomProperty(false));
	} else
	{
		addSubProperty(owner.createWidthProperty());
		addSubProperty(owner.createHeightProperty());
	}
}

bool QtnPropertyDelegateQRect::Register()
{
	return QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
			   &QtnPropertyQRectBase::staticMetaObject,
			   qtnCreateDelegate<QtnPropertyDelegateQRect,
				   QtnPropertyQRectBase>,
			   QtnPropertyQRectBase::qtnQRect_LTWH()) &&
		QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
			&QtnPropertyQRectBase::staticMetaObject,
			qtnCreateDelegate<QtnPropertyDelegateQRect, QtnPropertyQRectBase>,
			QtnPropertyQRectBase::qtnQRect_LTRB());
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
	strValue =
		QtnPropertyQRect::getToStringFormat(coordinates)
			.arg(locale.toString(value.left()), locale.toString(value.top()),
				locale.toString(coordinates ? value.right() : value.width()),
				locale.toString(coordinates ? value.bottom() : value.height()));

	return true;
}
