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

#include "PropertyView.h"

#include "Utils/InplaceEditing.h"
#include "Utils/QtnConnections.h"
#include "Delegates/Utils/PropertyDelegateMisc.h"

#include <QApplication>
#include <QScrollBar>
#include <QHelpEvent>
#include <QToolTip>

struct QtnPropertyView::Item
{
	QtnPropertyBase *property;
	std::unique_ptr<QtnPropertyDelegate> delegate;
	int level;

	Item *parent;
	std::vector<std::unique_ptr<Item>> children;
	QtnConnections connections;

	Item();

	inline bool collapsed() const;
};

struct QtnPropertyView::VisibleItem
{
	Item *item;
	int level;
	bool hasChildren;

	mutable QList<QtnSubItem> subItems;
	mutable bool subItemsValid;

	VisibleItem();
};

class QtnPainterState
{
public:
	QtnPainterState(QPainter &p);
	~QtnPainterState();

private:
	QPainter &m_p;
};

extern void set_smaller_text_osx(QWidget *w);

QtnPropertyView::QtnPropertyView(QWidget *parent, QtnPropertySet *propertySet)
	: QAbstractScrollArea(parent)
	, m_propertySet(propertySet)
	, m_activeProperty(nullptr)
	, m_delegateFactory(&QtnPropertyDelegateFactory::staticInstance())
	, m_visibleItemsValid(false)
	, m_grabMouseSubItem(nullptr)
	, m_style(QtnPropertyViewStyleLiveSplit)
	, m_itemHeight(0)
	, m_itemHeightSpacing(6)
	, m_valueLeftMargin(0)
	, m_splitRatio(0.5f)
	, m_rubberBand(nullptr)
	, m_lastChangeReason(0)
	, m_stopInvalidate(false)
	, m_mouseAtSplitter(false)
	, m_accessibilityProxy(nullptr)
{
	set_smaller_text_osx(this);

	setFocusPolicy(Qt::StrongFocus);
	viewport()->setMouseTracking(true);

	updateStyleStuff();

	updateItemsTree();
}

QtnPropertyView::~QtnPropertyView()
{
	// destruct everything
}

QtnAccessibilityProxy *QtnPropertyView::accessibilityProxy()
{
	if (!m_accessibilityProxy)
		m_accessibilityProxy = new QtnAccessibilityProxy(this);

	return m_accessibilityProxy;
}

void QtnPropertyView::onActivePropertyDestroyed()
{
	m_activeProperty = nullptr;
	emit activePropertyChanged(m_activeProperty);
	viewport()->update();
}

void QtnPropertyView::onEditedPropertyWillChange(
	QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue, int typeId)
{
	if (reason & QtnPropertyChangeReasonEditValue)
	{
		Q_ASSERT(nullptr != qobject_cast<QtnPropertyBase *>(sender()));
		auto property = static_cast<QtnPropertyBase *>(sender());

		emit beforePropertyEdited(property, newValue, typeId);
	}
}

void QtnPropertyView::onEditedPropertyDidChange(QtnPropertyChangeReason reason)
{
	if (0 != (reason & QtnPropertyChangeReasonEditValue))
	{
		Q_ASSERT(nullptr != qobject_cast<QtnPropertyBase *>(sender()));
		auto property = static_cast<QtnPropertyBase *>(sender());

		emit propertyEdited(property);
	}
}

void QtnPropertyView::setPropertySet(QtnPropertySet *newPropertySet)
{
	if (newPropertySet == m_propertySet)
		return;
	if (m_propertySet)
	{
		QObject::disconnect(m_propertySet, &QtnPropertyBase::destroyed, this,
			&QtnPropertyView::onPropertySetDestroyed);
	}

	m_propertySet = newPropertySet;

	if (m_propertySet)
	{
		QObject::connect(m_propertySet, &QtnPropertyBase::destroyed, this,
			&QtnPropertyView::onPropertySetDestroyed);
	}

	updateItemsTree();
}

QtnPropertyBase *QtnPropertyView::getPropertyParent(
	const QtnPropertyBase *property) const
{
	auto item = findItem(m_itemsTree.get(), property);

	if (nullptr != item && nullptr != item->parent)
		return item->parent->property;

	return nullptr;
}

bool QtnPropertyView::setActiveProperty(
	QtnPropertyBase *newActiveProperty, bool ensureVisible)
{
	if (m_activeProperty == newActiveProperty)
		return false;

	qtnStopInplaceEdit();

	if (ensureVisible)
		this->ensureVisible(newActiveProperty);

	if (!newActiveProperty)
	{
		setActivePropertyInternal(nullptr);
		return true;
	}

	int index = visibleItemIndexByProperty(newActiveProperty);

	if (index < 0)
		return false;

	setActivePropertyInternal(newActiveProperty);
	return true;
}

bool QtnPropertyView::setActiveProperty(int index, bool ensureVisible)
{
	if (index < 0)
		index = 0;

	if (nullptr == m_propertySet)
		return false;

	auto &cp = m_propertySet->childProperties();
	if (cp.isEmpty())
		return false;

	if (index >= cp.size())
		index = cp.size() - 1;

	return setActiveProperty(cp.at(index), ensureVisible);
}

bool QtnPropertyView::ensureVisible(const QtnPropertyBase *property)
{
	if (!property)
		return false;

	int index = visibleItemIndexByProperty(property);
	return ensureVisibleItemByIndex(index);
}

bool QtnPropertyView::setItemHeightSpacing(quint32 itemHeightSpacing)
{
	m_itemHeightSpacing = itemHeightSpacing;
	updateStyleStuff();
	return true;
}

void QtnPropertyView::setPropertyViewStyle(QtnPropertyViewStyle style)
{
	m_style = style;
}

void QtnPropertyView::addPropertyViewStyle(QtnPropertyViewStyle style)
{
	setPropertyViewStyle(propertyViewStyle() | style);
}

void QtnPropertyView::removePropertyViewStyle(QtnPropertyViewStyle style)
{
	setPropertyViewStyle(propertyViewStyle() & ~style);
}

QtnPropertyBase *QtnPropertyView::getPropertyAt(
	const QPoint &position, QRect *out_rect)
{
	int index = visibleItemIndexByPoint(position);

	if (index >= 0)
	{
		if (nullptr != out_rect)
		{
			*out_rect = visibleItemRect(index);
		}

		return m_visibleItems[index].item->property;
	}

	return nullptr;
}

int QtnPropertyView::valueLeftMargin() const
{
	return m_valueLeftMargin;
}

void QtnPropertyView::connectPropertyToEdit(
	QtnPropertyBase *property, QtnConnections &outConnections)
{
	Q_ASSERT(nullptr != property);

	outConnections.push_back(
		QObject::connect(property, &QtnPropertyBase::propertyWillChange, this,
			&QtnPropertyView::onEditedPropertyWillChange));

	outConnections.push_back(
		QObject::connect(property, &QtnPropertyBase::propertyDidChange, this,
			&QtnPropertyView::onEditedPropertyDidChange));
}

void QtnPropertyView::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);

	validateVisibleItems();

	if (m_visibleItems.isEmpty())
	{
		return;
	}

	int firstVisibleItemIndex =
		qMin(verticalScrollBar()->value() / m_itemHeight,
			(m_visibleItems.size() - 1));
	int lastVisibleItemIndex = qMin(
		((verticalScrollBar()->value() + viewport()->height()) / m_itemHeight) +
			1,
		(m_visibleItems.size() - 1));

	QRect itemRect = viewport()->rect();
	itemRect.setTop(
		firstVisibleItemIndex * m_itemHeight - verticalScrollBar()->value());
	itemRect.setBottom(itemRect.top() + m_itemHeight);

	QStylePainter painter(viewport());

	for (int i = firstVisibleItemIndex; i <= lastVisibleItemIndex; ++i)
	{
		const VisibleItem &vItem = m_visibleItems[i];
		drawItem(painter, itemRect, vItem);
		itemRect.translate(0, m_itemHeight);
	}
}

void QtnPropertyView::drawItem(
	QStylePainter &painter, const QRect &rect, const VisibleItem &vItem) const
{
	if (!vItem.item->delegate.get())
	{
		return;
	}

	QMargins margins(m_valueLeftMargin + rect.height() * vItem.level, 0, 0, 0);
	bool isActive = (m_activeProperty == vItem.item->property);

	QtnDrawContext drawContext{ &painter, this, rect, margins, splitPosition(),
		isActive, vItem.hasChildren };

	// create sub-items if not initialized
	if (!vItem.subItemsValid)
	{
		Q_ASSERT(vItem.subItems.isEmpty());
		vItem.item->delegate->createSubItems(drawContext, vItem.subItems);
		vItem.subItemsValid = true;
	}

	// draw sub-items
	for (const auto &subItem : vItem.subItems)
	{
		subItem.draw(drawContext);
	}
}

void QtnPropertyView::changeActivePropertyByIndex(int index)
{
	QtnPropertyBase *newActiveProperty =
		(index < 0) ? nullptr : m_visibleItems[index].item->property;
	setActiveProperty(newActiveProperty);
	ensureVisibleItemByIndex(index);
}

QtnPropertyBase *QtnPropertyView::visiblePropertyAtPoint(
	const QPoint &pos) const
{
	int index = visibleItemIndexByPoint(pos);

	if (index < 0)
		return nullptr;

	return m_visibleItems[index].item->property;
}

int QtnPropertyView::visibleItemIndexByPoint(const QPoint &pos) const
{
	int index = (verticalScrollBar()->value() + pos.y()) / m_itemHeight;
	if (index >= m_visibleItems.size())
		return -1;

	return index;
}

int QtnPropertyView::visibleItemIndexByProperty(
	const QtnPropertyBase *property) const
{
	validateVisibleItems();

	for (int i = 0, n = m_visibleItems.size(); i < n; ++i)
		if (m_visibleItems[i].item->property == property)
			return i;

	return -1;
}

QRect QtnPropertyView::visibleItemRect(int index) const
{
	Q_ASSERT(index >= 0 && index < m_visibleItems.size());

	QRect rect = viewport()->rect();
	rect.setTop(index * m_itemHeight - verticalScrollBar()->value());
	rect.setHeight(m_itemHeight);

	return rect;
}

QRect QtnPropertyView::propertyActionRect(
	QtnPropertyBase *property, int actionIndex)
{
	if (!property)
		return QRect();

	int index = visibleItemIndexByProperty(property);

	if (index < 0)
		return QRect();

	const auto &item = m_visibleItems[index];

	if (!item.subItemsValid)
		return QRect();

	if (actionIndex < 0 || actionIndex >= item.subItems.size())
		return QRect();

	return item.subItems[actionIndex].rect;
}

bool QtnPropertyView::handleMouseEvent(int index, QEvent *e, QPoint mousePos)
{
	if (index < 0)
	{
		deactivateSubItems();
		return false;
	}

	QtnEventContext context{ e, this };
	return handleEvent(context, m_visibleItems[index], mousePos);
}

void QtnPropertyView::resizeEvent(QResizeEvent *e)
{
	Q_UNUSED(e);

	qtnStopInplaceEdit();
	invalidateSubItems();
	updateVScrollbar();
}

static const int TOLERANCE = 3;

void QtnPropertyView::mousePressEvent(QMouseEvent *e)
{
	if (qAbs(e->x() - splitPosition()) < TOLERANCE)
	{
		Q_ASSERT(!m_rubberBand);
		m_rubberBand = new QRubberBand(QRubberBand::Line, this);

		QRect rect = viewport()->rect();
		rect.setLeft(e->x());
		rect.setRight(e->x());
		m_rubberBand->setGeometry(rect);
		m_rubberBand->show();
	} else
	{
		int index = visibleItemIndexByPoint(e->pos());
		if (index >= 0)
		{
			changeActivePropertyByIndex(index);
			handleMouseEvent(index, e, e->pos());
		}

		QAbstractScrollArea::mousePressEvent(e);
	}
}

void QtnPropertyView::mouseReleaseEvent(QMouseEvent *e)
{
	if (m_rubberBand)
	{
		delete m_rubberBand;
		m_rubberBand = nullptr;

		// update split ratio
		QRect rect = viewport()->rect();
		updateSplitRatio((float) (e->x() - rect.left()) / (float) rect.width());
	} else
	{
		handleMouseEvent(visibleItemIndexByPoint(e->pos()), e, e->pos());
		emit mouseReleased(e);
	}
}

void QtnPropertyView::mouseMoveEvent(QMouseEvent *e)
{
	if (m_rubberBand)
	{
		QRect rect = viewport()->rect();
		rect.setLeft(e->x());
		rect.setRight(e->x());
		m_rubberBand->setGeometry(rect);

		if (m_style & QtnPropertyViewStyleLiveSplit)
		{
			// update split ratio
			QRect rect = viewport()->rect();
			updateSplitRatio(
				(float) (e->x() - rect.left()) / (float) rect.width());
		}
	} else if (qAbs(e->x() - splitPosition()) < TOLERANCE)
	{
		if (!m_mouseAtSplitter)
		{
			m_mouseAtSplitter = true;
			m_oldCursor = cursor();
			setCursor(Qt::SplitHCursor);
		}
	} else
	{
		if (m_mouseAtSplitter)
		{
			m_mouseAtSplitter = false;
			setCursor(m_oldCursor);
		}

		int index = visibleItemIndexByPoint(e->pos());
		if (e->buttons() & Qt::LeftButton)
			changeActivePropertyByIndex(index);
		//else
		handleMouseEvent(index, e, e->pos());
	}
}

void QtnPropertyView::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (!m_rubberBand)
	{
		handleMouseEvent(visibleItemIndexByPoint(e->pos()), e, e->pos());
		QAbstractScrollArea::mouseDoubleClickEvent(e);
	}
}

bool QtnPropertyView::viewportEvent(QEvent *e)
{
	switch (e->type())
	{
		case QEvent::StyleChange:
			updateStyleStuff();
			break;

		case QEvent::ToolTip:
		{
			QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
			tooltipEvent(helpEvent);
			break;
		}

		case QEvent::Leave:
			deactivateSubItems();
			break;

		default:; // do nothing
	}

	return QAbstractScrollArea::viewportEvent(e);
}

void QtnPropertyView::scrollContentsBy(int dx, int dy)
{
	if (dx != 0 || dy != 0)
	{
		qtnStopInplaceEdit();
		invalidateSubItems();
	}

	QAbstractScrollArea::scrollContentsBy(dx, dy);
}

void QtnPropertyView::keyPressEvent(QKeyEvent *e)
{
	validateVisibleItems();

	if (m_visibleItems.empty())
	{
		QAbstractScrollArea::keyPressEvent(e);
		return;
	}

	QWidget *inplaceEditor = qtnGetInplaceEdit();
	if (inplaceEditor)
	{
		int key = e->key();
		if (key == Qt::Key_Escape || key == Qt::Key_Return ||
			key == Qt::Key_Enter)
		{
			qtnStopInplaceEdit();
			// eat event
			e->accept();
		}
		return;
	}

	switch (e->key())
	{
		case Qt::Key_Home:
		{
			// go to first item
			changeActivePropertyByIndex(0);
			break;
		}

		case Qt::Key_End:
		{
			// go to last item
			changeActivePropertyByIndex(m_visibleItems.size() - 1);
			break;
		}

		case Qt::Key_Up:
		{
			// go to previous item
			int index = visibleItemIndexByProperty(activeProperty());
			if (index < 0)
				changeActivePropertyByIndex(0);
			else
				changeActivePropertyByIndex(qMax(0, index - 1));
			break;
		}

		case Qt::Key_Down:
		{
			// go to next item
			int index = visibleItemIndexByProperty(activeProperty());
			if (index < 0)
				changeActivePropertyByIndex(0);
			else
				changeActivePropertyByIndex(
					qMin(m_visibleItems.size() - 1, index + 1));
			break;
		}

		case Qt::Key_PageUp:
		{
			// go to previous page
			int index = visibleItemIndexByProperty(activeProperty());
			if (index < 0)
				changeActivePropertyByIndex(0);
			else
			{
				int itemsPerPage =
					qMax(viewport()->rect().height() / m_itemHeight, 1);
				changeActivePropertyByIndex(qMax(0, index - itemsPerPage));
			}
			break;
		}

		case Qt::Key_PageDown:
		{
			// go to next page
			int index = visibleItemIndexByProperty(activeProperty());
			if (index < 0)
				changeActivePropertyByIndex(0);
			else
			{
				int itemsPerPage =
					qMax(viewport()->rect().height() / m_itemHeight, 1);
				changeActivePropertyByIndex(
					qMin(m_visibleItems.size() - 1, index + itemsPerPage));
			}
			break;
		}

		case Qt::Key_Left:
		{
			// go to parent item or collapse
			int index = visibleItemIndexByProperty(activeProperty());
			if (index < 0)
				changeActivePropertyByIndex(0);
			else
			{
				const VisibleItem &vItem = m_visibleItems[index];
				if (vItem.hasChildren && !vItem.item->collapsed())
				{
					// collapse opened property
					vItem.item->property->addState(QtnPropertyStateCollapsed);
				} else if (vItem.item->parent)
				{
					// activate parent property
					setActiveProperty(vItem.item->parent->property, true);
				}
			}
			break;
		}

		case Qt::Key_Right:
		{
			// go to child item or expand
			int index = visibleItemIndexByProperty(activeProperty());
			if (index < 0)
				changeActivePropertyByIndex(0);
			else
			{
				const VisibleItem &vItem = m_visibleItems[index];
				if (vItem.hasChildren && vItem.item->collapsed())
				{
					// expand closed property
					vItem.item->property->removeState(
						QtnPropertyStateCollapsed);
				} else if (vItem.hasChildren)
				{
					// activate child property
					setActiveProperty(
						vItem.item->children.front()->property, true);
				}
			}
			break;
		}

		default:
		{
			int index = visibleItemIndexByProperty(activeProperty());

			if (index >= 0)
			{
				QtnEventContext context{ e, this };
				if (handleEvent(context, m_visibleItems[index], QPoint()))
				{
					// eat event
					e->accept();
					return;
				}
			}

			// process by default
			QAbstractScrollArea::keyPressEvent(e);
		}
	}
}

void QtnPropertyView::wheelEvent(QWheelEvent *e)
{
	bool processed =
		handleMouseEvent(visibleItemIndexByPoint(e->pos()), e, e->pos());
	if (processed)
		return;

	QAbstractScrollArea::wheelEvent(e);
}

void QtnPropertyView::tooltipEvent(QHelpEvent *e)
{
	if (!handleMouseEvent(visibleItemIndexByPoint(e->pos()), e, e->pos()))
	{
		QToolTip::hideText();
	}
}

bool QtnPropertyView::handleEvent(
	QtnEventContext &context, VisibleItem &vItem, QPoint mousePos)
{
	if (!vItem.subItemsValid)
		return false;

	Q_ASSERT(!m_stopInvalidate);
	m_stopInvalidate = true;
	m_lastChangeReason = QtnPropertyChangeReason(0);
	bool result;
	// process event
	if (m_grabMouseSubItem)
		result = m_grabMouseSubItem->event(context);
	else
	{
		result = false;
		// update list of sub items under cursor
		QList<QtnSubItem *> activeSubItems;

		// make list of new active sub items
		for (auto &subItem : vItem.subItems)
		{
			if (mousePos.isNull() || subItem.rect.contains(mousePos))
			{
				subItem.activate(this, mousePos);
				activeSubItems.append(&subItem);
			}
		}

		// deactivate old sub items
		for (auto activeSubItem : m_activeSubItems)
		{
			activeSubItem->deactivate(this, mousePos);
		}

		// adopt new active sub items
		m_activeSubItems.swap(activeSubItems);

		// process event
		for (auto activeSubItem : m_activeSubItems)
		{
			if (activeSubItem->event(context))
			{
				result = true;
				break;
			}
		}
	}
	m_stopInvalidate = false;
	updateWithReason(m_lastChangeReason);

	return result;
}

QtnPropertyView::Item::Item()
	: property(nullptr)
	, level(0)
	, parent(nullptr)
{
}

bool QtnPropertyView::Item::collapsed() const
{
	return property->isCollapsed();
}

bool QtnPropertyView::grabMouseForSubItem(QtnSubItem *subItem, QPoint mousePos)
{
	Q_ASSERT(!m_grabMouseSubItem);
	if (m_grabMouseSubItem)
		return false;

	viewport()->grabMouse();
	m_grabMouseSubItem = subItem;
	m_grabMouseSubItem->grabMouse(this, mousePos);

	return true;
}

bool QtnPropertyView::releaseMouseForSubItem(
	QtnSubItem *subItem, QPoint mousePos)
{
	Q_UNUSED(subItem);
	Q_ASSERT(m_grabMouseSubItem == subItem);
	if (!m_grabMouseSubItem)
		return false;

	m_grabMouseSubItem->releaseMouse(this, mousePos);
	m_grabMouseSubItem = nullptr;
	viewport()->releaseMouse();

	return true;
}

void QtnPropertyView::updateItemsTree()
{
	m_itemsTree.reset(createItemsTree(m_propertySet));
	invalidateVisibleItems();
}

QtnPropertyView::Item *QtnPropertyView::createItemsTree(
	QtnPropertyBase *rootProperty)
{
	if (!rootProperty)
		return nullptr;

	auto item = new Item;
	item->property = rootProperty;
	auto &connections = item->connections;

	connections.push_back(
		QObject::connect(rootProperty, &QtnPropertyBase::propertyDidChange,
			this, &QtnPropertyView::onPropertyDidChange));

	auto delegate = m_delegateFactory.createDelegate(*rootProperty);
	item->delegate.reset(delegate);

	if (delegate)
	{
		// apply attributes
		auto delegateInfo = rootProperty->delegateInfo();
		if (delegateInfo)
		{
			delegate->applyAttributes(*delegateInfo);
		}

		// process delegate subproperties
		for (int i = 0, n = delegate->subPropertyCount(); i < n; ++i)
		{
			auto child = delegate->subProperty(i);
			Q_ASSERT(child);

			auto childItem = createItemsTree(child);
			childItem->parent = item;
			item->children.emplace_back(childItem);
		}
	}

	return item;
}

void QtnPropertyView::setActivePropertyInternal(QtnPropertyBase *property)
{
	disconnectActiveProperty();

	m_activeProperty = property;
	emit activePropertyChanged(m_activeProperty);
	viewport()->update();

	connectActiveProperty();
}

void QtnPropertyView::invalidateVisibleItems()
{
	deactivateSubItems();
	m_visibleItemsValid = false;
	m_visibleItems.clear();
	viewport()->update();
}

void QtnPropertyView::validateVisibleItems() const
{
	if (m_visibleItemsValid)
		return;

	fillVisibleItems(
		m_itemsTree.get(), (m_style & QtnPropertyViewStyleShowRoot) ? 0 : -1);

	updateVScrollbar();

	m_visibleItemsValid = true;
}

void QtnPropertyView::fillVisibleItems(Item *item, int level) const
{
	if (!item)
		return;

	// process children only for negative levels
	if (level < 0)
	{
		// process children
		for (auto &child : item->children)
		{
			fillVisibleItems(child.get(), level + 1);
		}

		return;
	}

	// skip not accepted items
	if (!acceptItem(*item))
		return;

	VisibleItem vItem;
	vItem.item = item;
	vItem.level = level;

	if (item->collapsed())
	{
		// check if item has any child
		for (auto &child : item->children)
		{
			if (acceptItem(*child.get()))
			{
				vItem.hasChildren = true;
				break;
			}
		}

		// add item and quit
		m_visibleItems.append(vItem);
		return;
	}

	// add item
	m_visibleItems.append(vItem);

	// save just added item index
	int index = m_visibleItems.size() - 1;

	// process children
	for (auto &child : item->children)
	{
		fillVisibleItems(child.get(), level + 1);
	}

	// if we add something -> current item has children
	if (index < (m_visibleItems.size() - 1))
		m_visibleItems[index].hasChildren = true;
}

bool QtnPropertyView::acceptItem(const Item &item) const
{
	return item.property->isVisible();
}

void QtnPropertyView::updateVScrollbar() const
{
	int viewportHeight = viewport()->height();
	int virtualHeight = m_itemHeight * m_visibleItems.size();

	verticalScrollBar()->setSingleStep(m_itemHeight);
	verticalScrollBar()->setPageStep(viewportHeight);
	verticalScrollBar()->setRange(
		0, qMax(0, virtualHeight - viewportHeight + 2));
}

void QtnPropertyView::updateStyleStuff()
{
	QFontMetrics fm(font());
	m_itemHeight = fm.height() + m_itemHeightSpacing;

	m_propertySetBackdroundColor = m_linesColor =
		palette().color(QPalette::Button);

	m_valueLeftMargin = style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing);
}

bool QtnPropertyView::ensureVisibleItemByIndex(int index)
{
	if (index < 0)
		return false;

	int vItemTop = index * m_itemHeight;
	int vItemBottom = vItemTop + m_itemHeight;

	QRect rect = viewport()->rect();
	int scrollPos = verticalScrollBar()->value();

	if (vItemTop < scrollPos)
		scrollPos = vItemTop;
	else if (vItemBottom > scrollPos + rect.height())
		scrollPos = vItemBottom - rect.height();
	else
		return false;

	verticalScrollBar()->setValue(scrollPos);
	return true;
}

void QtnPropertyView::invalidateSubItems()
{
	deactivateSubItems();

	for (auto &item : m_visibleItems)
	{
		item.subItemsValid = false;
		item.subItems.clear();
	}
}

void QtnPropertyView::deactivateSubItems()
{
	if (m_grabMouseSubItem)
	{
		viewport()->releaseMouse();
		m_grabMouseSubItem = nullptr;
	}

	for (auto subItem : m_activeSubItems)
		subItem->deactivate(this, QPoint());

	m_activeSubItems.clear();

	QToolTip::hideText();
}

int QtnPropertyView::splitPosition() const
{
	return (int) ((float) viewport()->rect().width() * m_splitRatio);
}

void QtnPropertyView::updateSplitRatio(float splitRatio)
{
	m_splitRatio = qBound(0.f, splitRatio, 1.f);
	// firce to regenerate sub-items
	invalidateSubItems();
	// repaint
	viewport()->update();
}

void QtnPropertyView::connectActiveProperty()
{
	if (nullptr != m_activeProperty)
	{
		QObject::connect(m_activeProperty, &QObject::destroyed, this,
			&QtnPropertyView::onActivePropertyDestroyed);
	}
}

void QtnPropertyView::disconnectActiveProperty()
{
	if (nullptr != m_activeProperty)
	{
		QObject::disconnect(m_activeProperty, &QObject::destroyed, this,
			&QtnPropertyView::onActivePropertyDestroyed);
	}
}

void QtnPropertyView::onPropertyDidChange(QtnPropertyChangeReason reason)
{
	if (!reason)
		return;

	if (m_stopInvalidate)
	{
		m_lastChangeReason |= reason;
	} else
	{
		updateWithReason(reason);
	}

	emit propertiesChanged(reason);
}

QtnPropertyView::Item *QtnPropertyView::findItem(
	Item *currentItem, const QtnPropertyBase *property) const
{
	if (nullptr != currentItem && nullptr != property)
	{
		if (property == currentItem->property)
			return currentItem;

		for (auto &item : currentItem->children)
		{
			auto found = findItem(item.get(), property);

			if (nullptr != found)
				return found;
		}
	}

	return nullptr;
}

QtnPropertyView::VisibleItem::VisibleItem()
	: item(nullptr)
	, level(0)
	, hasChildren(false)
	, subItemsValid(false)
{
}

void QtnPropertyView::onPropertySetDestroyed()
{
	m_propertySet = nullptr;
	updateItemsTree();
}

void QtnPropertyView::updateWithReason(QtnPropertyChangeReason reason)
{
	if (reason & QtnPropertyChangeReasonChildren)
	{
		updateItemsTree();
	} else if (reason & QtnPropertyChangeReasonState)
	{
		invalidateVisibleItems();
	} else
	{
		viewport()->update();
	}
}

QtnPainterState::QtnPainterState(QPainter &p)
	: m_p(p)
{
	m_p.save();
}

QtnPainterState::~QtnPainterState()
{
	m_p.restore();
}
