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

#ifndef QTN_PROPERTY_DELEGATE_AUX_H
#define QTN_PROPERTY_DELEGATE_AUX_H

#include "QtnProperty/FunctionalHelpers.h"

#include <QStyleOption>
#include <QStylePainter>
#include <QEvent>
#include <QVariant>

class QtnPropertyBase;
class QtnPropertyView;
struct QtnDrawContext;
struct QtnEventContext;

class QTN_IMPORT_EXPORT QtnSubItemEvent : public QEvent
{
public:
	enum Type
	{
		Activated = 3 * QEvent::User + 15,
		Deactivated = 3 * QEvent::User + 16,
		PressMouse = 3 * QEvent::User + 17,
		ReleaseMouse = 3 * QEvent::User + 18
	};

	QtnSubItemEvent(Type type, QPoint mousePos);

	inline QPoint pos() const;
	inline int x() const;
	inline int y() const;

private:
	QPoint m_mousePos;
};

QPoint QtnSubItemEvent::pos() const
{
	return m_mousePos;
}

int QtnSubItemEvent::x() const
{
	return m_mousePos.x();
}

int QtnSubItemEvent::y() const
{
	return m_mousePos.y();
}

enum QtnSubItemState
{
	QtnSubItemStateNone,
	QtnSubItemStateUnderCursor,
	QtnSubItemStatePushed
};

class QTN_IMPORT_EXPORT QtnPropertyToEdit
{
public:
	using EditorMaker = std::function<QWidget *()>;

	QtnPropertyToEdit();
	QtnPropertyToEdit(const QtnPropertyToEdit &other);

	void setup(QtnPropertyBase *property, const EditorMaker &editorMaker);

	QtnPropertyBase *property() const;
	QWidget *createEditor() const;

	bool isValid() const;

private:
	QtnPropertyBase *m_property;
	EditorMaker m_editorMaker;
};

inline QtnPropertyBase *QtnPropertyToEdit::property() const
{
	return m_property;
}

struct QTN_IMPORT_EXPORT QtnSubItem
{
	using DrawHandler =
		std::function<void(QtnDrawContext &, const QtnSubItem &)>;
	using EventHandler = std::function<bool(
		QtnEventContext &, const QtnSubItem &, QtnPropertyToEdit *)>;
	using ToolTipHandler =
		std::function<QString(QtnEventContext &, const QtnSubItem &)>;

	QtnSubItem(const QRect &rect);

	QRect rect;

	DrawHandler drawHandler;
	EventHandler eventHandler;
	ToolTipHandler tooltipHandler;

	inline QtnSubItemState state() const;
	inline void trackState();

	void setTextAsTooltip(const QString &text);
	void setPropertyNameAsTooltip(const QtnPropertyBase &property);
	void setPropertyDescriptionAsTooltip(const QtnPropertyBase &property);

private:
	bool activate(QtnPropertyView *widget, QPoint mousePos);
	bool deactivate(QtnPropertyView *widget, QPoint mousePos);

	bool grabMouse(QtnPropertyView *widget, QPoint mousePos);
	bool releaseMouse(QtnPropertyView *widget, QPoint mousePos);

	void draw(QtnDrawContext &context) const;
	bool event(QtnEventContext &context);
	bool selfEvent(
		QtnSubItemEvent::Type type, QtnPropertyView *widget, QPoint mousePos);

	bool m_trackState;
	quint8 m_activeCount;
	QtnSubItemState m_state;

	friend class QtnPropertyView;
};

QtnSubItemState QtnSubItem::state() const
{
	return m_state;
}

void QtnSubItem::trackState()
{
	m_trackState = true;
}

struct QTN_IMPORT_EXPORT QtnDrawContext
{
public:
	QStylePainter *painter;
	const QtnPropertyView *widget;

	const QRect rect;
	QMargins margins;
	const int splitPos;

	const bool isActive;
	const bool hasChildren;

	QStyle *style() const;
	void initStyleOption(QStyleOption &option) const;
	const QPalette &palette() const;
	QPalette::ColorGroup colorGroup() const;
	QColor alternateColor() const;
	QColor textColorFor(bool normalText) const;
};

struct QTN_IMPORT_EXPORT QtnEventContext
{
public:
	QEvent *event;
	QtnPropertyView *widget;

	inline int eventType() const;
	template <class EventT>
	EventT *eventAs()
	{
		return static_cast<EventT *>(event);
	}

	void updateWidget();

private:
	bool grabMouse(QtnSubItem *subItem, QPoint mousePos);
	bool releaseMouse(QtnSubItem *subItem, QPoint mousePos);

	friend struct QtnSubItem;
};

int QtnEventContext::eventType() const
{
	return static_cast<int>(event->type());
}

QTN_IMPORT_EXPORT QString qtnElidedText(const QPainter &painter,
	const QString &text, const QRect &rect, bool *elided = 0);
QTN_IMPORT_EXPORT void qtnDrawValueText(const QString &text, QPainter &painter,
	const QRect &rect, QStyle *style = nullptr);

template <typename T,
	typename std::enable_if<std::is_floating_point<T>::value>::type * = nullptr>
void fixMinMaxVariant(QVariant &minv, QVariant &maxv)
{
	bool minOk;
	bool maxOk;
	double min = minv.toDouble(&minOk);
	double max = maxv.toDouble(&maxOk);
	if (!minOk || max < min || !qIsFinite(min) ||
		min < std::numeric_limits<T>::lowest() ||
		min > std::numeric_limits<T>::max())
	{
		minv = QVariant();
	} else
	{
		minv = min;
	}
	if (!maxOk || max < min || !qIsFinite(max) ||
		max < std::numeric_limits<T>::lowest() ||
		max > std::numeric_limits<T>::max())
	{
		maxv = QVariant();
	} else
	{
		maxv = max;
	}
}

template <typename T,
	typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
void fixMinMaxVariant(QVariant &minv, QVariant &maxv)
{
	if (minv.type() == QVariant::ULongLong)
	{
		quint64 min = minv.toULongLong();
		if (min > quint64(std::numeric_limits<T>::max()))
		{
			minv = QVariant();
		} else
		{
			minv = T(min);
		}
	} else
	{
		bool minOk;
		qint64 min = minv.toLongLong(&minOk);
		if (!minOk || min < qint64(std::numeric_limits<T>::min()) ||
			(min >= 0 && quint64(min) > quint64(std::numeric_limits<T>::max())))
		{
			minv = QVariant();
		} else
		{
			minv = T(min);
		}
	}

	if (maxv.type() == QVariant::ULongLong)
	{
		quint64 max = maxv.toULongLong();
		if (max > quint64(std::numeric_limits<T>::max()))
		{
			maxv = QVariant();
		} else
		{
			maxv = T(max);
		}
	} else
	{
		bool maxOk;
		qint64 max = maxv.toLongLong(&maxOk);
		if (!maxOk || max < qint64(std::numeric_limits<T>::min()) ||
			(max >= 0 && quint64(max) > quint64(std::numeric_limits<T>::max())))
		{
			maxv = QVariant();
		} else
		{
			maxv = T(max);
		}
	}

    if (minv.isValid() && maxv.isValid() && maxv.value<T>() < minv.value<T>())
	{
		minv = QVariant();
		maxv = QVariant();
	}
}

#endif // QTN_PROPERTY_DELEGATE_AUX_H
