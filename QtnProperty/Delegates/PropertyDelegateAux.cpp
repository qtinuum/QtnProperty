/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyDelegateAux.h"
#include "QtnProperty/PropertyView.h"
#include "QtnProperty/MultiProperty.h"
#include "QtnProperty/Utils/InplaceEditing.h"
#include "QtnProperty/Utils/QtnConnections.h"

#include <QMouseEvent>
#include <QToolTip>

QtnSubItem::QtnSubItem(const QRect &rect)
	: rect(rect)
	, m_trackState(false)
	, m_activeCount(0)
	, m_state(QtnSubItemStateNone)
{
}

void QtnSubItem::setTextAsTooltip(const QString &text)
{
	tooltipHandler = [text](QtnEventContext &, const QtnSubItem &) -> QString {
		return text;
	};
}

void QtnSubItem::setPropertyNameAsTooltip(const QtnPropertyBase &property)
{
	tooltipHandler = [&property](
						 QtnEventContext &, const QtnSubItem &) -> QString {
		return property.displayName();
	};
}

void QtnSubItem::setPropertyDescriptionAsTooltip(
	const QtnPropertyBase &property)
{
	tooltipHandler = [&property](
						 QtnEventContext &, const QtnSubItem &) -> QString {
		auto descr = property.description();
		if (descr.isEmpty())
			return property.displayName();

		return descr;
	};
}

bool QtnSubItem::activate(QtnPropertyView *widget, QPoint mousePos)
{
	if (!m_trackState)
		return false;

	Q_ASSERT(m_activeCount <= 1);
	if (m_activeCount > 1)
		return false;

	++m_activeCount;

	if (m_state != QtnSubItemStateUnderCursor)
	{
		m_state = QtnSubItemStateUnderCursor;
		widget->viewport()->update();
		selfEvent(QtnSubItemEvent::Activated, widget, mousePos);
	}

	return true;
}

bool QtnSubItem::deactivate(QtnPropertyView *widget, QPoint mousePos)
{
	if (!m_trackState)
		return false;

	Q_ASSERT(m_activeCount > 0);
	if (m_activeCount == 0)
		return false;

	--m_activeCount;

	if ((m_activeCount == 0) && (m_state != QtnSubItemStateNone))
	{
		m_state = QtnSubItemStateNone;
		widget->viewport()->update();
		selfEvent(QtnSubItemEvent::Deactivated, widget, mousePos);
	}

	return true;
}

bool QtnSubItem::grabMouse(QtnPropertyView *widget, QPoint mousePos)
{
	Q_ASSERT(m_activeCount > 0);
	Q_ASSERT(m_state == QtnSubItemStateUnderCursor);

	m_state = QtnSubItemStatePushed;
	widget->viewport()->update();
	selfEvent(QtnSubItemEvent::PressMouse, widget, mousePos);

	return true;
}

bool QtnSubItem::releaseMouse(QtnPropertyView *widget, QPoint mousePos)
{
	Q_ASSERT(m_activeCount > 0);
	Q_ASSERT(m_state == QtnSubItemStatePushed);

	m_state = QtnSubItemStateUnderCursor;
	widget->viewport()->update();
	selfEvent(QtnSubItemEvent::ReleaseMouse, widget, mousePos);

	return true;
}

void QtnSubItem::draw(QtnDrawContext &context) const
{
	if (drawHandler)
		drawHandler(context, *this);
}

bool QtnSubItem::event(QtnEventContext &context)
{
	if (m_trackState)
	{
		switch (context.eventType())
		{
			case QEvent::MouseButtonPress:
			case QEvent::MouseButtonDblClick:
			{
				auto event = context.eventAs<QMouseEvent>();
				if (event->button() == Qt::LeftButton)
					context.grabMouse(this, event->pos());
				break;
			}

			case QEvent::MouseButtonRelease:
			{
				auto event = context.eventAs<QMouseEvent>();
				if ((event->button() == Qt::LeftButton) &&
					(m_state == QtnSubItemStatePushed))
					context.releaseMouse(this, event->pos());
				break;
			}

			default:
				break;
		}
	}

	if (context.eventType() == QEvent::ToolTip)
	{
		if (!tooltipHandler)
			return false;

		QString tooltipText = tooltipHandler(context, *this);
		if (!tooltipText.isEmpty())
		{
			auto event = context.eventAs<QHelpEvent>();
			QToolTip::showText(
				event->globalPos(), tooltipText, context.widget, rect);
			return true;
		}
	}

	if (eventHandler)
	{
		QtnPropertyToEdit propertyToEdit;
		bool result = eventHandler(context, *this, &propertyToEdit);
		if (propertyToEdit.isValid())
		{
			auto editProperty = propertyToEdit.property();
			bool editable = editProperty->isEditableByUser();
			std::shared_ptr<QtnConnections> connections;
			if (editable)
			{
				connections = std::make_shared<QtnConnections>();
				std::weak_ptr<QtnConnections> weakConnections = connections;
				connections->push_back(
					QObject::connect(editProperty, &QObject::destroyed,
						[weakConnections]() //
						{
							if (!weakConnections.expired())
								weakConnections.lock()->clear();
						}));
				context.widget->connectPropertyToEdit(
					editProperty, *connections.get());
			}
			auto editor = propertyToEdit.createEditor();
			if (editor)
			{
				if (editable)
				{
					QObject::connect(editor, &QObject::destroyed,
						[connections]() { connections->disconnect(); });
				}

				qtnStartInplaceEdit(editor);
			}
		}
		return result;
	}

	return false;
}

bool QtnSubItem::selfEvent(
	QtnSubItemEvent::Type type, QtnPropertyView *widget, QPoint mousePos)
{
	QtnSubItemEvent event_(type, mousePos);
	QtnEventContext context{ &event_, widget };
	return event(context);
}

QStyle *QtnDrawContext::style() const
{
	return widget->style();
}

void QtnDrawContext::initStyleOption(QStyleOption &option) const
{
	option.initFrom(widget);
	// State_MouseOver should be set explicitly
	option.state &= ~QStyle::State_MouseOver;
}

const QPalette &QtnDrawContext::palette() const
{
	return widget->palette();
}

QPalette::ColorGroup QtnDrawContext::colorGroup() const
{
	return palette().currentColorGroup();
}

QColor QtnDrawContext::alternateColor() const
{
	auto color = palette().color(QPalette::Button);
	if (color == palette().color(QPalette::Window))
	{
		color = color.darker(120);
	}
	return color;
}

QColor QtnDrawContext::textColorFor(bool normalText) const
{
	QPalette::ColorRole role;
	QPalette::ColorGroup group =
		normalText ? palette().currentColorGroup() : QPalette::Disabled;
	if (isActive)
	{
		role = QPalette::HighlightedText;
	} else if (normalText)
	{
		role = QPalette::Text;
	} else
	{
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
		group = QPalette::Active;
		role = QPalette::PlaceholderText;
#else
		group = QPalette::Disabled;
		role = QPalette::Text;
#endif
	}

	return palette().color(group, role);
}

void QtnEventContext::updateWidget()
{
	widget->viewport()->update();
}

bool QtnEventContext::grabMouse(QtnSubItem *subItem, QPoint mousePos)
{
	return widget->grabMouseForSubItem(subItem, mousePos);
}

bool QtnEventContext::releaseMouse(QtnSubItem *subItem, QPoint mousePos)
{
	return widget->releaseMouseForSubItem(subItem, mousePos);
}

QtnSubItemEvent::QtnSubItemEvent(Type type, QPoint mousePos)
	: QEvent((QEvent::Type) type)
	, m_mousePos(mousePos)
{
}

QString qtnElidedText(const QPainter &painter, const QString &text,
	const QRect &rect, bool *elided)
{
	QString newText =
		painter.fontMetrics().elidedText(text, Qt::ElideRight, rect.width());

	if (elided)
		*elided = (newText != text);

	return newText;
}

void qtnDrawValueText(
	const QString &text, QPainter &painter, const QRect &rect, QStyle *style)
{
	if (text.isEmpty())
		return;

	auto textRect = rect;
	if (style)
	{
		textRect.adjust(style->pixelMetric(QStyle::PM_ButtonMargin), 0, 0, 0);
	}

	painter.drawText(textRect, Qt::AlignLeading | Qt::AlignVCenter,
		qtnElidedText(painter, text, textRect, nullptr));
}

QtnPropertyToEdit::QtnPropertyToEdit()
	: m_property(nullptr)
{
}

QtnPropertyToEdit::QtnPropertyToEdit(const QtnPropertyToEdit &other)
{
	m_property = other.m_property;
	m_editorMaker = other.m_editorMaker;
}

void QtnPropertyToEdit::setup(
	QtnPropertyBase *property, const EditorMaker &editorMaker)
{
	Q_ASSERT(property);
	Q_ASSERT(editorMaker);
	m_property = property;
	m_editorMaker = editorMaker;
}

QWidget *QtnPropertyToEdit::createEditor() const
{
	if (!m_editorMaker)
		return nullptr;

	return m_editorMaker();
}

bool QtnPropertyToEdit::isValid() const
{
	return m_property && m_editorMaker;
}
