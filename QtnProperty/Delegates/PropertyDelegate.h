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

#ifndef QTN_PROPERTY_DELEGATE_H
#define QTN_PROPERTY_DELEGATE_H

#include "QtnProperty/CoreAPI.h"
#include "QtnProperty/Property.h"

#include <functional>
#include <QStylePainter>

class QKeyEvent;
class QtnPropertyDelegateFactory;
class QLineEdit;

class QTN_IMPORT_EXPORT QtnInplaceInfo
{
public:
	QEvent *activationEvent;

	QtnInplaceInfo();
};

QTN_IMPORT_EXPORT QString qtnElidedText(const QPainter &painter,
	const QString &text, const QRect &rect, bool *elided = 0);

class QTN_IMPORT_EXPORT QtnPropertyDelegate
{
	Q_DISABLE_COPY(QtnPropertyDelegate)

public:
	virtual ~QtnPropertyDelegate();

	// for complex properties like PropertyQFont
	inline int subPropertyCount() const;
	inline QtnPropertyBase *subProperty(int index);

	// tune up with attributes
	inline void applyAttributes(
		const QtnPropertyDelegateAttributes &attributes);

	inline void drawValue(QStylePainter &painter, const QRect &rect,
		const QStyle::State &state, bool *needTooltip = nullptr) const;

	inline QString toolTip() const;

	inline bool acceptKeyPressedForInplaceEdit(QKeyEvent *keyEvent) const;

	QWidget *createValueEditor(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr);

	// override this if property value can be displayed as string
	virtual bool propertyValueToStr(QString &strValue) const;

	inline QtnProperty *getOwnerProperty() const;

protected:
	QtnPropertyDelegate(QtnProperty *ownerProperty);

	virtual int subPropertyCountImpl() const;

	virtual QtnPropertyBase *subPropertyImpl(int index);

	virtual void applyAttributesImpl(
		const QtnPropertyDelegateAttributes &attributes);

	virtual void drawValueImpl(QStylePainter &painter, const QRect &rect,
		const QStyle::State &state, bool *needTooltip = nullptr) const;
	virtual QString toolTipImpl() const;
	virtual bool acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const;
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) = 0;

	// helper functions
	static void drawValueText(const QString &text, QStylePainter &painter,
		const QRect &rect, const QStyle::State &state,
		bool *needTooltip = nullptr);
	QLineEdit *createValueEditorLineEdit(QWidget *parent, const QRect &rect,
		bool readOnly, QtnInplaceInfo *inplaceInfo = nullptr);

	QtnProperty *ownerProperty;
};

int QtnPropertyDelegate::subPropertyCount() const
{
	return subPropertyCountImpl();
}

QtnPropertyBase *QtnPropertyDelegate::subProperty(int index)
{
	return subPropertyImpl(index);
}

void QtnPropertyDelegate::applyAttributes(
	const QtnPropertyDelegateAttributes &attributes)
{
	applyAttributesImpl(attributes);
}

void QtnPropertyDelegate::drawValue(QStylePainter &painter, const QRect &rect,
	const QStyle::State &state, bool *needTooltip) const
{
	drawValueImpl(painter, rect, state, needTooltip);
}

QString QtnPropertyDelegate::toolTip() const
{
	return toolTipImpl();
}

bool QtnPropertyDelegate::acceptKeyPressedForInplaceEdit(
	QKeyEvent *keyEvent) const
{
	return acceptKeyPressedForInplaceEditImpl(keyEvent);
}

QtnProperty *QtnPropertyDelegate::getOwnerProperty() const
{
	return ownerProperty;
}

template <typename PropertyClass>
class QtnPropertyDelegateTyped : public QtnPropertyDelegate
{
	Q_DISABLE_COPY(QtnPropertyDelegateTyped)

public:
	inline const PropertyClass &owner() const
	{
		return m_owner;
	}

	inline PropertyClass &owner()
	{
		return m_owner;
	}

protected:
	QtnPropertyDelegateTyped(PropertyClass &owner)
		: QtnPropertyDelegate(reinterpret_cast<QtnProperty *>(&owner))
		, m_owner(owner)
	{
	}

private:
	PropertyClass &m_owner;
};

template <typename PropertyClass>
class QtnPropertyDelegateTypedEx
	: public QtnPropertyDelegateTyped<PropertyClass>
{
	Q_DISABLE_COPY(QtnPropertyDelegateTypedEx)

protected:
	QtnPropertyDelegateTypedEx(PropertyClass &owner)
		: QtnPropertyDelegateTyped<PropertyClass>(owner)
	{
	}

	virtual int subPropertyCountImpl() const override
	{
		return m_subProperties.size();
	}

	virtual QtnPropertyBase *subPropertyImpl(int index) override
	{
		return m_subProperties[index].data();
	}

	void addSubProperty(QtnPropertyBase *subProperty)
	{
		Q_ASSERT(subProperty);

		if (!subProperty)
			return;

		m_subProperties.append(QSharedPointer<QtnPropertyBase>(subProperty));

		subProperty->connectMasterState(
			QtnPropertyDelegateTyped<PropertyClass>::getOwnerProperty());
	}

	QList<QSharedPointer<QtnPropertyBase>> m_subProperties;
};

#endif // QTN_PROPERTY_DELEGATE_H
