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

#ifndef QTN_PROPERTY_DELEGATE_H
#define QTN_PROPERTY_DELEGATE_H

#include "QtnProperty/Config.h"
#include "QtnProperty/Property.h"
#include "PropertyDelegateAux.h"

#include <QStylePainter>

class QtnPropertyDelegateFactory;
struct QtnSubPropertyInfo;

class QtnPropertyEditorHandlerBase;

class QTN_IMPORT_EXPORT QtnPropertyDelegate
{
	Q_DISABLE_COPY(QtnPropertyDelegate)

	friend class QtnPropertyEditorHandlerBase;
	QtnPropertyDelegateFactory *m_factory = nullptr;
	QtnPropertyEditorHandlerBase *m_editorHandler = nullptr;

public:
	virtual ~QtnPropertyDelegate();
	virtual void init();

	QtnPropertyChangeReason editReason() const;

	inline QtnPropertyBase *stateProperty() const;
	inline void setStateProperty(QtnPropertyBase *p);

	inline QtnPropertyBase *property();
	inline const QtnPropertyBase *propertyImmutable() const;

	inline QtnPropertyDelegateFactory *factory() const;
	inline void setFactory(QtnPropertyDelegateFactory *factory);

	// for complex properties like PropertyQFont
	inline int subPropertyCount() const;
	inline QtnPropertyBase *subProperty(int index);

	// tune up with attributes
	inline void applyAttributes(const QtnPropertyDelegateInfo &info);
	// create GUI sub elements to present property on PropertyView
	inline void createSubItems(
		QtnDrawContext &context, QList<QtnSubItem> &subItems);

	void applySubPropertyInfo(
		const QtnPropertyDelegateInfo &info, const QtnSubPropertyInfo &subInfo);

protected:
	QtnPropertyDelegate(QtnPropertyBase &ownerProperty);

	void drawButton(const QtnDrawContext &context, const QtnSubItem &item,
		const QIcon &icon, const QString &text);

	virtual int subPropertyCountImpl() const;
	virtual QtnPropertyBase *subPropertyImpl(int index);

	virtual void applyAttributesImpl(const QtnPropertyDelegateInfo &info);

	virtual void createSubItemsImpl(
		QtnDrawContext &context, QList<QtnSubItem> &subItems) = 0;

	// helper functions
	QStyle::State state(bool isActive, const QtnSubItem &subItem) const;

	void addLockItem(QtnDrawContext &context, QList<QtnSubItem> &subItems);

protected:
	QtnPropertyBase *m_ownerProperty;
	QtnPropertyBase *m_stateProperty;
};

QtnPropertyBase *QtnPropertyDelegate::stateProperty() const
{
	return m_stateProperty ? m_stateProperty : m_ownerProperty;
}

void QtnPropertyDelegate::setStateProperty(QtnPropertyBase *p)
{
	m_stateProperty = p;
}

QtnPropertyBase *QtnPropertyDelegate::property()
{
	return m_ownerProperty;
}

const QtnPropertyBase *QtnPropertyDelegate::propertyImmutable() const
{
	return m_ownerProperty;
}

QtnPropertyDelegateFactory *QtnPropertyDelegate::factory() const
{
	return m_factory;
}

void QtnPropertyDelegate::setFactory(QtnPropertyDelegateFactory *factory)
{
	m_factory = factory;
}

int QtnPropertyDelegate::subPropertyCount() const
{
	return subPropertyCountImpl();
}

QtnPropertyBase *QtnPropertyDelegate::subProperty(int index)
{
	return subPropertyImpl(index);
}

void QtnPropertyDelegate::applyAttributes(const QtnPropertyDelegateInfo &info)
{
	applyAttributesImpl(info);
}

void QtnPropertyDelegate::createSubItems(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	createSubItemsImpl(context, subItems);
}

#endif // QTN_PROPERTY_DELEGATE_H
