/*******************************************************************************
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

#include "Install.h"

#include "PropertyInt64.h"
#include "PropertyUInt64.h"
#include "PropertyQVariant.h"
#include "PropertyQKeySequence.h"
#include "Delegates/Core/PropertyDelegateBool.h"
#include "Delegates/Core/PropertyDelegateDouble.h"
#include "Delegates/Core/PropertyDelegateFloat.h"
#include "Delegates/Core/PropertyDelegateInt.h"
#include "Delegates/Core/PropertyDelegateUInt.h"
#include "Delegates/Core/PropertyDelegateEnum.h"
#include "Delegates/Core/PropertyDelegateEnumFlags.h"
#include "Delegates/Core/PropertyDelegateQPoint.h"
#include "Delegates/Core/PropertyDelegateQRect.h"
#include "Delegates/Core/PropertyDelegateQSize.h"
#include "Delegates/Core/PropertyDelegateQString.h"
#include "Delegates/Core/PropertyDelegateQPointF.h"
#include "Delegates/Core/PropertyDelegateQSizeF.h"
#include "Delegates/Core/PropertyDelegateQRectF.h"
#include "Delegates/GUI/PropertyDelegateQColor.h"
#include "Delegates/GUI/PropertyDelegateQFont.h"
#include "Delegates/GUI/PropertyDelegateQPen.h"
#include "Delegates/GUI/PropertyDelegateQBrush.h"
#include "MultiProperty.h"
#include "QObjectPropertySet.h"
#include "Utils/AccessibilityProxy.h"

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>

#include <map>

template <typename QtnPropertyRealCallback>
static QtnProperty *createRealNumberProperty(
	QObject *object, const QMetaProperty &metaProperty)
{
	using ValueTypeStore = typename QtnPropertyRealCallback::ValueTypeStore;

	auto property = new QtnPropertyRealCallback(nullptr);

	switch (metaProperty.revision())
	{
		case PERCENT_SUFFIX:
		{
			QtnPropertyDelegateInfo delegate;
			qtnInitPercentSpinBoxDelegate(delegate);
			property->setDelegateInfo(delegate);

			property->setCallbackValueGet(
				[object, metaProperty]() -> ValueTypeStore {
					return ValueTypeStore(
						metaProperty.read(object).toDouble() * 100.0);
				});

			property->setCallbackValueSet(
				[object, metaProperty](ValueTypeStore value, QtnPropertyChangeReason /*reason*/) {
					metaProperty.write(object, value / ValueTypeStore(100.0));
				});

			return property;
		}

		case DEGREE_SUFFIX:
		{
			QtnPropertyDelegateInfo delegate;
			qtnInitDegreeSpinBoxDelegate(delegate);
			property->setDelegateInfo(delegate);
			break;
		}
	}

	property->setCallbackValueGet([object, metaProperty]() -> ValueTypeStore {
		return ValueTypeStore(metaProperty.read(object).toDouble());
	});

	property->setCallbackValueSet([object, metaProperty](ValueTypeStore value, QtnPropertyChangeReason /*reason*/) {
		metaProperty.write(object, value);
	});

	return property;
}

static void qtnRegisterDefaultMetaPropertyFactory()
{
	qtnRegisterMetaPropertyFactory(
		QMetaType::Bool, qtnCreateFactory<QtnPropertyBoolCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QString, qtnCreateFactory<QtnPropertyQStringCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::Double, createRealNumberProperty<QtnPropertyDoubleCallback>);
	qtnRegisterMetaPropertyFactory(
		QMetaType::Float, createRealNumberProperty<QtnPropertyFloatCallback>);
	qtnRegisterMetaPropertyFactory(
		QMetaType::Int, qtnCreateFactory<QtnPropertyIntCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::UInt, qtnCreateFactory<QtnPropertyUIntCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::LongLong, qtnCreateFactory<QtnPropertyInt64Callback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::ULongLong, qtnCreateFactory<QtnPropertyUInt64Callback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QPoint, qtnCreateFactory<QtnPropertyQPointCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QPointF, qtnCreateFactory<QtnPropertyQPointFCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QRect, qtnCreateFactory<QtnPropertyQRectCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QRectF, qtnCreateFactory<QtnPropertyQRectFCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QSize, qtnCreateFactory<QtnPropertyQSizeCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QSizeF, qtnCreateFactory<QtnPropertyQSizeFCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QColor, qtnCreateFactory<QtnPropertyQColorCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QFont, qtnCreateFactory<QtnPropertyQFontCallback>());
	qtnRegisterMetaPropertyFactory(
		QMetaType::QPen, qtnCreateFactory<QtnPropertyQPenCallback>());
	qtnRegisterMetaPropertyFactory(qMetaTypeId<Qt::PenStyle>(),
		qtnCreateFactory<QtnPropertyQPenStyleCallback>());
	qtnRegisterMetaPropertyFactory(qMetaTypeId<Qt::BrushStyle>(),
		qtnCreateFactory<QtnPropertyQBrushStyleCallback>());
	qtnRegisterMetaPropertyFactory(QMetaType::QVariant,
		[](QObject *object,
			const QMetaProperty &metaProperty) -> QtnProperty * {
			return new QtnPropertyQVariantCallback(object, metaProperty);
		});
	qtnRegisterMetaPropertyFactory(QMetaType::QKeySequence,
		qtnCreateFactory<QtnPropertyQKeySequenceCallback>());
}

bool qtnPropertyRegister()
{
	Q_INIT_RESOURCE(QtnProperty);

	qRegisterMetaType<QtnPropertyChangeReason>();
	qRegisterMetaType<QtnPropertyBase *>();
	qRegisterMetaType<QtnPropertySet *>();
	qRegisterMetaType<QtnAccessibilityProxy *>();
	qRegisterMetaType<QtnMultiVariant>();

	qtnRegisterDefaultMetaPropertyFactory();
	return true;
}

void qtnPropertyInstallTranslations(const QLocale &locale)
{
	static QTranslator translator;
	QCoreApplication::removeTranslator(&translator);
	if (translator.load(locale, "QtnProperty.qm", "", ":/Translations"))
	{
		QCoreApplication::installTranslator(&translator);
	}
}
