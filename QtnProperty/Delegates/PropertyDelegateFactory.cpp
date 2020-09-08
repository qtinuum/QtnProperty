/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
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

#include "PropertyDelegateFactory.h"
#include "Utils/PropertyDelegateMisc.h"
#include "Utils/PropertyDelegatePropertySet.h"
#include "Utils/PropertyDelegateGeoCoord.h"
#include "Utils/PropertyDelegateGeoPoint.h"
#include "Core/PropertyDelegateBool.h"
#include "Core/PropertyDelegateInt.h"
#include "Core/PropertyDelegateUInt.h"
#include "Core/PropertyDelegateDouble.h"
#include "Core/PropertyDelegateFloat.h"
#include "Core/PropertyDelegateEnum.h"
#include "Core/PropertyDelegateEnumFlags.h"
#include "Core/PropertyDelegateQString.h"
#include "Core/PropertyDelegateQPoint.h"
#include "Core/PropertyDelegateQPointF.h"
#include "Core/PropertyDelegateQSize.h"
#include "Core/PropertyDelegateQSizeF.h"
#include "Core/PropertyDelegateQRect.h"
#include "Core/PropertyDelegateQRectF.h"
#include "GUI/PropertyDelegateQColor.h"
#include "GUI/PropertyDelegateQPen.h"
#include "GUI/PropertyDelegateQBrush.h"
#include "GUI/PropertyDelegateQFont.h"
#include "GUI/PropertyDelegateButton.h"
#include "QtnProperty/PropertyQKeySequence.h"
#include "QtnProperty/PropertyInt64.h"
#include "QtnProperty/PropertyUInt64.h"
#include "QtnProperty/MultiProperty.h"

#include <QDebug>

QtnPropertyDelegateFactory::QtnPropertyDelegateFactory(
	QtnPropertyDelegateFactory *superFactory)
	: m_superFactory(nullptr)
{
	setSuperFactory(superFactory);
}

void QtnPropertyDelegateFactory::setSuperFactory(
	QtnPropertyDelegateFactory *superFactory)
{
	Q_ASSERT(m_superFactory != this);
	m_superFactory = superFactory;
}

QtnPropertyDelegate *QtnPropertyDelegateFactory::createDelegate(
	QtnPropertyBase &owner)
{
	auto factory = this;

	while (factory)
	{
		auto result = factory->createDelegateInternal(owner);
		if (result)
		{
			result->setFactory(this);
			result->init();
			return result;
		}

		factory = factory->m_superFactory;
	}

	auto delegateInfo = owner.delegateInfo();
	QByteArray delegateName;

	if (delegateInfo)
		delegateName = delegateInfo->name;

	// create delegate stub
	if (delegateName.isEmpty())
	{
		qWarning() << "Cannot find default delegate for property"
				   << owner.name();
		qWarning() << "Did you forget to register default delegate for "
				   << owner.metaObject()->className() << "type?";
	} else
	{
		qWarning() << "Cannot find delegate with name" << delegateName
				   << "for property" << owner.name();
		qWarning() << "Did you forget to register" << delegateName
				   << "delegate for" << owner.metaObject()->className()
				   << "type?";
	}

	return qtnCreateDelegateError(owner,
		QString("Delegate <%1> unknown")
			.arg(QString::fromLatin1(delegateName)));
}

QtnPropertyDelegate *QtnPropertyDelegateFactory::createDelegateInternal(
	QtnPropertyBase &owner)
{
	const QMetaObject *metaObject = owner.metaObject();

	CreateFunction createFunction = nullptr;
	while (metaObject && !createFunction)
	{
		// try to find delegate factory by class name
		auto it = m_createItems.find(metaObject);

		if (it != m_createItems.end())
		{
			// try to find delegate factory by name
			const CreateItem &createItem = it.value();
			auto delegateInfo = owner.delegateInfo();
			QByteArray delegateName;

			if (delegateInfo)
				delegateName = delegateInfo->name;

			if (delegateName.isEmpty())
			{
				createFunction = createItem.defaultCreateFunction;
			} else
			{
				auto jt = createItem.createFunctions.find(delegateName);
				if (jt != createItem.createFunctions.end())
					createFunction = jt.value();
			}
		}

		metaObject = metaObject->superClass();
	}

	if (!createFunction)
		return nullptr;

	return createFunction(owner);
}

bool QtnPropertyDelegateFactory::registerDelegateDefault(
	const QMetaObject *propertyMetaObject, const CreateFunction &createFunction,
	const QByteArray &delegateName)
{
	Q_ASSERT(propertyMetaObject);
	Q_ASSERT(createFunction);

	// find or create creation record
	CreateItem &createItem = m_createItems[propertyMetaObject];
	// register default create function
	createItem.defaultCreateFunction = createFunction;

	if (!delegateName.isEmpty())
	{
		return registerDelegate(
			propertyMetaObject, createFunction, delegateName);
	}

	return true;
}

bool QtnPropertyDelegateFactory::registerDelegate(
	const QMetaObject *propertyMetaObject, const CreateFunction &createFunction,
	const QByteArray &delegateName)
{
	Q_ASSERT(propertyMetaObject);
	Q_ASSERT(createFunction);
	Q_ASSERT(!delegateName.isEmpty());

	// find or create creation record
	CreateItem &createItem = m_createItems[propertyMetaObject];
	// register create function
	createItem.createFunctions[delegateName] = createFunction;

	return true;
}

bool QtnPropertyDelegateFactory::unregisterDelegate(
	const QMetaObject *propertyMetaObject)
{
	Q_ASSERT(propertyMetaObject);

	auto it = m_createItems.find(propertyMetaObject);
	if (it == m_createItems.end())
		return false;

	m_createItems.erase(it);
	return true;
}

bool QtnPropertyDelegateFactory::unregisterDelegate(
	const QMetaObject *propertyMetaObject, const QByteArray &delegateName)
{
	Q_ASSERT(propertyMetaObject);
	Q_ASSERT(!delegateName.isEmpty());

	auto it = m_createItems.find(propertyMetaObject);
	if (it == m_createItems.end())
		return false;

	auto &createFunctions = it->createFunctions;
	auto it2 = createFunctions.find(delegateName);
	if (it2 == createFunctions.end())
		return false;

	createFunctions.erase(it2);
	return true;
}

void QtnPropertyDelegateFactory::registerDefaultDelegates(
	QtnPropertyDelegateFactory &factory)
{
	QtnPropertyDelegatePropertySet::Register(factory);
	QtnPropertyDelegateBoolCheck::Register(factory);
	QtnPropertyDelegateBoolCombobox::Register(factory);
	QtnPropertyDelegateInt::Register(factory);
	QtnPropertyDelegateUInt::Register(factory);
	QtnPropertyDelegateInt64::Register(factory);
	QtnPropertyDelegateUInt64::Register(factory);
	QtnPropertyDelegateDouble::Register(factory);
	QtnPropertyDelegateFloat::Register(factory);
	QtnPropertyDelegateEnum::Register(factory);
	QtnPropertyDelegateEnumFlags::Register(factory);
	QtnPropertyDelegateQString::Register(factory);
	QtnPropertyDelegateQStringFile::Register(factory);
	QtnPropertyDelegateQStringList::Register(factory);
	QtnPropertyDelegateQStringCallback::Register(factory);
	QtnPropertyDelegateQPoint::Register(factory);
	QtnPropertyDelegateQPointF::Register(factory);
	QtnPropertyDelegateQSize::Register(factory);
	QtnPropertyDelegateQSizeF::Register(factory);
	QtnPropertyDelegateQRect::Register(factory);
	QtnPropertyDelegateQRectF::Register(factory);
	QtnPropertyDelegateGeoCoord::Register(factory);
	QtnPropertyDelegateGeoPoint::Register(factory);
	QtnPropertyDelegateQColor::Register(factory);
	QtnPropertyDelegateQColorSolid::Register(factory);
	QtnPropertyDelegateQFont::Register(factory);
	QtnPropertyDelegateButton::Register(factory);
	QtnPropertyDelegateButtonLink::Register(factory);
	QtnPropertyDelegateQPenStyle::Register(factory);
	QtnPropertyDelegateQPen::Register(factory);
	QtnPropertyDelegateQBrushStyle::Register(factory);
	QtnPropertyDelegateQKeySequence::Register(factory);
	QtnMultiPropertyDelegate::Register(factory);
}

static QScopedPointer<QtnPropertyDelegateFactory> _staticInstance;

QtnPropertyDelegateFactory &QtnPropertyDelegateFactory::staticInstance()
{
	if (!_staticInstance)
	{
		_staticInstance.reset(new QtnPropertyDelegateFactory);
		registerDefaultDelegates(*_staticInstance.data());
	}
	return *_staticInstance.data();
}

void QtnPropertyDelegateFactory::resetDefaultInstance(
	QtnPropertyDelegateFactory *factory)
{
	if (_staticInstance)
	{
		auto currentFactory = factory;
		while (currentFactory)
		{
			if (currentFactory == _staticInstance.data())
			{
				_staticInstance.take();
				break;
			}
			currentFactory = currentFactory->superFactory();
		}
	}

	_staticInstance.reset(factory);
}
