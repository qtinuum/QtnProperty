/*******************************************************************************
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

#include "Config.h"

#include "PropertyInt64.h"
#include "PropertyUInt64.h"
#include "PropertyQPointF.h"
#include "PropertyQSizeF.h"
#include "PropertyQRectF.h"
#include "PropertyQVariant.h"
#include "PropertyQKeySequence.h"
#include "Delegates/Core/PropertyDelegateBool.h"
#include "Delegates/Core/PropertyDelegateDouble.h"
#include "Delegates/Core/PropertyDelegateInt.h"
#include "Delegates/Core/PropertyDelegateUInt.h"
#include "Delegates/Core/PropertyDelegateEnum.h"
#include "Delegates/Core/PropertyDelegateEnumFlags.h"
#include "Delegates/Core/PropertyDelegateQPoint.h"
#include "Delegates/Core/PropertyDelegateQRect.h"
#include "Delegates/Core/PropertyDelegateQSize.h"
#include "Delegates/Core/PropertyDelegateQString.h"
#include "Delegates/GUI/PropertyDelegateQColor.h"
#include "Delegates/GUI/PropertyDelegateQFont.h"
#include "MultiProperty.h"
#include "QObjectPropertySet.h"
#include "Utils/AccessibilityProxy.h"

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>

#include <map>

static bool Register()
{
	Q_INIT_RESOURCE(QtnProperty);

	qRegisterMetaType<QtnPropertyChangeReason>("QtnPropertyChangeReason");
	qRegisterMetaType<QtnPropertyBase *>("QtnPropertyBase*");
	qRegisterMetaType<QtnPropertySet *>("QtnPropertySet*");
	qRegisterMetaType<QtnAccessibilityProxy *>("QtnAccessibilityProxy*");

	bool ok = qtnRegisterDefaultMetaPropertyFactory() && //
		QtnPropertyDelegateBoolCheck::Register() && //
		QtnPropertyDelegateBoolCombobox::Register() && //
		QtnPropertyDelegateDouble::Register() && //
		QtnPropertyDelegateInt::Register() && //
		QtnPropertyDelegateUInt::Register() && //
		QtnPropertyDelegateEnum::Register() && //
		QtnPropertyDelegateEnumFlags::Register() && //
		QtnPropertyDelegateQPoint::Register() && //
		QtnPropertyDelegateQRect::Register() && //
		QtnPropertyDelegateQSize::Register() && //
		QtnPropertyDelegateQString::Register() && //
		QtnPropertyDelegateQStringFile::Register() && //
		QtnPropertyDelegateQStringList::Register() && //
		QtnPropertyDelegateQColor::Register() && //
		QtnPropertyDelegateQFont::Register() && //
		QtnPropertyInt64::Register() && //
		QtnPropertyUInt64::Register() && //
		QtnPropertyQPointF::Register() && //
		QtnPropertyQSizeF::Register() && //
		QtnPropertyQRectF::Register() && //
		QtnPropertyQVariant::Register() && //
		QtnPropertyQKeySequence::Register() && //
		QtnMultiProperty::Register();

	Q_ASSERT(ok);
	return ok;
}

static const bool qtnReg = Register();

void qtnInstallTranslations(const QLocale &locale)
{
	static QTranslator translator;
	if (translator.load(locale, "QtnProperty.qm", "", ":/Translations"))
	{
		static bool installOnce = false;
		if (not installOnce)
		{
			installOnce = true;
			QCoreApplication::installTranslator(&translator);
		}
	}
}
