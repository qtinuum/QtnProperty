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

#include "PropertyDelegateSliderBox.h"

#include "QtnProperty/PropertyView.h"
#include "QtnProperty/Auxiliary/PropertyMacro.h"
#include "QtnProperty/PropertyDelegateAttrs.h"
#include "QtnProperty/MultiProperty.h"

#include <QMouseEvent>
#include <QVariantAnimation>

QByteArray qtnFillColorAttr()
{
	return QByteArrayLiteral("fillColor");
}

QByteArray qtnLiveUpdateAttr()
{
	return QByteArrayLiteral("liveUpdate");
}

QByteArray qtnDrawBorderAttr()
{
	return QByteArrayLiteral("drawBorder");
}

QByteArray qtnUpdateByScrollAttr()
{
	return QByteArrayLiteral("updateByScroll");
}

QByteArray qtnAnimateAttr()
{
	return QByteArrayLiteral("animate");
}

QByteArray qtnToolTipAttr()
{
	return QByteArrayLiteral("toolTip");
}

QtnPropertyDelegateSlideBox::QtnPropertyDelegateSlideBox(QtnPropertyBase &owner)
	: QtnPropertyDelegateWithValue(owner)
	, m_liveUpdate(false)
	, m_drawBorder(true)
	, m_updateByScroll(true)
	, m_animate(false)
	, m_boxFillColor(QColor::fromRgb(200, 200, 255))
	, m_itemToolTip(QtnPropertyView::tr("Drag/Scroll mouse to change value"))
	, m_dragValuePart(0.0)
	, m_oldValuePart(0.0)
	, m_animateWidget(nullptr)
{
}

QtnPropertyDelegateSlideBox::~QtnPropertyDelegateSlideBox()
{
	m_animation.reset();
}

void QtnPropertyDelegateSlideBox::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnFillColorAttr(), m_boxFillColor);
	info.loadAttribute(qtnLiveUpdateAttr(), m_liveUpdate);
	info.loadAttribute(qtnDrawBorderAttr(), m_drawBorder);
	info.loadAttribute(qtnUpdateByScrollAttr(), m_updateByScroll);
	info.loadAttribute(qtnAnimateAttr(), m_animate);
	info.loadAttribute(qtnToolTipAttr(), m_itemToolTip);
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
	info.loadAttribute(qtnPrecisionAttr(), m_precision);
	info.loadAttribute(qtnMultiplierAttr(), m_multiplier);
	m_min = info.attributes.value(qtnMinAttr());
	m_max = info.attributes.value(qtnMaxAttr());

	m_precision = qBound(0, m_precision, std::numeric_limits<double>::digits10);
	if (!qIsFinite(m_multiplier) || qFuzzyCompare(m_multiplier, 0.0))
	{
		m_multiplier = 1.0;
	}
}

bool QtnPropertyDelegateSlideBox::createSubItemValueImpl(
	QtnDrawContext &context, QtnSubItem &subItemValue)
{
	subItemValue.trackState();
	subItemValue.setTextAsTooltip(m_itemToolTip);
	subItemValue.drawHandler =
		qtnMemFn(this, &QtnPropertyDelegateSlideBox::draw);
	subItemValue.eventHandler =
		qtnMemFn(this, &QtnPropertyDelegateSlideBox::event);

	if (m_animate)
	{
		m_dragValuePart = propertyValuePart();
		m_animation.reset(new QVariantAnimation());
		m_animateWidget = context.widget->viewport();
	}

	return true;
}

void QtnPropertyDelegateSlideBox::draw(
	QtnDrawContext &context, const QtnSubItem &item)
{
	if (stateProperty()->isMultiValue())
	{
		qtnDrawValueText(QtnMultiProperty::getMultiValuePlaceholder(),
			*context.painter, item.rect, context.style());
		return;
	}

	double valuePart = m_dragValuePart =
		(item.state() == QtnSubItemStatePushed ||
			(m_animate && m_animation->state() == QVariantAnimation::Running))
		? dragValuePart()
		: propertyValuePart();
	if (valuePart < 0.0)
		return;

	auto boxRect = item.rect;
	boxRect.adjust(2, 2, -2, -2);

	auto valueRect = boxRect;
	valueRect.setWidth(int(valuePart * valueRect.width()));

	auto &painter = *context.painter;

	painter.save();

	auto palette = painter.style()->standardPalette();
	auto colorGroup = stateProperty()->isEditableByUser() ? QPalette::Active
														  : QPalette::Disabled;

	painter.fillRect(boxRect, palette.color(colorGroup, QPalette::Background));
	painter.fillRect(valueRect, m_boxFillColor);

	painter.setPen(context.textColorFor(stateProperty()->isEditableByUser()));
	painter.drawRect(valueRect);
	if (m_drawBorder)
		painter.drawRect(boxRect);

	painter.restore();

	qtnDrawValueText(
		valuePartToStr(valuePart), painter, boxRect, context.style());
}

bool QtnPropertyDelegateSlideBox::event(
	QtnEventContext &context, const QtnSubItem &item, QtnPropertyToEdit *toEdit)
{
	switch (context.eventType())
	{
		case QEvent::KeyPress:
		{
			int key = context.eventAs<QKeyEvent>()->key();

			int increment = 0;

			if ((key == Qt::Key_Plus) || (key == Qt::Key_Equal))
				increment = 1;
			else if ((key == Qt::Key_Minus) || (key == Qt::Key_Underscore))
				increment = -1;
			else
				return false;

			toEdit->setup(property(), [this, increment]() -> QWidget * {
				incrementPropertyValueInternal(increment);
				return nullptr;
			});
			return true;
		}

		case QEvent::Wheel:
		{
			if (m_updateByScroll)
			{
				int steps =
					context.eventAs<QWheelEvent>()->angleDelta().y() / 120;
				toEdit->setup(property(), [this, steps]() -> QWidget * {
					incrementPropertyValueInternal(steps);
					return nullptr;
				});
				return true;
			}

			return false;
		}

		case QtnSubItemEvent::Activated:
		{
			m_oldCursor = context.widget->cursor();
			return true;
		}

		case QtnSubItemEvent::PressMouse:
		{
			if (!m_animate)
			{
				m_dragValuePart = toDragValuePart(
					context.eventAs<QtnSubItemEvent>()->x(), item.rect);
				context.updateWidget();
			}
			return true;
		}

		case QEvent::MouseMove:
		{
			if (item.state() == QtnSubItemStatePushed)
			{
				auto dragValuePart = toDragValuePart(
					context.eventAs<QMouseEvent>()->x(), item.rect);
				if (m_liveUpdate)
				{
					if (m_animate)
						m_animation->stop();
					m_dragValuePart = dragValuePart;
					toEdit->setup(
						property(), [this, dragValuePart]() -> QWidget * {
							setPropertyValuePart(dragValuePart);
							return nullptr;
						});
				} else if (!m_animate)
				{
					m_dragValuePart = dragValuePart;
					context.updateWidget();
				}
			}
			return true;
		}

		case QtnSubItemEvent::ReleaseMouse:
		{
			context.widget->setCursor(m_oldCursor);
			auto dragValuePart = toDragValuePart(
				context.eventAs<QtnSubItemEvent>()->x(), item.rect);
			toEdit->setup(property(), [this, dragValuePart]() -> QWidget * {
				dragTo(dragValuePart);
				return nullptr;
			});
			return true;
		}

		default:
			return false;
	}
}

void QtnPropertyDelegateSlideBox::incrementPropertyValueInternal(int steps)
{
	if (m_animate)
		m_animation->stop();
	incrementPropertyValue(steps);
	m_dragValuePart = propertyValuePart();
}

double QtnPropertyDelegateSlideBox::toDragValuePart(int x, const QRect &rect)
{
	double result = double(x - rect.left()) / rect.width();
	if (result < 0.0)
		result = 0.0;
	else if (result > 1.0)
		result = 1.0;
	return result;
}

void QtnPropertyDelegateSlideBox::dragTo(double value)
{
	if (m_animate)
		prepareAnimate();
	setPropertyValuePart(value);
	if (m_animate)
		startAnimate();
	else
		m_dragValuePart = value;
}

void QtnPropertyDelegateSlideBox::prepareAnimate()
{
	if (m_animation->state() == QVariantAnimation::Running)
	{
		m_oldValuePart = m_dragValuePart;
		m_animation->stop();
	} else
	{
		m_oldValuePart = m_dragValuePart = propertyValuePart();
	}
}

void QtnPropertyDelegateSlideBox::startAnimate()
{
	double startValue = m_oldValuePart;
	double endValue = propertyValuePart();
	if (endValue == startValue)
		return;
	m_animation->setStartValue(startValue);
	m_animation->setEndValue(endValue);
	m_animation->setDuration(300);
	m_animation->setEasingCurve(QEasingCurve::OutCirc);
	QObject::connect(m_animation.data(), &QVariantAnimation::valueChanged,
		qtnMemFn(this, &QtnPropertyDelegateSlideBox::onAnimationChanged));
	m_animation->start();
}

void QtnPropertyDelegateSlideBox::onAnimationChanged(const QVariant &value)
{
	m_dragValuePart = value.toDouble();
	if (m_animateWidget)
		m_animateWidget->update();
}
