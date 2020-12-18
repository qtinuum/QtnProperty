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

#ifndef QTN_PROPERTYVIEW_H
#define QTN_PROPERTYVIEW_H

#include "FunctionalHelpers.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Utils/AccessibilityProxy.h"

#include <QAbstractScrollArea>

#include <memory>

class QRubberBand;
class QHelpEvent;
class QtnConnections;

enum QtnPropertyViewStyleFlag
{
	QtnPropertyViewStyleNone = 0x0000,
	QtnPropertyViewStyleShowRoot = 0x0001,
	QtnPropertyViewStyleLiveSplit = 0x0002,
	QtnPropertyViewStyleDblClickActivation = 0x0004
};

Q_DECLARE_FLAGS(QtnPropertyViewStyle, QtnPropertyViewStyleFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtnPropertyViewStyle)

class QTN_IMPORT_EXPORT QtnPropertyView : public QAbstractScrollArea
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnPropertyView)

public:
	explicit QtnPropertyView(
		QWidget *parent = nullptr, QtnPropertySet *propertySet = nullptr);
	virtual ~QtnPropertyView() override;

	inline QtnPropertyDelegateFactory *delegateFactory();

	inline const QtnPropertySet *propertySet() const;
	inline QtnPropertySet *propertySet();
	void setPropertySet(QtnPropertySet *newPropertySet);

	QtnPropertyBase *getPropertyParent(const QtnPropertyBase *property) const;
	inline QtnPropertyBase *activeProperty();
	inline const QtnPropertyBase *activeProperty() const;
	bool setActiveProperty(
		QtnPropertyBase *newActiveProperty, bool ensureVisible = false);
	bool setActiveProperty(int index, bool ensureVisible = false);

	bool ensureVisible(const QtnPropertyBase *property);

	inline int itemHeight() const;

	inline quint32 itemHeightSpacing() const;
	bool setItemHeightSpacing(quint32 itemHeightSpacing);

	inline QtnPropertyViewStyle propertyViewStyle() const;
	void setPropertyViewStyle(QtnPropertyViewStyle style);
	void addPropertyViewStyle(QtnPropertyViewStyle style);
	void removePropertyViewStyle(QtnPropertyViewStyle style);

	QtnPropertyBase *getPropertyAt(
		const QPoint &position, QRect *out_rect = nullptr);

	void connectPropertyToEdit(
		QtnPropertyBase *property, QtnConnections &outConnections);
	int valueLeftMargin() const;

	bool isMouseCaptured() const;

public slots:
	QtnAccessibilityProxy *accessibilityProxy();

signals:
	void propertiesChanged(QtnPropertyChangeReason reason);
	// emits when active property has changed
	void activePropertyChanged(QtnPropertyBase *activeProperty);
	void mouseReleased(QMouseEvent *e);
	void beforePropertyEdited(
		QtnPropertyBase *property, QtnPropertyValuePtr newValue, int typeId);
	void propertyEdited(QtnPropertyBase *property);

	void beforePropertyLockToggled(QtnPropertyBase *property);
	void propertyLockToggled(QtnPropertyBase *property);

private:
	void onActivePropertyDestroyed();
	void onEditedPropertyWillChange(QtnPropertyChangeReason reason,
		QtnPropertyValuePtr newValue, int typeId);
	void onEditedPropertyDidChange(QtnPropertyChangeReason reason);

protected:
	void paintEvent(QPaintEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseDoubleClickEvent(QMouseEvent *e) override;
	bool viewportEvent(QEvent *e) override;
	void scrollContentsBy(int dx, int dy) override;
	void keyPressEvent(QKeyEvent *e) override;
	void wheelEvent(QWheelEvent *e) override;
	void tooltipEvent(QHelpEvent *e);

private:
	struct Item;
	struct VisibleItem;

private:
	void updateItemsTree();
	Item *createItemsTree(QtnPropertyBase *rootProperty);

	void setActivePropertyInternal(QtnPropertyBase *property);

	void invalidateVisibleItems();
	void validateVisibleItems() const;
	void fillVisibleItems(Item *item, int level) const;
	bool acceptItem(const Item &item) const;

	void drawItem(QStylePainter &painter, const QRect &rect,
		const VisibleItem &vItem) const;

	void changeActivePropertyByIndex(int index);
	QtnPropertyBase *visiblePropertyAtPoint(const QPoint &pos) const;
	int visibleItemIndexByPoint(const QPoint &pos) const;
	int visibleItemIndexByProperty(const QtnPropertyBase *property) const;
	QRect visibleItemRect(int index) const;
	QRect propertyActionRect(QtnPropertyBase *property, int actionIndex);

	bool handleMouseEvent(int index, QEvent *e, QPoint mousePos);
	bool handleEvent(
		QtnEventContext &context, VisibleItem &vItem, QPoint mousePos);
	bool grabMouseForSubItem(QtnSubItem *subItem, QPoint mousePos);
	bool releaseMouseForSubItem(QtnSubItem *subItem, QPoint mousePos);

	void updateVScrollbar() const;
	void updateStyleStuff();

	bool ensureVisibleItemByIndex(int index);
	void invalidateSubItems();
	void deactivateSubItems();

	int splitPosition() const;
	void updateSplitRatio(float splitRatio);

	void connectActiveProperty();
	void disconnectActiveProperty();

	void onPropertyDidChange(QtnPropertyChangeReason reason, Item *item);
	void onPropertySetDestroyed();
	void updateWithReason(QtnPropertyChangeReason reason);

	Item *findItem(Item *currentItem, const QtnPropertyBase *property) const;
	void setupItemDelegate(Item *item);

private:
	QtnPropertySet *m_propertySet;
	QtnPropertyBase *m_activeProperty;

	QtnPropertyDelegateFactory m_delegateFactory;

	std::unique_ptr<Item> m_itemsTree;

	mutable QList<VisibleItem> m_visibleItems;
	mutable bool m_visibleItemsValid;

	QList<QtnSubItem *> m_activeSubItems;
	QtnSubItem *m_grabMouseSubItem;

	QtnPropertyViewStyle m_style;
	int m_itemHeight;
	quint32 m_itemHeightSpacing;
	int m_valueLeftMargin;
	QColor m_linesColor;
	QColor m_propertySetBackdroundColor;

	float m_splitRatio;
	std::unique_ptr<QRubberBand> m_rubberBand;
	QtnPropertyChangeReason m_lastChangeReason;
	unsigned m_stopInvalidate;
	bool m_mouseAtSplitter;
	bool m_mouseCaptured;

	friend class QtnAccessibilityProxy;
	QtnAccessibilityProxy *m_accessibilityProxy;
	friend struct QtnEventContext;
};

QtnPropertyDelegateFactory *QtnPropertyView::delegateFactory()
{
	return &m_delegateFactory;
}

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

#endif // QTN_PROPERTYVIEW_H
