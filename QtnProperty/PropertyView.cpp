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

#include "PropertyWidget.h"

#include "Utils/InplaceEditing.h"

#include <QtGui>
#include <QApplication>
#include <QScrollBar>
//#include <QStyleOptionFrameV3>
#include <QHelpEvent>
#include <QToolTip>

class QtnPainterState
{
public:
    QtnPainterState(QPainter& p)
        : m_p(p)
    {
        m_p.save();
    }

    ~QtnPainterState()
    {
        m_p.restore();
    }

private:
    QPainter& m_p;
};

static void updateVisibleProperties(const QtnPropertyBase* property, unsigned int& count, bool simpleOnly)
{
    Q_ASSERT(property);

    if (!property->isVisible())
        return;

    if (simpleOnly && !property->isSimple())
        return;

    ++count;

    const QtnPropertySet* propertySet = property->asPropertySet();
    if (!propertySet)
        return;

    if (!propertySet->childProperties().isEmpty() && !(propertySet->state()&QtnPropertyStateCollapsed))
    {
        foreach(QtnPropertyBase* childProperty, propertySet->childProperties())
        {
            updateVisibleProperties(childProperty, count, simpleOnly);
        }
    }
}

extern void set_smaller_text_osx(QWidget *w);

QtnPropertyView::QtnPropertyView(QWidget* parent, QtnPropertySet* propertySet)
    : QAbstractScrollArea(parent),
      m_propertySet(propertySet),
      m_activeProperty(nullptr),
      m_delegateFactory(&QtnPropertyDelegateFactory::staticInstance()),
      m_visibleItemsValid(false),
      m_grabMouseSubItem(nullptr),
      m_style(QtnPropertyViewStyleLiveSplit),
      m_itemHeight(0),
      m_itemHeightSpacing(6),
      m_valueLeftMargin(0),
      m_splitRatio(0.5f),
      m_rubberBand(nullptr),
      m_mouseAtSplitter(false),
      m_accessibilityProxy(nullptr)
{
  set_smaller_text_osx(this);
  
  setFocusPolicy(Qt::StrongFocus);
  viewport()->setMouseTracking(true);

  updateStyleStuff();

  updateItemsTree();
}

QtnPropertyView::~QtnPropertyView()
{
}

QtnAccessibilityProxy *QtnPropertyView::accessibilityProxy()
{
    if (!m_accessibilityProxy)
        m_accessibilityProxy = new QtnAccessibilityProxy(this);

    return m_accessibilityProxy;
}


void QtnPropertyView::setPropertySet(QtnPropertySet* newPropertySet)
{
    if (m_propertySet){
        QObject::disconnect(m_propertySet, &QtnPropertyBase::propertyDidChange, this, &QtnPropertyView::onPropertyDidChange);
        QObject::disconnect(m_propertySet, &QtnPropertyBase::destroyed, this, &QtnPropertyView::onPropertyDestroyed);
    }

    m_propertySet = newPropertySet;

    if (m_propertySet){
        QObject::connect(m_propertySet, &QtnPropertyBase::propertyDidChange, this, &QtnPropertyView::onPropertyDidChange);
        QObject::connect(m_propertySet, &QtnPropertyBase::destroyed, this, &QtnPropertyView::onPropertyDestroyed);
    }

    updateItemsTree();
}

bool QtnPropertyView::setActiveProperty(QtnPropertyBase* newActiveProperty)
{
    if (m_activeProperty == newActiveProperty)
        return false;

    qtnStopInplaceEdit();

    if (!newActiveProperty)
    {
        m_activeProperty = nullptr;
        emit activePropertyChanged(activeProperty());
        viewport()->update();
        return true;
    }

    int index = visibleItemIndexByProperty(newActiveProperty);
    if (index < 0)
        return false;

    m_activeProperty = newActiveProperty;
    emit activePropertyChanged(activeProperty());
    viewport()->update();
    return true;
}

bool QtnPropertyView::ensureVisible(const QtnPropertyBase* property)
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

int QtnPropertyView::valueLeftMargin() const
{
    return m_valueLeftMargin;
}

void QtnPropertyView::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    validateVisibleItems();

    if (m_visibleItems.isEmpty())
    {
        return;
    }

    int firstVisibleItemIndex = qMin(verticalScrollBar()->value() / m_itemHeight, (m_visibleItems.size() - 1));
    int lastVisibleItemIndex = qMin(((verticalScrollBar()->value() + viewport()->height()) / m_itemHeight) + 1, (m_visibleItems.size() - 1));

    QRect itemRect = viewport()->rect();
    itemRect.setTop(firstVisibleItemIndex * m_itemHeight - verticalScrollBar()->value());
    itemRect.setBottom(itemRect.top() + m_itemHeight);

    QStylePainter painter(viewport());

    for (int i = firstVisibleItemIndex; i <= lastVisibleItemIndex; ++i)
    {
        const VisibleItem& vItem = m_visibleItems[i];
        drawItem(painter, itemRect, vItem);
        itemRect.translate(0, m_itemHeight);
    }
}

void QtnPropertyView::drawItem(QStylePainter& painter, const QRect& rect, const VisibleItem& vItem) const
{
    if (!vItem.item->delegate.data())
    {
        //drawPropertyDelegateNotImpl();
        return;
    }

    QMargins margins(m_valueLeftMargin + rect.height() * vItem.level, 0, 0, 0);
    bool isActive = (m_activeProperty == vItem.item->property);

    QtnDrawContext drawContext{&painter, this,
                                               rect, margins, splitPosition(),
                                               isActive, vItem.hasChildren};

    // create sub-items if not initialized
    if (!vItem.subItemsValid)
    {
        Q_ASSERT(vItem.subItems.isEmpty());
        vItem.item->delegate->createSubItems(drawContext, vItem.subItems);
        vItem.subItemsValid = true;
    }

    // draw sub-items
    for (const auto& subItem : vItem.subItems)
    {
        subItem.draw(drawContext);
    }
}

void QtnPropertyView::changeActivePropertyByIndex(int index)
{
    QtnPropertyBase* newActiveProperty = (index < 0) ? nullptr : m_visibleItems[index].item->property;
    setActiveProperty(newActiveProperty);
    ensureVisibleItemByIndex(index);
}

int QtnPropertyView::visibleItemIndexByPoint(QPoint pos) const
{
    int index = (verticalScrollBar()->value() + pos.y()) / m_itemHeight;
    if (index >= m_visibleItems.size())
        return -1;

    return index;
}

int QtnPropertyView::visibleItemIndexByProperty(const QtnPropertyBase* property) const
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

bool QtnPropertyView::handleMouseEvent(int index, QEvent* e, QPoint mousePos)
{
    if (index < 0)
    {
        deactivateSubItems();
        return false;
    }

    QtnEventContext context{e, this};
    return handleEvent(context, m_visibleItems[index], mousePos);
}

void QtnPropertyView::resizeEvent(QResizeEvent* e)
{
    Q_UNUSED(e);

    qtnStopInplaceEdit();
    invalidateSubItems();
    updateVScrollbar();
}

static const int TOLERANCE = 3;

void QtnPropertyView::mousePressEvent(QMouseEvent* e)
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
    }
    else
    {
        int index = visibleItemIndexByPoint(e->pos());
        if (index >= 0)
        {
            changeActivePropertyByIndex(index);
            handleMouseEvent(index, e, e->pos());
        }
    }
}

void QtnPropertyView::mouseReleaseEvent(QMouseEvent* e)
{
    if (m_rubberBand)
    {
        delete m_rubberBand;
        m_rubberBand = nullptr;

        // update split ratio
        QRect rect = viewport()->rect();
        updateSplitRatio((float)(e->x() - rect.left()) / (float)rect.width());
    }
    else
    {
        handleMouseEvent(visibleItemIndexByPoint(e->pos()), e, e->pos());
    }
}

void QtnPropertyView::mouseMoveEvent(QMouseEvent* e)
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
            updateSplitRatio((float)(e->x() - rect.left()) / (float)rect.width());
        }
    }
    else if (qAbs(e->x() - splitPosition()) < TOLERANCE)
    {
        if (!m_mouseAtSplitter)
        {
            m_mouseAtSplitter = true;
            m_oldCursor = cursor();
            setCursor(Qt::SplitHCursor);
        }
    }
    else
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

void QtnPropertyView::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (!m_rubberBand)
    {
       handleMouseEvent(visibleItemIndexByPoint(e->pos()), e, e->pos());
    }
}

bool QtnPropertyView::viewportEvent(QEvent* e)
{
    switch (e->type())
    {
    case QEvent::StyleChange:
        updateStyleStuff();
        break;

    case QEvent::ToolTip:
        {
            QHelpEvent* helpEvent = static_cast<QHelpEvent*>(e);
            tooltipEvent(helpEvent);
        }
        break;

    //case QEvent::Enter:
    case QEvent::Leave:
        deactivateSubItems();
        break;

    default:
        ;// do nothing
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

void QtnPropertyView::keyPressEvent(QKeyEvent* e)
{
    validateVisibleItems();

    if (m_visibleItems.empty())
    {
        QAbstractScrollArea::keyPressEvent(e);
        return;
    }

    QWidget* inplaceEditor = qtnGetInplaceEdit();
    if (inplaceEditor)
    {
        int key = e->key();
        if (key == Qt::Key_Escape || key == Qt::Key_Return || key == Qt::Key_Enter)
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
                changeActivePropertyByIndex(qMin(m_visibleItems.size() - 1, index + 1));
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
                int itemsPerPage = qMax(viewport()->rect().height() / m_itemHeight, 1);
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
                int itemsPerPage = qMax(viewport()->rect().height() / m_itemHeight, 1);
                changeActivePropertyByIndex(qMin(m_visibleItems.size() - 1, index + itemsPerPage));
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
                const VisibleItem& vItem = m_visibleItems[index];
                if (vItem.hasChildren && !vItem.item->collapsed())
                {
                    // collapse opened property
                    vItem.item->property->addState(QtnPropertyStateCollapsed);
                }
                else if (vItem.item->parent)
                {
                    // activate parent property
                    setActiveProperty(vItem.item->parent->property);
                    ensureVisible(activeProperty());
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
                const VisibleItem& vItem = m_visibleItems[index];
                if (vItem.hasChildren && vItem.item->collapsed())
                {
                    // expand closed property
                    vItem.item->property->removeState(QtnPropertyStateCollapsed);
                }
                else if (vItem.hasChildren)
                {
                    // activate child property
                    setActiveProperty(vItem.item->children.first()->property);
                    ensureVisible(activeProperty());
                }
            }
            break;
        }

        default:
        {
            int index = visibleItemIndexByProperty(activeProperty());

            if (index >= 0)
            {
                QtnEventContext context{e, this};
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
    bool processed = handleMouseEvent(visibleItemIndexByPoint(e->pos()), e, e->pos());
    if (processed)
        return;

    QAbstractScrollArea::wheelEvent(e);
}

void QtnPropertyView::tooltipEvent(QHelpEvent* e)
{
    if (!handleMouseEvent(visibleItemIndexByPoint(e->pos()), e, e->pos()))
    {
        QToolTip::hideText();
    }
}

bool QtnPropertyView::handleEvent(QtnEventContext& context, VisibleItem& vItem, QPoint mousePos)
{
    if (!vItem.subItemsValid)
        return false;

    // process event
    if (m_grabMouseSubItem)
        return m_grabMouseSubItem->event(context);
    else
    {
        // update list of sub items under cursor
        QList<QtnSubItem*> activeSubItems;

        // make list of new active sub items
        for (auto& subItem: vItem.subItems)
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
                return true;
        }
    }

    return false;
}

bool QtnPropertyView::grabMouseForSubItem(QtnSubItem* subItem, QPoint mousePos)
{
    Q_ASSERT(!m_grabMouseSubItem);
    if (m_grabMouseSubItem)
        return false;

    viewport()->grabMouse();
    m_grabMouseSubItem = subItem;
    m_grabMouseSubItem->grabMouse(this, mousePos);

    return true;
}

bool QtnPropertyView::releaseMouseForSubItem(QtnSubItem* subItem, QPoint mousePos)
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
    m_itemsTree.reset(createItemsTree(m_propertySet, m_delegateFactory));
    invalidateVisibleItems();
}

QtnPropertyView::Item* QtnPropertyView::createItemsTree(QtnPropertyBase* rootProperty, const QtnPropertyDelegateFactory& factory)
{
    if (!rootProperty)
        return nullptr;

    Item* item(new Item());
    item->property = rootProperty;
    item->delegate.reset(factory.createDelegate(*rootProperty));

    if (item->delegate)
    {
        // apply attributes
        const QtnPropertyDelegateInfo* delegateInfo = rootProperty->delegate();
        if (delegateInfo)
        {
            item->delegate->applyAttributes(delegateInfo->attributes);
        }

        // process delegate subproperties
        for (int i = 0, n = item->delegate->subPropertyCount(); i < n; ++i)
        {
            QtnPropertyBase* child = item->delegate->subProperty(i);
            if (child)
            {
                QSharedPointer<Item> childItem(createItemsTree(child, factory));
                childItem->parent = item;
                item->children.append(childItem);
            }
        }
    }

    return item;
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

    //m_visibleItems.clear();
    fillVisibleItems(m_itemsTree.data(), (m_style & QtnPropertyViewStyleShowRoot) ? 0 : -1);

    updateVScrollbar();

    m_visibleItemsValid = true;
}

void QtnPropertyView::fillVisibleItems(Item* item, int level) const
{
    if (!item)
        return;

    // process children only for negative levels
    if (level < 0)
    {
        // process children
        for (int i = 0, n = item->children.size(); i < n; ++i)
            fillVisibleItems(item->children[i].data(), level + 1);

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
        for (int i = 0, n = item->children.size(); i < n; ++i)
        {
            if (acceptItem(*item->children[i].data()))
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
    for (int i = 0, n = item->children.size(); i < n; ++i)
        fillVisibleItems(item->children[i].data(), level + 1);

    // if we add something -> current item has children
    if (index < (m_visibleItems.size() - 1))
        m_visibleItems[index].hasChildren = true;
}

bool QtnPropertyView::acceptItem(Item& item) const
{
    return item.property->isVisible();
}

void QtnPropertyView::updateVScrollbar() const
{
    int viewportHeight = viewport()->height();
    int virtualHeight = m_itemHeight * m_visibleItems.size();

    verticalScrollBar()->setSingleStep(m_itemHeight);
    verticalScrollBar()->setPageStep(viewportHeight);
    verticalScrollBar()->setRange(0, qMax(0, virtualHeight - viewportHeight + 2));
}

void QtnPropertyView::updateStyleStuff()
{
    QFontMetrics fm(font());
    m_itemHeight = fm.height() + m_itemHeightSpacing;

    m_propertySetBackdroundColor = m_linesColor = palette().color(QPalette::Button);

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

    for (auto& item: m_visibleItems)
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
    return (int)((float)viewport()->rect().width() * m_splitRatio);
}

void QtnPropertyView::updateSplitRatio(float splitRatio)
{
    m_splitRatio = qBound(0.f, splitRatio, 1.f);
    // firce to regenerate sub-items
    invalidateSubItems();
    // repaint
    viewport()->update();
}

void QtnPropertyView::onPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
    Q_UNUSED(changedProperty);
    Q_UNUSED(firedProperty);

    if (reason & QtnPropertyChangeReasonChildren)
    {
        // regrow tree
        updateItemsTree();
    }
    else if (reason & QtnPropertyChangeReasonState)
    {
        invalidateVisibleItems();
        viewport()->update();
    }
    else if (reason & QtnPropertyChangeReasonValue)
    {
        viewport()->update();
    }
}

void QtnPropertyView::onPropertyDestroyed()
{
    setPropertySet(0);
}

