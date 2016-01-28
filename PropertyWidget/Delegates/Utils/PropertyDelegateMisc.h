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

#ifndef QTN_PROPERTY_DELEGATE_MISC_H
#define QTN_PROPERTY_DELEGATE_MISC_H

#include "../PropertyDelegate.h"

class QTN_PW_EXPORT QtnInplaceInfo
{
public:
    QEvent* activationEvent;

    QtnInplaceInfo()
        : activationEvent(0)
    {
    }
};

class QTN_PW_EXPORT QtnPropertyDelegateWithValue: public QtnPropertyDelegate
{
    Q_DISABLE_COPY(QtnPropertyDelegateWithValue)

protected:
    QtnPropertyDelegateWithValue() {}

    void createSubItemsImpl(QtnDrawContext& context, QList<QtnSubItem>& subItems) override;

    // override to define value part of property item
    virtual bool createSubItemValueImpl(QtnDrawContext& context, QtnSubItem& subItemValue) = 0;

    // sub-items functions
    void addSubItemBackground(QtnDrawContext& context, QList<QtnSubItem>& subItems);
    void addSubItemSelection(QtnDrawContext& context, QList<QtnSubItem>& subItems);
    void addSubItemBranchNode(QtnDrawContext& context, QList<QtnSubItem>& subItems);
    void addSubItemName(QtnDrawContext& context, QList<QtnSubItem>& subItems);
    void addSubItemValue(QtnDrawContext& context, QList<QtnSubItem>& subItems);
};

class QTN_PW_EXPORT QtnPropertyDelegateWithValueEditor: public QtnPropertyDelegateWithValue
{
    Q_DISABLE_COPY(QtnPropertyDelegateWithValueEditor)

protected:
    QtnPropertyDelegateWithValueEditor() {}

    bool createSubItemValueImpl(QtnDrawContext& context, QtnSubItem& subItemValue) override;

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

class QTN_PW_EXPORT QtnPropertyDelegateError: public QtnPropertyDelegateWithValue
{
    Q_DISABLE_COPY(QtnPropertyDelegateError)

public:
    QtnPropertyDelegateError(QtnPropertyBase& owner, QString error)
        : m_owner(owner),
          m_error(error)
    {}

protected:
    QtnPropertyBase* propertyImpl() override { return &m_owner; }
    const QtnPropertyBase* propertyImmutableImpl() const override { return &m_owner; }
    bool createSubItemValueImpl(QtnDrawContext& context, QtnSubItem& subItemValue) override;

private:
    QtnPropertyBase& m_owner;
    QString m_error;
};

QTN_PW_EXPORT QtnPropertyDelegate* qtnCreateDelegateError(QtnPropertyBase& owner, QString error);

#endif // QTN_PROPERTY_DELEGATE_MISC_H
