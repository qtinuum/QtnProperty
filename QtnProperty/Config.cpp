#include "Config.h"

#include "PropertyInt64.h"
#include "PropertyUInt64.h"
#include "PropertyPercent.h"
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
	QtnPropertyPercent::Register();
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


