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

#include "PropertyQRect.h"

#include "PropertyQSize.h"

static QByteArray qtnQRect_LTRB()
{
	returnQByteArrayLiteral("QRect_LTRB");
}
static QByteArray qtnQRect_LTWH()
{
	returnQByteArrayLiteral("QRect_LTWH");
}

void QtnPropertyQRectBase::setMode(bool coordinates)
{
	this->coordinates = coordinates;
}

QtnProperty *QtnPropertyQRectBase::createLeftProperty(bool move)
{
	return createFieldProperty(QtnPropertyQRect::leftString(),
		QtnPropertyQRect::leftDescription(), &QRect::left,
		move ? &QRect::moveLeft : &QRect::setLeft);
}

QtnProperty *QtnPropertyQRectBase::createTopProperty(bool move)
{
	return createFieldProperty(QtnPropertyQRect::topString(),
		QtnPropertyQRect::topDescription(), &QRect::top,
		move ? &QRect::moveTop : &QRect::setTop);
}

QtnProperty *QtnPropertyQRectBase::createRightProperty(bool move)
{
	return createFieldProperty(QtnPropertyQRect::rightString(),
		QtnPropertyQRect::rightDescription(), &QRect::right,
		move ? &QRect::moveRight : &QRect::setRight);
}

QtnProperty *QtnPropertyQRectBase::createBottomProperty(bool move)
{
	return createFieldProperty(QtnPropertyQRect::bottomString(),
		QtnPropertyQRect::bottomDescription(), &QRect::bottom,
		move ? &QRect::moveBottom : &QRect::setBottom);
}

QtnProperty *QtnPropertyQRectBase::createWidthProperty()
{
	return createFieldProperty(QtnPropertyQSize::widthString(),
		QtnPropertyQSize::widthDescription(), &QRect::width, &QRect::setWidth);
}

QtnProperty *QtnPropertyQRectBase::createHeightProperty()
{
	return createFieldProperty(QtnPropertyQSize::heightString(),
		QtnPropertyQSize::heightDescription(), &QRect::height,
		&QRect::setHeight);
}

QByteArray QtnPropertyQRectBase::qtnQRect_LTRB()
{
	returnQByteArrayLiteral("QRect_LTRB");
}

QByteArray QtnPropertyQRectBase::qtnQRect_LTWH()
{
	returnQByteArrayLiteral("QRect_LTWH");
}

QtnPropertyQRectBase::QtnPropertyQRectBase(QObject *parent)
	: ParentClass(parent)
{
	addState(QtnPropertyStateCollapsed);

	setMode(false);

	setDelegateInfoCallback([this]() -> QtnPropertyDelegateInfo {
		QtnPropertyDelegateInfo result;
		result.name = coordinates ? qtnQRect_LTRB() : qtnQRect_LTWH();
		return result;
	});
}

bool QtnPropertyQRectBase::fromStrImpl(const QString &str, bool edit)
{
	QRegExp *rect_parser;
	if (coordinates)
	{
		static QRegExp coordRectParser(QStringLiteral("\
\\s*\\[\\s*\\(\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*,\
\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\)\\s*,\
\\s*\\(\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*,\
\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\)\\s*\\]\\s*"));
		rect_parser = &coordRectParser;
	} else
	{
		static QRegExp simpleRectParser(QStringLiteral("\
\\s*\\[\\s*\\(\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\s*,\
\\s*(-?((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+)))\\)\\s*,\
\\s*((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+))\\s*x\
\\s*((\\d+\\.\\d*)|(\\d*\\.\\d+)|(\\d+))\\s*\\]\\s*"));
		rect_parser = &simpleRectParser;
	}

	if (!rect_parser->exactMatch(str))
		return false;

	bool ok;
	auto x = rect_parser->cap(1).toInt(&ok);

	if (!ok)
		return false;

	auto y = rect_parser->cap(6).toInt(&ok);

	if (!ok)
		return false;

	if (coordinates)
	{
		auto right = rect_parser->cap(11).toInt(&ok);

		if (!ok)
			return false;

		auto bottom = rect_parser->cap(15).toInt(&ok);

		if (!ok)
			return false;

		return setValue(QRect(x, y, right - x, bottom - y));
	}

	auto width = rect_parser->cap(11).toInt(&ok);

	if (!ok)
		return false;

	auto height = rect_parser->cap(15).toInt(&ok);

	if (!ok)
		return false;

	return setValue(QRect(x, y, width, height), edit);
}

bool QtnPropertyQRectBase::toStrImpl(QString &str) const
{
	auto v = value();

	if (coordinates)
	{
		str = QStringLiteral("[(%1, %2), (%3, %4)]")
				  .arg(v.left())
				  .arg(v.top())
				  .arg(v.right())
				  .arg(v.bottom());
	} else
	{
		str = QStringLiteral("[(%1, %2), %3 x %4]")
				  .arg(v.left())
				  .arg(v.top())
				  .arg(v.width())
				  .arg(v.height());
	}
	return true;
}

QtnPropertyQRect::QtnPropertyQRect(QObject *parent)
	: QtnSinglePropertyValue<QtnPropertyQRectBase>(parent)
{
}

QString QtnPropertyQRect::getToStringFormat(bool coordinates)
{
	return coordinates ? tr("[(%1, %2), (%3, %4)]") : tr("[(%1, %2) %3 x %4]");
}

QString QtnPropertyQRect::leftString()
{
	return tr("Left");
}

QString QtnPropertyQRect::leftDescription()
{
	return tr("Left position of the %1");
}

QString QtnPropertyQRect::topString()
{
	return tr("Top");
}

QString QtnPropertyQRect::topDescription()
{
	return tr("Top position of the %1");
}

QString QtnPropertyQRect::rightString()
{
	return tr("Right");
}

QString QtnPropertyQRect::rightDescription()
{
	return tr("Right position of the %1");
}

QString QtnPropertyQRect::bottomString()
{
	return tr("Bottom");
}

QString QtnPropertyQRect::bottomDescription()
{
	return tr("Bottom position of the %1");
}

QtnPropertyQRectCallback::QtnPropertyQRectCallback(QObject *parent)
	: QtnSinglePropertyCallback<QtnPropertyQRectBase>(parent)
{
}
