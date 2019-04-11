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

#include "PropertyQSizeF.h"

#include "Delegates/PropertyDelegateFactory.h"
#include "Core/PropertyQSize.h"
#include "QObjectPropertySet.h"

#include <QLocale>
#include <QLineEdit>

QtnPropertyQSizeFBase::QtnPropertyQSizeFBase(QObject *parent)
	: ParentClass(parent)
{
	addState(QtnPropertyStateCollapsed);
}

QtnProperty *QtnPropertyQSizeFBase::createWidthProperty()
{
	return createFieldProperty(QtnPropertyQSize::widthString(),
		QtnPropertyQSize::widthDescription(), &QSizeF::width,
		&QSizeF::setWidth);
}

QtnProperty *QtnPropertyQSizeFBase::createHeightProperty()
{
	return createFieldProperty(QtnPropertyQSize::heightString(),
		QtnPropertyQSize::heightDescription(), &QSizeF::height,
		&QSizeF::setHeight);
}

bool QtnPropertyQSizeFBase::fromStrImpl(const QString &str, bool edit)
{
	static QRegExp sizeParser(QStringLiteral("\
\\s*\\[\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*x\
\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*\\]\\s*"));

	if (!sizeParser.exactMatch(str))
		return false;

	bool ok;
	auto width = qreal(sizeParser.cap(1).toDouble(&ok));

	if (!ok)
		return false;

	auto height = qreal(sizeParser.cap(6).toDouble(&ok));

	if (!ok)
		return false;

	return setValue(QSizeF(width, height), edit);
}

bool QtnPropertyQSizeFBase::toStrImpl(QString &str) const
{
	auto v = value();
	str = QStringLiteral("[%1 x %2]").arg(v.width()).arg(v.height());
	return true;
}

QtnPropertyQSizeFCallback::QtnPropertyQSizeFCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQSizeFBase>(parent)
{
}

QtnPropertyQSizeF::QtnPropertyQSizeF(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQSizeFBase>(parent)
{
}

bool QtnPropertyQSizeF::Register()
{
	qtnRegisterMetaPropertyFactory(
		QVariant::SizeF, qtnCreateFactory<QtnPropertyQSizeFCallback>());

	return QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
		&QtnPropertyQSizeFBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQSizeF, QtnPropertyQSizeFBase>,
		QByteArrayLiteral("QSizeF"));
}

QtnPropertyDelegateQSizeF::QtnPropertyDelegateQSizeF(
	QtnPropertyQSizeFBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQSizeFBase>(owner)
{
	addSubProperty(owner.createWidthProperty());
	addSubProperty(owner.createHeightProperty());
}

QWidget *QtnPropertyDelegateQSizeF::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQSizeF::propertyValueToStr(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQSize::getToStringFormat().arg(
		locale.toString(value.width(), 'g', 15),
		locale.toString(value.height(), 'g', 15));

	return true;
}
