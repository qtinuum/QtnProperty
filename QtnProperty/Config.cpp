/*
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Config.h"

#include "PropertyInt64.h"
#include "PropertyUInt64.h"
#include "PropertyQPointF.h"
#include "PropertyQSizeF.h"
#include "PropertyQRectF.h"
#include "PropertyQVariant.h"
#include "MultiProperty.h"

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>

#include <map>

int Register()
{
	Q_INIT_RESOURCE(QtnProperty);

	QtnPropertyInt64::Register();
	QtnPropertyUInt64::Register();
	QtnPropertyQPointF::Register();
	QtnPropertyQSizeF::Register();
	QtnPropertyQRectF::Register();
	QtnPropertyQVariant::Register();
	QtnMultiProperty::Register();

	return 0;
}

static const int qtnReg = Register();


void qtnInstallTranslations(const QLocale &locale)
{
	static std::unique_ptr<QTranslator> translator;

	if (nullptr == translator)
	{
		translator.reset(new QTranslator);
	} else
	{
		QCoreApplication::removeTranslator(translator.get());
	}

	if (translator->load(locale, "QtnProperty.qm", "", ":/Translations"))
		QCoreApplication::installTranslator(translator.get());
}


