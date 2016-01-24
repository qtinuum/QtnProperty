/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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

#ifndef QTN_PROPERTY_DELEGATE_H
#define QTN_PROPERTY_DELEGATE_H

#include "../PropertyWidgetAPI.h"
#include "../../Core/Property.h"
#include "PropertyDelegateAux.h"

class QtnInplaceInfo;

class QTN_PW_EXPORT QtnPropertyDelegate
{
    Q_DISABLE_COPY(QtnPropertyDelegate)

public:
    virtual ~QtnPropertyDelegate() {}

    QtnPropertyBase* property();
    const QtnPropertyBase* propertyImmutable() const;

    // for complex properties like PropertyQFont
    int subPropertyCount() const;
    QtnPropertyBase* subProperty(int index);

    // tune up with attributes
    void applyAttributes(const QtnPropertyDelegateAttributes& attributes);
    // create GUI sub elements to present property on PropertyView
    void createSubItems(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems);

protected:
    QtnPropertyDelegate() {}

    virtual QtnPropertyBase* propertyImpl() = 0;
    virtual const QtnPropertyBase* propertyImmutableImpl() const = 0;
    virtual int subPropertyCountImpl() const { return 0; }
    virtual QtnPropertyBase* subPropertyImpl(int index) { Q_UNUSED(index); return nullptr; }

    virtual void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) { Q_UNUSED(attributes); }

    virtual void createSubItemsImpl(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems) = 0;

    // helper functions
    QStyle::State state(bool isActive, QtnPropertyDelegateSubItemState subState) const;
    static void drawValueText(const QString& text, QStylePainter& painter, const QRect& rect, QStyle::State state, bool* needTooltip = nullptr);
};

class QTN_PW_EXPORT QtnPropertyDelegateWithValue: public QtnPropertyDelegate
{
    Q_DISABLE_COPY(QtnPropertyDelegateWithValue)

protected:
    QtnPropertyDelegateWithValue() {}

    void createSubItemsImpl(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems) override;

    // override to define value part of property item
    virtual bool createSubItemValueImpl(QtnPropertyDelegateDrawContext& context, QtnPropertyDelegateSubItem& subItemValue) = 0;

    // sub-items functions
    void addSubItemBackground(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems);
    void addSubItemSelection(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems);
    void addSubItemBranchNode(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems);
    void addSubItemName(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems);
    void addSubItemValue(QtnPropertyDelegateDrawContext& context, QList<QtnPropertyDelegateSubItem>& subItems);
};

class QTN_PW_EXPORT QtnPropertyDelegateWithValueEditor: public QtnPropertyDelegateWithValue
{
    Q_DISABLE_COPY(QtnPropertyDelegateWithValueEditor)

protected:
    QtnPropertyDelegateWithValueEditor() {}

    bool createSubItemValueImpl(QtnPropertyDelegateDrawContext& context, QtnPropertyDelegateSubItem& subItemValue) override;

    // override to draw property value or override propertyValueToStrImpl to draw value as text
    virtual void drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip = nullptr) const;
//    virtual QString toolTipImpl() const;
    // override if property value can be displayed as string
    virtual bool propertyValueToStrImpl(QString& strValue) const { Q_UNUSED(strValue); return false; }
    // override to filter key events that will activate property Editor
    virtual bool acceptKeyPressedForInplaceEditImpl(QKeyEvent* keyEvent) const;
    // override to implement property Editor
    virtual QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) = 0;

    QWidget* createValueEditorLineEdit(QWidget* parent, const QRect& rect, bool readOnly, QtnInplaceInfo* inplaceInfo = nullptr);
};

template <typename PropertyClass, typename DelegateClass = QtnPropertyDelegateWithValueEditor>
class QtnPropertyDelegateTyped: public DelegateClass
{
    Q_DISABLE_COPY(QtnPropertyDelegateTyped)

public:
    const PropertyClass& owner() const { return m_owner; }
    PropertyClass& owner() { return m_owner; }

protected:
    QtnPropertyDelegateTyped(PropertyClass& owner)
        : m_owner(owner)
    {
    }

    ~QtnPropertyDelegateTyped()
    {
    }

    QtnPropertyBase* propertyImpl() override { return &m_owner; }
    const QtnPropertyBase* propertyImmutableImpl() const override { return &m_owner; }

private:
    PropertyClass& m_owner;
};

template <typename PropertyClass, typename DelegateClass = QtnPropertyDelegateWithValueEditor>
class QtnPropertyDelegateTypedEx: public QtnPropertyDelegateTyped<PropertyClass, DelegateClass>
{
    Q_DISABLE_COPY(QtnPropertyDelegateTypedEx)

protected:
    QtnPropertyDelegateTypedEx(PropertyClass& owner)
        : QtnPropertyDelegateTyped<PropertyClass>(owner)
    {
    }

    ~QtnPropertyDelegateTypedEx()
    {
    }

    int subPropertyCountImpl() const override { return m_subProperties.size(); }
    QtnPropertyBase* subPropertyImpl(int index) override { return m_subProperties[index].data(); }

    void addSubProperty(QtnPropertyBase* subProperty)
    {
        Q_ASSERT(subProperty);
        if (!subProperty)
            return;

        m_subProperties.append(QSharedPointer<QtnPropertyBase>(subProperty));
    }

private:
    QList<QSharedPointer<QtnPropertyBase>> m_subProperties;
};


class QTN_PW_EXPORT QtnInplaceInfo
{
public:
    QEvent* activationEvent;

    QtnInplaceInfo()
        : activationEvent(0)
    {
    }
};

QTN_PW_EXPORT QString qtnElidedText(const QPainter& painter, const QString& text, const QRect& rect, bool* elided = 0);


#endif // QTN_PROPERTY_DELEGATE_H
