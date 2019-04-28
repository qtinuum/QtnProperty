/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

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
#include <QDoubleSpinBox>

class QTN_PW_EXPORT QtnInplaceInfo
{
public:
    QEvent* activationEvent;

    QtnInplaceInfo()
        : activationEvent(0)
    {
    }
};

class QTN_PW_EXPORT QtnPropertyDelegateWithValues: public QtnPropertyDelegate
{
    Q_DISABLE_COPY(QtnPropertyDelegateWithValues)

protected:
    QtnPropertyDelegateWithValues() {}

    // override to define value part of property item
    virtual void createSubItemValuesImpl(QtnDrawContext& context, const QRect& valueRect, QList<QtnSubItem>& subItems) = 0;

    // sub-items functions
    void addSubItemBackground(QtnDrawContext& context, QList<QtnSubItem>& subItems);
    void addSubItemSelection(QtnDrawContext& context, QList<QtnSubItem>& subItems);
    void addSubItemBranchNode(QtnDrawContext& context, QList<QtnSubItem>& subItems);
    void addSubItemName(QtnDrawContext& context, QList<QtnSubItem>& subItems);
    void addSubItemReset(QtnDrawContext& context, QList<QtnSubItem>& subItems);
    void addSubItemValues(QtnDrawContext& context, QList<QtnSubItem>& subItems);

private:
    void createSubItemsImpl(QtnDrawContext& context, QList<QtnSubItem>& subItems) override;
};

class QTN_PW_EXPORT QtnPropertyDelegateWithValue: public QtnPropertyDelegateWithValues
{
    Q_DISABLE_COPY(QtnPropertyDelegateWithValue)

protected:
    QtnPropertyDelegateWithValue() {}

    // override to define value part of property item
    virtual bool createSubItemValueImpl(QtnDrawContext& context, QtnSubItem& subItemValue) = 0;

private:
    void createSubItemValuesImpl(QtnDrawContext& context, const QRect& valueRect, QList<QtnSubItem>& subItems) override;
};

class QTN_PW_EXPORT QtnPropertyDelegateWithValueEditor: public QtnPropertyDelegateWithValue
{
    Q_DISABLE_COPY(QtnPropertyDelegateWithValueEditor)

protected:
    QtnPropertyDelegateWithValueEditor() {}

    bool createSubItemValueImpl(QtnDrawContext& context, QtnSubItem& subItemValue) override;

    // override to draw property value or override propertyValueToStrImpl to draw value as text
    virtual void drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip = nullptr) const;
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

class QTN_PW_EXPORT QtnDoubleSpinBox : public QDoubleSpinBox
{
public:
    explicit QtnDoubleSpinBox(QWidget* parent);
    QString textFromValue(double val) const override;
};

class QTN_PW_EXPORT QtnSpinBoxUnsigned: public QSpinBox
{
    Q_OBJECT

public:
    explicit QtnSpinBoxUnsigned(QWidget* parent);

    void setUintRange(quint32 minValue, quint32 maxValue);
    void setUintSingleStep(quint32 stepValue);
    void setUintValue(quint32 value);

Q_SIGNALS:
    void uintValueChanged(quint32 value);

protected:
    using QSpinBox::setRange;
    using QSpinBox::setSingleStep;
    using QSpinBox::setValue;

    int valueFromText(const QString& text) const override;
    QString textFromValue(int val) const override;
    QValidator::State validate(QString &text, int &) const override;


private:
    void onValueChanged(int value);

    quint32 m_min;
    quint32 m_max;
};


#endif // QTN_PROPERTY_DELEGATE_MISC_H
