/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
#include "PropertyDelegateAttrs.h"
#include "Utils/DoubleSpinBox.h"

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
	int m_precision;
	double m_multiplier;
	QColor m_boxFillColor;
	QString m_itemToolTip;
	QVariant m_min;
	QVariant m_max;
	QString m_suffix;

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
		return mv.isValid() ? mv.value<ValueTypeStore>()
							: this->owner().minValue();
	}

	ValueTypeStore maxValue() const
	{
		const QVariant &mv = this->m_max;
		return mv.isValid() ? mv.value<ValueTypeStore>()
							: this->owner().maxValue();
	}

	ValueTypeStore currentValue() const
	{
		return qBound(minValue(), this->owner().value(), maxValue());
	}

	template <typename T,
		typename std::enable_if<sizeof(T) < 8>::type * = nullptr>
	inline bool isHugeInterval() const
	{
		return false;
	}

	template <typename T,
		typename std::enable_if<std::is_integral<T>::value &&
			sizeof(T) == 8>::type * = nullptr>
	inline bool isHugeInterval() const
	{
		using IntervalType = typename PropertyClass::IntervalType;
		const quint64 originalInterval = IntervalType(maxValue() - minValue());
		const auto maxInterval = quint64(std::numeric_limits<qint64>::max());
		return originalInterval > maxInterval;
	}

	template <typename T,
		typename std::enable_if<std::is_floating_point<T>::value &&
			sizeof(T) >= 8>::type * = nullptr>
	inline bool isHugeInterval() const
	{
		const auto originalInterval = maxValue() - minValue();
		const auto maxInterval = double(std::numeric_limits<qint64>::max());
		return originalInterval > maxInterval;
	}

	template <typename T,
		typename std::enable_if<sizeof(T) < 8>::type * = nullptr>
	inline double interval(T to) const
	{
		using IntervalType = typename PropertyClass::IntervalType;
		return double(IntervalType(to - minValue()));
	}

	template <typename T,
		typename std::enable_if<std::is_integral<T>::value &&
			sizeof(T) == 8>::type * = nullptr>
	inline double interval(T to) const
	{
		quint64 currentInterval = quint64(to - minValue());
		if (isHugeInterval<T>())
		{
			if (to == maxValue())
			{
				return 1.0;
			}
			using IntervalType = typename PropertyClass::IntervalType;
			const quint64 originalInterval =
				IntervalType(maxValue() - minValue());

			auto halfCurrent = double(currentInterval / 2);
			auto halfOriginal = double(originalInterval / 2);
			return halfCurrent / halfOriginal;
		}

		return double(currentInterval);
	}

	template <typename T,
		typename std::enable_if<std::is_floating_point<T>::value &&
			sizeof(T) >= 8>::type * = nullptr>
	inline double interval(T to) const
	{
		if (isHugeInterval<T>())
		{
			if (to == maxValue())
			{
				return 1.0;
			}
			auto halfMin = minValue() * 0.5;
			auto halfMax = maxValue() * 0.5;
			auto halfTo = to * 0.5;

			const auto halfOriginal = halfMax - halfMin;

			auto halfCurrent = halfTo - halfMin;

			return halfCurrent / halfOriginal;
		}

		return double(to) - double(minValue());
	}

	virtual double propertyValuePart() const override
	{
		return interval(currentValue()) / interval(maxValue());
	}

	template <typename T,
		typename std::enable_if<std::is_floating_point<T>::value>::type * =
			nullptr>
	QString valueToStr(T value) const
	{
		if (this->m_multiplier == 1.0)
		{
			return QtnDoubleSpinBox::valueToText(value, QLocale(),
					   qBound(0, this->m_precision,
						   std::numeric_limits<T>::digits10),
					   true) +
				this->m_suffix;
		}

		return QtnDoubleSpinBox::valueToText(double(value) * this->m_multiplier,
				   QLocale(), this->m_precision, true) +
			this->m_suffix;
	}

	template <typename T,
		typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
	QString valueToStr(T value) const
	{
		if (this->m_multiplier == 1.0)
		{
			return QLocale().toString(value) + this->m_suffix;
		}

		return QtnDoubleSpinBox::valueToText(double(value) * this->m_multiplier,
				   QLocale(), this->m_precision, true) +
			this->m_suffix;
	}

	virtual QString valuePartToStr(double valuePart) const override
	{
		return valueToStr(partToValue<ValueTypeStore>(valuePart));
	}

	virtual void incrementPropertyValue(int steps) override
	{
		this->owner().incrementValue(this->editReason(), steps);
	}

	virtual void setPropertyValuePart(double valuePart) override
	{
		this->owner().setValue(
			partToValue<ValueTypeStore>(valuePart), this->editReason());
	}

	template <typename T,
		typename std::enable_if<std::is_floating_point<T>::value &&
			sizeof(T) >= 8>::type * = nullptr>
	ValueTypeStore partToValue(double valuePart) const
	{
		Q_ASSERT(valuePart >= 0.0);
		Q_ASSERT(valuePart <= 1.0);

		if (isHugeInterval<T>())
		{
			if (valuePart == 1.0)
			{
				return maxValue();
			}

			const auto originalInterval = maxValue() - minValue();

			auto halfOriginal = originalInterval * 0.5;
			auto halfMin = minValue() * 0.5;
			auto value = ValueTypeStore(valuePart * halfOriginal);
			return (value + halfMin) * 2.0;
		}
		auto value = ValueTypeStore(valuePart * interval(maxValue()));
		return value + minValue();
	}

	template <typename T,
		typename std::enable_if<std::is_integral<T>::value &&
			sizeof(T) == 8>::type * = nullptr>
	ValueTypeStore partToValue(double valuePart) const
	{
		Q_ASSERT(valuePart >= 0.0);
		Q_ASSERT(valuePart <= 1.0);

		ValueTypeStore value;
		if (isHugeInterval<T>())
		{
			if (valuePart == 1.0)
			{
				return maxValue();
			}

			using IntervalType = typename PropertyClass::IntervalType;
			const quint64 originalInterval =
				IntervalType(maxValue() - minValue());

			auto dividedInterval = originalInterval / 2;
			value = ValueTypeStore(valuePart * dividedInterval) * 2;
		} else
		{
			value = ValueTypeStore(valuePart * interval(maxValue()));
		}
		return value + minValue();
	}

	template <typename T,
		typename std::enable_if<sizeof(T) < 8>::type * = nullptr>
	ValueTypeStore partToValue(double valuePart) const
	{
		Q_ASSERT(valuePart >= 0.0);
		Q_ASSERT(valuePart <= 1.0);

		auto value = ValueTypeStore(valuePart * interval(maxValue()));
		return value + minValue();
	}
};

#endif // QTN_PROPERTY_DELEGATE_SLIDER_BOX_H
