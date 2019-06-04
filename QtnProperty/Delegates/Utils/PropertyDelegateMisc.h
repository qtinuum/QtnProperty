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

#ifndef QTN_PROPERTY_DELEGATE_MISC_H
#define QTN_PROPERTY_DELEGATE_MISC_H

#include "QtnProperty/Delegates/PropertyDelegate.h"
#include <deque>

class QTN_IMPORT_EXPORT QtnInplaceInfo
{
public:
	QEvent *activationEvent;

	QtnInplaceInfo();
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateWithValues
	: public QtnPropertyDelegate
{
	Q_DISABLE_COPY(QtnPropertyDelegateWithValues)

protected:
	QtnPropertyDelegateWithValues(QtnPropertyBase &owner);

	// override to define value part of property item
	virtual void createSubItemValuesImpl(QtnDrawContext &context,
		const QRect &valueRect, QList<QtnSubItem> &subItems) = 0;
	virtual void createSubItemsImpl(
		QtnDrawContext &context, QList<QtnSubItem> &subItems) override;

	// sub-items functions
	void addSubItemBackground(
		QtnDrawContext &context, QList<QtnSubItem> &subItems);
	void addSubItemSelection(
		QtnDrawContext &context, QList<QtnSubItem> &subItems);
	void addSubItemBranchNode(
		QtnDrawContext &context, QList<QtnSubItem> &subItems);
	void addSubItemName(QtnDrawContext &context, QList<QtnSubItem> &subItems);
	void addSubItemReset(QtnDrawContext &context, QList<QtnSubItem> &subItems);
	void addSubItemValues(QtnDrawContext &context, QList<QtnSubItem> &subItems);
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateWithValue
	: public QtnPropertyDelegateWithValues
{
	Q_DISABLE_COPY(QtnPropertyDelegateWithValue)

protected:
	QtnPropertyDelegateWithValue(QtnPropertyBase &owner);

	// override to define value part of property item
	virtual bool createSubItemValueImpl(
		QtnDrawContext &context, QtnSubItem &subItemValue) = 0;
	void createSubItemValuesImpl(QtnDrawContext &context,
		const QRect &valueRect, QList<QtnSubItem> &subItems) override;
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateWithValueEditor
	: public QtnPropertyDelegateWithValue
{
	Q_DISABLE_COPY(QtnPropertyDelegateWithValueEditor)

public:
	inline QWidget *createValueEditor(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr);
	inline void drawValue(QStylePainter &painter, const QRect &rect) const;
	inline bool acceptKeyPressedForInplaceEdit(QKeyEvent *keyEvent) const;

protected:
	QtnPropertyDelegateWithValueEditor(QtnPropertyBase &owner);

	// override if property value can be displayed as string
	virtual bool propertyValueToStrImpl(QString &strValue) const;
	// override if you need a custom tool tip
	virtual bool toolTipImpl(QString &strValue) const;

	bool createSubItemValueImpl(
		QtnDrawContext &context, QtnSubItem &subItemValue) override;

	// override to draw property value or override propertyValueToStrImpl to draw value as text
	virtual void drawValueImpl(QStylePainter &painter, const QRect &rect) const;
	// override to filter key events that will activate property Editor
	virtual bool acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const;
	// override to implement property Editor
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) = 0;

	QLineEdit *createValueEditorLineEdit(QWidget *parent, const QRect &rect,
		bool readOnly, QtnInplaceInfo *inplaceInfo = nullptr) const;
};

QWidget *QtnPropertyDelegateWithValueEditor::createValueEditor(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	return createValueEditorImpl(parent, rect, inplaceInfo);
}

void QtnPropertyDelegateWithValueEditor::drawValue(
	QStylePainter &painter, const QRect &rect) const
{
	return drawValueImpl(painter, rect);
}

bool QtnPropertyDelegateWithValueEditor::acceptKeyPressedForInplaceEdit(
	QKeyEvent *keyEvent) const
{
	return acceptKeyPressedForInplaceEditImpl(keyEvent);
}

template <typename PropertyClass,
	typename DelegateClass = QtnPropertyDelegateWithValueEditor>
class QtnPropertyDelegateTyped : public DelegateClass
{
	Q_DISABLE_COPY(QtnPropertyDelegateTyped)

public:
	const PropertyClass &owner() const
	{
		return *static_cast<const PropertyClass *>(this->propertyImmutable());
	}
	PropertyClass &owner()
	{
		return *static_cast<PropertyClass *>(this->property());
	}

protected:
	QtnPropertyDelegateTyped(PropertyClass &owner)
		: DelegateClass(owner)
	{
	}
};

template <typename PropertyClass,
	typename DelegateClass = QtnPropertyDelegateWithValueEditor>
class QtnPropertyDelegateTypedEx
	: public QtnPropertyDelegateTyped<PropertyClass, DelegateClass>
{
	Q_DISABLE_COPY(QtnPropertyDelegateTypedEx)

protected:
	QtnPropertyDelegateTypedEx(PropertyClass &owner)
		: QtnPropertyDelegateTyped<PropertyClass, DelegateClass>(owner)
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

		m_subProperties.emplace_back(subProperty);

		subProperty->connectMasterState(this->property());
	}

protected:
	std::deque<QScopedPointer<QtnPropertyBase>> m_subProperties;
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateError
	: public QtnPropertyDelegateWithValue
{
	Q_DISABLE_COPY(QtnPropertyDelegateError)

public:
	QtnPropertyDelegateError(QtnPropertyBase &owner, const QString &error);

protected:
	bool createSubItemValueImpl(
		QtnDrawContext &context, QtnSubItem &subItemValue) override;

private:
	QString m_error;
};

QTN_IMPORT_EXPORT QtnPropertyDelegate *qtnCreateDelegateError(
	QtnPropertyBase &owner, QString error);

#endif // QTN_PROPERTY_DELEGATE_MISC_H
