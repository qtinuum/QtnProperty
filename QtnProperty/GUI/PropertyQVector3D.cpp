/*******************************************************************************
Copyright (c) 2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
#include "PropertyQVector3D.h"

#include "QtnProperty/Core/PropertyQPoint.h"

QtnPropertyQVector3DBase::QtnPropertyQVector3DBase(QObject *parent)
	: ParentClass(parent)
{
}

QtnProperty *QtnPropertyQVector3DBase::createXProperty()
{
	return createFieldProperty(&QVector3D::x, &QVector3D::setX,
		QtnPropertyQPoint::xKey(), QtnPropertyQPoint::xDisplayName(),
		QtnPropertyQPoint::xDescriptionFmt());
}

QtnProperty *QtnPropertyQVector3DBase::createYProperty()
{
	return createFieldProperty(&QVector3D::y, &QVector3D::setY,
		QtnPropertyQPoint::yKey(), QtnPropertyQPoint::yDisplayName(),
		QtnPropertyQPoint::yDescriptionFmt());
}

QtnProperty *QtnPropertyQVector3DBase::createZProperty()
{
	return createFieldProperty(&QVector3D::z, &QVector3D::setZ,
		QtnPropertyQVector3D::zKey(), QtnPropertyQVector3D::zDisplayName(),
		QtnPropertyQVector3D::zDescriptionFmt());
}

static const auto sPrefix = QStringLiteral("QVector3D");

bool QtnPropertyQVector3DBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	auto text = str.trimmed();
	if (!text.startsWith(sPrefix))
	{
		return false;
	}
	text = text.mid(sPrefix.length()).trimmed();

	if (!text.startsWith("(") || !text.endsWith(")"))
	{
		return false;
	}

	text = text.mid(1, text.length() - 1).trimmed();
	auto params = text.split(QChar(','));
	if (params.size() != 3)
	{
		return false;
	}

	bool ok = false;
	float x = params.at(0).trimmed().toFloat(&ok);

	if (!ok)
		return false;

	float y = params.at(1).trimmed().toFloat(&ok);

	if (!ok)
		return false;

	float z = params.at(2).trimmed().toFloat(&ok);

	if (!ok)
		return false;

	return setValue(QVector3D(x, y, z), reason);
}

bool QtnPropertyQVector3DBase::toStrImpl(QString &str) const
{
	QVector3D v = value();
	str = QStringLiteral("QVector3D(%1, %2, %2)")
			  .arg(v.x())
			  .arg(v.y())
			  .arg(v.z());
	return true;
}

QtnPropertyQVector3D::QtnPropertyQVector3D(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQVector3DBase>(parent)
{
}

QString QtnPropertyQVector3D::zKey()
{
	return QStringLiteral("z");
}

QString QtnPropertyQVector3D::zDisplayName()
{
	return tr("Z");
}

QString QtnPropertyQVector3D::zDescriptionFmt()
{
	return tr("Z of the %1");
}

QString QtnPropertyQVector3D::getToStringFormat()
{
	return tr("[%1, %2, %3]");
}

QtnPropertyQVector3DCallback::QtnPropertyQVector3DCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQVector3DBase>(parent)
{
}
