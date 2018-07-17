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

#include "PropertyQPointF.h"

#include "Delegates/PropertyDelegateFactory.h"
#include "PropertyDelegateAttrs.h"
#include "Core/PropertyQPoint.h"
#include "QObjectPropertySet.h"

#include <QLocale>
#include <QLineEdit>

QtnPropertyQPointFBase::QtnPropertyQPointFBase(QObject *parent)
	: ParentClass(parent)
{
	addState(QtnPropertyStateCollapsed);
}

QtnProperty *QtnPropertyQPointFBase::createXProperty()
{
	return createFieldProperty(
		getXLabel(), getXDescriptionFormat(), &QPointF::x, &QPointF::setX);
}

QtnProperty *QtnPropertyQPointFBase::createYProperty()
{
	return createFieldProperty(
		getYLabel(), getYDescriptionFormat(), &QPointF::y, &QPointF::setY);
}

QString QtnPropertyQPointFBase::getXLabel()
{
	return QtnPropertyQPointF::tr("X");
}

QString QtnPropertyQPointFBase::getXDescriptionFormat()
{
	return QtnPropertyQPointF::tr("X of the %1");
}

QString QtnPropertyQPointFBase::getYLabel()
{
	return QtnPropertyQPointF::tr("Y");
}

QString QtnPropertyQPointFBase::getYDescriptionFormat()
{
	return QtnPropertyQPointF::tr("Y of the %1");
}

bool QtnPropertyQPointFBase::fromStrImpl(const QString &str, bool edit)
{
	static QRegExp point_parser(QStringLiteral("\
\\s*\\[\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*,\
\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*\\]\\s*"));

	if (!point_parser.exactMatch(str))
		return false;

	bool ok;
	auto x = qreal(point_parser.cap(1).toDouble(&ok));

	if (!ok)
		return false;

	auto y = qreal(point_parser.cap(6).toDouble(&ok));

	if (!ok)
		return false;

	return setValue(QPointF(x, y), edit);
}

bool QtnPropertyQPointFBase::toStrImpl(QString &str) const
{
	auto v = value();
	str = QStringLiteral("[%1, %2]").arg(v.x()).arg(v.y());
	return true;
}

QtnPropertyQPointFCallback::QtnPropertyQPointFCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQPointFBase>(parent)
{
}

QtnPropertyQPointF::QtnPropertyQPointF(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQPointFBase>(parent)
{
}

bool QtnPropertyQPointF::Register()
{
	qtnRegisterMetaPropertyFactory(
		QVariant::PointF, qtnCreateFactory<QtnPropertyQPointFCallback>());

	return QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
		&QtnPropertyQPointFBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQPointF, QtnPropertyQPointFBase>,
		QByteArrayLiteral("QPointF"));
}

QtnPropertyDelegateQPointF::QtnPropertyDelegateQPointF(
	QtnPropertyQPointFBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQPointFBase>(owner)
{
	auto delegateCallback = [this]() -> const QtnPropertyDelegateInfo * {
		auto result = new QtnPropertyDelegateInfo;
		result->name = qtnSpinBoxDelegate();
		result->attributes = mAttributes;
		return result;
	};
	mPropertyX = owner.createXProperty();
	mPropertyY = owner.createYProperty();
	mPropertyX->setDelegateCallback(delegateCallback);
	mPropertyY->setDelegateCallback(delegateCallback);
	addSubProperty(mPropertyX);
	addSubProperty(mPropertyY);
}

void QtnPropertyDelegateQPointF::applyAttributesImpl(
	const QtnPropertyDelegateAttributes &attributes)
{
	mAttributes = attributes;
	qtnGetAttribute(mAttributes, qtnSuffixAttr(), mSuffix);
}

QWidget *QtnPropertyDelegateQPointF::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateQPointF::propertyValueToStr(QString &strValue) const
{
	auto value = owner().value();

	QLocale locale;
	strValue = QtnPropertyQPoint::getToStringFormat().arg(
		locale.toString(value.x(), 'g', 6) + mSuffix,
		locale.toString(value.y(), 'g', 6) + mSuffix);

	return true;
}
