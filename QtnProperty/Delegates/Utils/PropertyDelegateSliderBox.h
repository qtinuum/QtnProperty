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

#ifndef QTN_PROPERTY_DELEGATE_SLIDER_BOX_H
#define QTN_PROPERTY_DELEGATE_SLIDER_BOX_H

#include "PropertyDelegateMisc.h"

class QVariantAnimation;

class QTN_PW_EXPORT QtnPropertyDelegateSlideBox: public QtnPropertyDelegateWithValue
{
    Q_DISABLE_COPY(QtnPropertyDelegateSlideBox)

protected:
    QtnPropertyDelegateSlideBox();
    ~QtnPropertyDelegateSlideBox();

    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    bool createSubItemValueImpl(QtnDrawContext& context, QtnSubItem& subItemValue) override;

    virtual void draw(QtnDrawContext& context, const QtnSubItem& item);
    virtual bool event(QtnEventContext& context, const QtnSubItem& item);

    float dragValuePart() const { return m_dragValuePart; }

    virtual float propertyValuePart() const = 0;
    virtual QString valuePartToStr(float valuePart) const = 0;
    virtual void incrementPropertyValue(int steps) = 0;
    virtual void setPropertyValuePart(float valuePart) = 0;

    QColor m_boxFillColor;
    bool m_liveUpdate;
    bool m_drawBorder;
    bool m_updateByScroll;
    bool m_animate;
    QString m_itemToolTip;

private:
    void updateDragValuePart(int x, const QRect& rect);
    void onPropertyWillChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue);
    void onPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);
    void onAnimationChanged(const QVariant& value);

    float m_dragValuePart;
    QCursor m_oldCursor;

    QScopedPointer<QVariantAnimation> m_animation;
    float m_oldValuePart;
    QWidget* m_animateWidget;
    QMetaObject::Connection m_c1;
    QMetaObject::Connection m_c2;
};

template <typename PropertyClass>
class QtnPropertyDelegateSlideBoxTyped: public QtnPropertyDelegateTyped<PropertyClass, QtnPropertyDelegateSlideBox>
{
    Q_DISABLE_COPY(QtnPropertyDelegateSlideBoxTyped)

public:
    QtnPropertyDelegateSlideBoxTyped(PropertyClass& owner)
        : QtnPropertyDelegateTyped<PropertyClass, QtnPropertyDelegateSlideBox>(owner)
    {}

protected:
    typedef typename PropertyClass::ValueType ValueType;

    float propertyValuePart() const override
    {
        ValueType valueInterval = this->owner().maxValue() - this->owner().minValue();
        if (valueInterval <= 0)
            return -1.f;

        return float(this->owner().value() - this->owner().minValue())/valueInterval;
    }
    QString valuePartToStr(float valuePart) const
    {
        return QString::number(partToValue(valuePart));
    }
    void incrementPropertyValue(int steps)
    {
        this->owner().incrementValue(steps);
    }

    void setPropertyValuePart(float valuePart)
    {
        this->owner().setValue(partToValue(valuePart));
    }

    ValueType partToValue(float valuePart) const
    {
        float value = this->owner().minValue() + ValueType(valuePart * (this->owner().maxValue() - this->owner().minValue()));
        return static_cast<int>(value/this->owner().stepValue())*this->owner().stepValue();
    }
};

#endif // QTN_PROPERTY_DELEGATE_SLIDER_BOX_H
