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

	QColor m_boxFillColor;
	bool m_liveUpdate;
	bool m_drawBorder;
	bool m_updateByScroll;
	bool m_animate;
	QString m_itemToolTip;

private:
	void updateDragValuePart(int x, const QRect &rect);
	void onPropertyWillChange(QtnPropertyChangeReason reason,
		QtnPropertyValuePtr newValue, int typeId);
	void onPropertyDidChange(QtnPropertyChangeReason reason);
	void onAnimationChanged(const QVariant &value);

	double m_dragValuePart;
	QCursor m_oldCursor;

	QScopedPointer<QVariantAnimation> m_animation;
	double m_oldValuePart;
	QWidget *m_animateWidget;
	QMetaObject::Connection m_c1;
	QMetaObject::Connection m_c2;
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

	typedef typename PropertyClass::ValueTypeStore ValueTypeStore;

public:
	QtnPropertyDelegateSlideBoxTyped(PropertyClass &owner)
		: QtnPropertyDelegateTyped<PropertyClass, QtnPropertyDelegateSlideBox>(
			  owner)
	{
	}

protected:
	inline double interval(ValueTypeStore to) const
	{
		auto &p = this->owner();
		using IntervalType = typename PropertyClass::IntervalType;
		return double(IntervalType(to - p.minValue()));
	}

	virtual double propertyValuePart() const override
	{
		auto &p = this->owner();
		return interval(p.value()) / interval(p.maxValue());
	}

	virtual QString valuePartToStr(double valuePart) const override
	{
		return QLocale().toString(partToValue(valuePart));
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

		auto &p = this->owner();

		double value = valuePart * interval(p.maxValue());
		value /= double(p.stepValue());
		value = std::floor(value) * double(p.stepValue());

		return ValueTypeStore(value) + p.minValue();
	}
};

#endif // QTN_PROPERTY_DELEGATE_SLIDER_BOX_H
