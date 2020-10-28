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

#ifndef QTN_PROPERTY_DELEGATE_SLIDER_BOX_H
#define QTN_PROPERTY_DELEGATE_SLIDER_BOX_H

#include "PropertyDelegateMisc.h"
#include "Delegates/PropertyDelegateAux.h"

class QVariantAnimation;

class QTN_IMPORT_EXPORT QtnPropertyDelegateSlideBox
	: public QtnPropertyDelegateWithValue
{
	Q_DISABLE_COPY(QtnPropertyDelegateSlideBox)

protected:
	QtnPropertyDelegateSlideBox(QtnPropertyBase &owner);
	virtual ~QtnPropertyDelegateSlideBox() override;

	void applyAttributesImpl(const QtnPropertyDelegateInfo &info) override;
	bool createSubItemValueImpl(
		QtnDrawContext &context, QtnSubItem &subItemValue) override;

	virtual void draw(QtnDrawContext &context, const QtnSubItem &item);
	virtual bool event(QtnEventContext &context, const QtnSubItem &item,
		QtnPropertyToEdit *toEdit);

	inline double dragValuePart() const;

	virtual double propertyValuePart() const = 0;
	virtual QString valuePartToStr(double valuePart) const = 0;
	virtual void incrementPropertyValue(int steps) = 0;
	virtual void setPropertyValuePart(double valuePart) = 0;

	void prepareAnimate();
	void startAnimate();

	bool m_liveUpdate;
	bool m_drawBorder;
	bool m_updateByScroll;
	bool m_animate;
	QColor m_boxFillColor;
	QString m_itemToolTip;
	QVariant m_min;
	QVariant m_max;

private:
	void incrementPropertyValueInternal(int steps);
	double toDragValuePart(int x, const QRect &rect);
	void dragTo(double value);
	void onAnimationChanged(const QVariant &value);

	double m_dragValuePart;
	double m_oldValuePart;

	QWidget *m_animateWidget;
	QCursor m_oldCursor;
	QScopedPointer<QVariantAnimation> m_animation;
};

double QtnPropertyDelegateSlideBox::dragValuePart() const
{
	return m_dragValuePart;
}

template <typename PropertyClass>
class QtnPropertyDelegateSlideBoxTyped
	: public QtnPropertyDelegateTyped<PropertyClass,
		  QtnPropertyDelegateSlideBox>
{
	Q_DISABLE_COPY(QtnPropertyDelegateSlideBoxTyped)

	using ValueTypeStore = typename PropertyClass::ValueTypeStore;

	using ParentClass =
		QtnPropertyDelegateTyped<PropertyClass, QtnPropertyDelegateSlideBox>;

public:
	QtnPropertyDelegateSlideBoxTyped(PropertyClass &owner)
		: ParentClass(owner)
	{
	}

protected:
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override
	{
		ParentClass::applyAttributesImpl(info);
		fixMinMaxVariant<ValueTypeStore>(this->m_min, this->m_max);
	}

	ValueTypeStore minValue() const
	{
		const QVariant &mv = this->m_min;
		return mv.isValid() ? mv.value<ValueTypeStore>() : owner().minValue();
	}

	ValueTypeStore maxValue() const
	{
		const QVariant &mv = this->m_max;
		return mv.isValid() ? mv.value<ValueTypeStore>() : owner().maxValue();
	}

	ValueTypeStore currentValue() const
	{
		return std::min(maxValue(), std::max(minValue(), owner().value()));
	}

	inline double interval(ValueTypeStore to) const
	{
		using IntervalType = typename PropertyClass::IntervalType;
		return double(IntervalType(to - minValue()));
	}

	virtual double propertyValuePart() const override
	{
		auto &p = this->owner();
		return interval(currentValue()) / interval(maxValue());
	}

	template <typename T,
		typename std::enable_if<std::is_floating_point<T>::value>::type * =
			nullptr>
	static QString valueToStr(T value)
	{
		return QLocale().toString(value, 'g', 5);
	}

	template <typename T,
		typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
	static QString valueToStr(T value)
	{
		return QLocale().toString(value);
	}

	virtual QString valuePartToStr(double valuePart) const override
	{
		return valueToStr(partToValue(valuePart));
	}

	virtual void incrementPropertyValue(int steps) override
	{
		this->owner().incrementValue(this->editReason(), steps);
	}

	virtual void setPropertyValuePart(double valuePart) override
	{
		this->owner().setValue(partToValue(valuePart), this->editReason());
	}

	ValueTypeStore partToValue(double valuePart) const
	{
		Q_ASSERT(valuePart >= 0.0);
		Q_ASSERT(valuePart <= 1.0);

		return ValueTypeStore(valuePart * interval(maxValue())) + minValue();
	}
};

#endif // QTN_PROPERTY_DELEGATE_SLIDER_BOX_H
