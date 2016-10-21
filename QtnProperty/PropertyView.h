/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#ifndef QTN_PROPERTYVIEW_H
#define QTN_PROPERTYVIEW_H

#include "CoreAPI.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "PropertySet.h"
#include "Utils/AccessibilityProxy.h"

#include <QAbstractScrollArea>
#include <QScopedPointer>

class QRubberBand;
class QHelpEvent;

class QTN_IMPORT_EXPORT QtnPropertyViewFilter
{
public:
	virtual bool accept(const QtnPropertyBase* property) const = 0;

	virtual ~QtnPropertyViewFilter() {}

protected:
	QtnPropertyViewFilter() {}
};

enum QtnPropertyViewStyleFlag
{
	QtnPropertyViewStyleNone = 0x0000,
	QtnPropertyViewStyleShowRoot = 0x0001,
	QtnPropertyViewStyleLiveSplit = 0x0002,
	QtnPropertyViewStyleDblClickActivation = 0x0004
	//QtnPropertyViewStyle = 0x0008
};
Q_DECLARE_FLAGS(QtnPropertyViewStyle, QtnPropertyViewStyleFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtnPropertyViewStyle)

class QtnConnections;

class QTN_IMPORT_EXPORT QtnPropertyView: public QAbstractScrollArea
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnPropertyView)

public:
	explicit QtnPropertyView(QWidget* parent = nullptr,
							 QtnPropertySet* propertySet = nullptr);
	~QtnPropertyView();

	inline const QtnPropertySet* propertySet() const;
	inline QtnPropertySet* propertySet();
	void setPropertySet(QtnPropertySet* newPropertySet);

	QtnPropertyBase *getPropertyParent(const QtnPropertyBase *property) const;
	inline QtnPropertyBase* activeProperty();
	inline const QtnPropertyBase* activeProperty() const;
	bool setActiveProperty(QtnPropertyBase*newActiveProperty);

	bool ensureVisible(const QtnPropertyBase* property);

	inline int itemHeight() const;

	inline quint32 itemHeightSpacing() const;
	bool setItemHeightSpacing(quint32 itemHeightSpacing);

	inline QtnPropertyViewStyle propertyViewStyle() const;
	void setPropertyViewStyle(QtnPropertyViewStyle style);
	void addPropertyViewStyle(QtnPropertyViewStyle style);
	void removePropertyViewStyle(QtnPropertyViewStyle style);

	QtnPropertyBase *getPropertyAt(const QPoint &position, QRect *out_rect = nullptr);

	void connectPropertyToEdit(QtnPropertyBase *property, QtnConnections &outConnections);

public slots:
	QtnAccessibilityProxy* accessibilityProxy();

Q_SIGNALS:
	// emits when active property has changed
	void activePropertyChanged(QtnPropertyBase* activeProperty);
	void mouseReleased(QMouseEvent *e);
	void beforePropertyEdited(QtnProperty *property, QtnPropertyValuePtr newValue, int typeId);
	void propertyEdited(QtnProperty *property);

private slots:
	void onActivePropertyDestroyed();
	void onEditedPropertyWillChange(QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue, int typeId);
	void onEditedPropertyDidChange(QtnPropertyChangeReason reason);

protected:
	void paintEvent(QPaintEvent* e) override;
	void resizeEvent(QResizeEvent* e) override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseDoubleClickEvent(QMouseEvent* e) override;
	bool viewportEvent(QEvent* e) override;
	void scrollContentsBy(int dx, int dy) override;
	void keyPressEvent(QKeyEvent* e) override;
	void tooltipEvent(QHelpEvent* e);

private:

	struct Item
	{
		QtnPropertyBase* property;
		QScopedPointer<QtnPropertyDelegate> delegate;
		int level;

		Item *parent;
		QList<QSharedPointer<Item> > children;

		Item();

		inline bool collapsed() const;
	};

	struct Action
	{
		QRect rect;
		std::function<bool(QEvent *, QRect)> action;
	};

	struct VisibleItem
	{
		Item *item;
		int level;
		bool hasChildren;

		mutable QList<Action> actions;
		mutable bool actionsValid;
		mutable bool needTooltip;

		VisibleItem();
	};

private:
	void updateItemsTree();
	static Item* createItemsTree(QtnPropertyBase* rootProperty, const QtnPropertyDelegateFactory& factory);

	void setActivePropertyInternal(QtnPropertyBase *property);
	bool startPropertyEdit(QtnPropertyDelegate *delegate, QEvent *e, const QRect &rect);

	void invalidateVisibleItems();
	void validateVisibleItems() const;
	void fillVisibleItems(Item* item, int level) const;
	bool acceptItem(Item& item) const;

	void drawBranchNode(QStylePainter& painter, QRect& rect, const VisibleItem& vItem);
	void drawPropertySetItem(QStylePainter &painter, const QRect &rect, const VisibleItem& vItem);
	void drawPropertyItem(QStylePainter& painter, const QRect& rect, const VisibleItem& vItem);

	void changeActivePropertyByIndex(int index);
	int visibleItemIndexByPoint(QPoint pos) const;
	int visibleItemIndexByProperty(const QtnPropertyBase* property) const;
	QRect visibleItemRect(int index) const;

	bool processItemActionByMouse(int index, QMouseEvent* e);

	void updateVScrollbar() const;
	void updateStyleStuff();

	bool ensureVisibleItemByIndex(int index);
	void invalidateVisibleItemsActions();

	int splitPosition() const;
	void updateSplitRatio(float splitRatio);

	void connectActiveProperty();
	void disconnectActiveProperty();

	void onPropertySetDidChange(QtnPropertyChangeReason reason);

private:
	Item *findItem(Item *currentItem, const QtnPropertyBase *property) const;

	QtnPropertySet* m_propertySet;
	QtnPropertyBase* m_activeProperty;

	QtnPropertyDelegateFactory m_delegateFactory;

	QScopedPointer<Item> m_itemsTree;

	mutable QList<VisibleItem> m_visibleItems;
	mutable bool m_visibleItemsValid;

	QtnPropertyViewStyle m_style;
	int m_itemHeight;
	quint32 m_itemHeightSpacing;
	int m_leadMargin;
	QColor m_linesColor;
	QColor m_propertySetBackdroundColor;

	float m_splitRatio;
	QRubberBand* m_rubberBand;

	friend class QtnAccessibilityProxy;
	QtnAccessibilityProxy* m_accessibilityProxy;
};

const QtnPropertySet *QtnPropertyView::propertySet() const
{
	return m_propertySet;
}

QtnPropertySet *QtnPropertyView::propertySet()
{
	return m_propertySet;
}

QtnPropertyBase *QtnPropertyView::activeProperty()
{
	return m_activeProperty;
}

const QtnPropertyBase *QtnPropertyView::activeProperty() const
{
	return m_activeProperty;
}

int QtnPropertyView::itemHeight() const
{
	return m_itemHeight;
}

quint32 QtnPropertyView::itemHeightSpacing() const
{
	return m_itemHeightSpacing;
}

QtnPropertyViewStyle QtnPropertyView::propertyViewStyle() const
{
	return m_style;
}

bool QtnPropertyView::Item::collapsed() const
{
	return property->isCollapsed();
}

#endif // QTN_PROPERTYVIEW_H
