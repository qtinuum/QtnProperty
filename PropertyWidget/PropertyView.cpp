/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
 */

#include "PropertyWidget.h"

#include "Utils/InplaceEditing.h"

#include <QtGui>
#include <QApplication>
#include <QScrollBar>
#include <QStyleOptionFrameV3>
#include <QHelpEvent>
#include <QToolTip>

namespace Qtinuum
{

class PainterState
{
public:
    PainterState(QPainter &p)
        : m_p(p)
    {
        m_p.save();
    }

    ~PainterState()
    {
        m_p.restore();
    }

private:
    QPainter &m_p;
};

static void updateVisibleProperties(const Property *property, unsigned int &count, bool simpleOnly)
{
    Q_ASSERT(property);

    if (!property->isVisible())
        return;

    if (simpleOnly && !property->isSimple())
        return;

    ++count;

    if (!property->childProperties().isEmpty() && !(property->state()&PropertyStateCollapsed))
    {
        foreach(Property *childProperty, property->childProperties())
        {
            updateVisibleProperties(childProperty, count, simpleOnly);
        }
    }
}

PropertyView::PropertyView(QWidget* parent, Property *property)
    : QAbstractScrollArea(parent),
      m_property(property),
      m_activeProperty(nullptr),
      m_delegateFactory(&PropertyDelegateFactory::staticInstance()),
      m_visibleItemsValid(false),
      m_style(PropertyViewStyleLiveSplit),
      m_itemHeight(0),
      m_itemHeightRatio(1.f),
      m_leadMargin(0),
      m_splitRatio(0.5f),
      m_rubberBand(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
    viewport()->setMouseTracking(true);

    updateStyleStuff();

    updateItemsTree();
}

PropertyView::~PropertyView()
{
}

void PropertyView::setProperty(Property *newProperty)
{
    if (m_property)
        QObject::disconnect(m_property, &Property::propertyDidChange, this, &PropertyView::OnPropertyDidChange);

    m_property = newProperty;

    if (m_property)
        QObject::connect(m_property, &Property::propertyDidChange, this, &PropertyView::OnPropertyDidChange);

    updateItemsTree();
}

bool PropertyView::setActiveProperty(Property *newActiveProperty)
{
    if (m_activeProperty == newActiveProperty)
        return false;

    stopInplaceEdit();

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

bool PropertyView::ensureVisible(const Property *property)
{
    if (!property)
        return false;

    int index = visibleItemIndexByProperty(property);
    return ensureVisibleItemByIndex(index);
}

bool PropertyView::setItemHeightRatio(float itemHeightRatio)
{
    if (itemHeightRatio <= 0.f)
        return false;

    m_itemHeightRatio = itemHeightRatio;
    updateStyleStuff();
    return true;
}

void PropertyView::setPropertyViewStyle(PropertyViewStyle style)
{
    m_style = style;
}

void PropertyView::addPropertyViewStyle(PropertyViewStyle style)
{
    setPropertyViewStyle(propertyViewStyle() | style);
}

void PropertyView::removePropertyViewStyle(PropertyViewStyle style)
{
    setPropertyViewStyle(propertyViewStyle() & ~style);
}

void PropertyView::paintEvent(QPaintEvent *e)
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
        const VisibleItem &vItem = m_visibleItems[i];

        if (vItem.item->isPropertySet())
            drawPropertySetItem(painter, itemRect, vItem);
        else
            drawPropertyItem(painter, itemRect, vItem);

        // mark as valid actions after any painting
        vItem.actionsValid = true;

        itemRect.translate(0, m_itemHeight);
    }
}

void PropertyView::drawBranchNode(QStylePainter &painter, QRect &rect, const VisibleItem &vItem)
{
    if (vItem.hasChildren)
    {
        QRectF branchRect(rect.left(), rect.top(), rect.height(), rect.height());
        //painter.drawRect(branchRect);
        qreal side = branchRect.height() / 3.5f;

        QPainterPath branchPath;
        if (vItem.item->collapsed())
        {
            branchPath.moveTo(branchRect.left() + side, branchRect.top() + side);
            branchPath.lineTo(branchRect.right() - side - 1, branchRect.top() + branchRect.height() / 2.f);
            branchPath.lineTo(branchRect.left() + side, branchRect.bottom() - side);
            branchPath.closeSubpath();
        }
        else
        {
            branchPath.moveTo(branchRect.left() + side, branchRect.top() + side);
            branchPath.lineTo(branchRect.right() - side, branchRect.top() + side);
            branchPath.lineTo(branchRect.left() + branchRect.width() / 2.f, branchRect.bottom() - side - 1);
            branchPath.closeSubpath();
        }

        if (painter.testRenderHint(QPainter::Antialiasing))
        {
            painter.fillPath(branchPath, palette().color(QPalette::Text));
        }
        else
        {
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.fillPath(branchPath, palette().color(QPalette::Text));
            painter.setRenderHint(QPainter::Antialiasing, false);
        }

        // add action to collapse/expand branch
        if (!vItem.actionsValid)
        {
            Action branch;
            branch.rect = branchRect.toRect();//opt.rect;

            Property* property = vItem.item->property;
            branch.action = [property](QEvent *e, QRect rect)->bool {
                if (e->type() == QEvent::MouseButtonPress)
                {
                    property->switchStateAuto(PropertyStateCollapsed);
                    return true;
                }

                return false;
            };

            vItem.actions.append(branch);
        }

        // skip branch space
        rect.setLeft(branchRect.right() + 1);
    }
}

void PropertyView::drawPropertySetItem(QStylePainter &painter, const QRect &rect, const VisibleItem &vItem)
{
    bool enabled = vItem.item->property->isEditableByUser();
    bool selected = m_activeProperty == vItem.item->property;

    QPalette::ColorGroup cg = enabled ? QPalette::Active : QPalette::Disabled;

    // fill background
    painter.fillRect(rect, selected ? palette().color(QPalette::Highlight) : m_propertySetBackdroundColor);

    QRect nameRect = rect;

    // skip levels indent
    nameRect.setLeft(nameRect.left() + m_leadMargin + nameRect.height() * vItem.level);
    if (!nameRect.isValid())
        return;

    // draw branch node
    drawBranchNode(painter, nameRect, vItem);
    if (!nameRect.isValid())
        return;

    PainterState s(painter);

    // draw name
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);

    painter.setPen(palette().color(cg, selected ? QPalette::HighlightedText : QPalette::Text));

    QString elidedName = painter.fontMetrics().elidedText(vItem.item->property->name(), Qt::ElideRight, nameRect.width());
    painter.drawText(nameRect, Qt::AlignLeading|Qt::AlignVCenter|Qt::TextSingleLine, elidedName);
}

void PropertyView::drawPropertyItem(QStylePainter &painter, const QRect &rect, const VisibleItem &vItem)
{
    bool enabled = vItem.item->property->isEditableByUser();
    bool selected = m_activeProperty == vItem.item->property;

    QStyle::State state = QStyle::State_Active;
    if (enabled)
        state |= QStyle::State_Enabled;
    if (selected)
    {
        state |= QStyle::State_Selected;
        state |= QStyle::State_HasFocus;
    }

    QPalette::ColorGroup cg = enabled ? QPalette::Active : QPalette::Disabled;

    // fill background
    if (selected)
        painter.fillRect(rect, palette().color(QPalette::Highlight));

    int splitPos = splitPosition();

    PainterState s(painter);

    QPen linesPen(m_linesColor);
    painter.setPen(linesPen);

    painter.drawLine(rect.bottomLeft(), rect.bottomRight());
    painter.drawLine(splitPos, rect.top(), splitPos, rect.bottom());

    QRect nameRect = rect;
    QRect valueRect = rect;
    QRect editRect = rect;

    nameRect.setRight(splitPosition());
    valueRect.setLeft(splitPosition() + m_leadMargin + 1);
    editRect.setLeft(splitPosition() + 1);

    // skip levels space
    nameRect.setLeft(nameRect.left() + m_leadMargin + nameRect.height() * vItem.level);
    if (!nameRect.isValid())
        return;

    // draw branch node
    drawBranchNode(painter, nameRect, vItem);
    if (!nameRect.isValid())
        return;

    // draw name
    painter.setPen(palette().color(cg, selected ? QPalette::HighlightedText : QPalette::Text));

    painter.drawText(  nameRect, Qt::AlignLeading|Qt::AlignVCenter|Qt::TextSingleLine
                     , elidedText(painter, vItem.item->property->name(), nameRect));

    // draw property value
    if (valueRect.isValid())
    {
        vItem.needTooltip = false;

        // draw property value
        vItem.item->delegate->drawValue(painter, valueRect, state, &vItem.needTooltip);

        // add action to edit property value
        if (!vItem.actionsValid)
        {
            Action edit;
            edit.rect = editRect;

            PropertyDelegate* propertyDelegate = vItem.item->delegate.data();
            edit.action = [propertyDelegate, this](QEvent *e, QRect rect)->bool {
                bool doEdit = false;

                if (this->propertyViewStyle() & PropertyViewStyleDblClickActivation)
                {
                    doEdit = (e->type() == QEvent::MouseButtonDblClick);
                }
                else
                {
                    doEdit = (e->type() == QEvent::MouseButtonRelease);
                }

                if (doEdit)
                {
                    InplaceInfo inplaceInfo;
                    inplaceInfo.activationEvent = e;
                    QWidget *editor = propertyDelegate->createValueEditor(viewport(), rect, &inplaceInfo);
                    if (!editor)
                        return false;

                    if (!editor->isVisible())
                        editor->show();

                    startInplaceEdit(editor);

                    return true;
                }

                return false;
            };

            vItem.actions.append(edit);
        }
    }
}

void PropertyView::changeActivePropertyByIndex(int index)
{
    Property *newActiveProperty = (index < 0) ? nullptr : m_visibleItems[index].item->property;
    setActiveProperty(newActiveProperty);
    ensureVisibleItemByIndex(index);
}

int PropertyView::visibleItemIndexByPoint(QPoint pos) const
{
    int index = (verticalScrollBar()->value() + pos.y()) / m_itemHeight;
    if (index >= m_visibleItems.size())
        return -1;

    return index;
}

int PropertyView::visibleItemIndexByProperty(const Property *property) const
{
    validateVisibleItems();

    for (int i = 0, n = m_visibleItems.size(); i < n; ++i)
        if (m_visibleItems[i].item->property == property)
            return i;

    return -1;
}

QRect PropertyView::visibleItemRect(int index) const
{
    Q_ASSERT(index >= 0 && index < m_visibleItems.size());

    QRect rect = viewport()->rect();
    rect.setTop(index * m_itemHeight - verticalScrollBar()->value());
    rect.setHeight(m_itemHeight);

    return rect;
}

bool PropertyView::processItemActionByMouse(int index, QMouseEvent *e)
{
    if (index < 0)
        return false;

    const VisibleItem &vItem = m_visibleItems[index];

    if (!vItem.actionsValid)
        return false;

    const QList<Action>& actions = vItem.actions;
    foreach (const Action &action, actions)
    {
        if (action.rect.contains(e->pos()))
        {
            return action.action(e, action.rect);
        }
    }

    return false;
}

void PropertyView::resizeEvent(QResizeEvent *e)
{
    stopInplaceEdit();
    invalidateVisibleItemsActions();
    updateVScrollbar();
}

static const int TOLERANCE = 3;

void PropertyView::mousePressEvent(QMouseEvent *e)
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
            processItemActionByMouse(index, e);
        }
    }
}

void PropertyView::mouseReleaseEvent(QMouseEvent *e)
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
        processItemActionByMouse(visibleItemIndexByPoint(e->pos()), e);
    }
}

void PropertyView::mouseMoveEvent(QMouseEvent *e)
{
    if (m_rubberBand)
    {
        QRect rect = viewport()->rect();
        rect.setLeft(e->x());
        rect.setRight(e->x());
        m_rubberBand->setGeometry(rect);

        if (m_style & PropertyViewStyleLiveSplit)
        {
            // update split ratio
            QRect rect = viewport()->rect();
            updateSplitRatio((float)(e->x() - rect.left()) / (float)rect.width());
        }
    }
    else if (qAbs(e->x() - splitPosition()) < TOLERANCE)
    {
        setCursor(Qt::SplitHCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);

        int index = visibleItemIndexByPoint(e->pos());
        if (index >= 0)
        {
            if (e->buttons() & Qt::LeftButton)
                changeActivePropertyByIndex(index);
            else
                processItemActionByMouse(index, e);
        }
    }
}

void PropertyView::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (!m_rubberBand)
    {
       processItemActionByMouse(visibleItemIndexByPoint(e->pos()), e);
    }
}

bool PropertyView::viewportEvent(QEvent *e)
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
        }
        break;

    default:
        ;// do nothing
    }

    return QAbstractScrollArea::viewportEvent(e);
}

void PropertyView::scrollContentsBy(int dx, int dy)
{
    if (dx != 0 || dy != 0)
    {
        stopInplaceEdit();
        invalidateVisibleItemsActions();
    }

    QAbstractScrollArea::scrollContentsBy(dx, dy);
}

void PropertyView::keyPressEvent(QKeyEvent *e)
{
    validateVisibleItems();

    if (m_visibleItems.empty())
    {
        QAbstractScrollArea::keyPressEvent(e);
        return;
    }

    QWidget *inplaceEditor = getInplaceEdit();
    if (inplaceEditor)
    {
        int key = e->key();
        if (key == Qt::Key_Escape || key == Qt::Key_Return || key == Qt::Key_Enter)
        {
            stopInplaceEdit();
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
                const VisibleItem &vItem = m_visibleItems[index];
                if (vItem.hasChildren && !vItem.item->collapsed())
                {
                    // collapse opened property
                    vItem.item->property->addState(PropertyStateCollapsed);
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
                const VisibleItem &vItem = m_visibleItems[index];
                if (vItem.hasChildren && vItem.item->collapsed())
                {
                    // expand closed property
                    vItem.item->property->removeState(PropertyStateCollapsed);
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
                const QScopedPointer<PropertyDelegate> &delegate = m_visibleItems[index].item->delegate;
                if (!delegate.isNull() && delegate->acceptKeyPressedForInplaceEdit(e))
                {
                    qDebug() << "PropertyView Key event: " << e;

                    InplaceInfo inplaceInfo;
                    inplaceInfo.activationEvent = e;

                    QRect valueRect = visibleItemRect(index);
                    valueRect.setLeft(splitPosition());

                    QWidget *editor = delegate->createValueEditor(viewport(), valueRect, &inplaceInfo);
                    if (!editor)
                        break;

                    if (!editor->isVisible())
                        editor->show();

                    startInplaceEdit(editor);

                    // eat event
                    e->accept();
                    return;

                    //break;
                }
            }

            // process by default
            QAbstractScrollArea::keyPressEvent(e);
        }
    }
}

void PropertyView::tooltipEvent(QHelpEvent *e)
{
    int index = visibleItemIndexByPoint(e->pos());
    if (index >= 0)
    {
        QRect rect = visibleItemRect(index);
        int splitPos = splitPosition();

        QString tooltipText;

        const VisibleItem &vItem = m_visibleItems[index];

        // propertyset case
        if (vItem.item->isPropertySet())
        {
            tooltipText = vItem.item->property->description();
        }
        // name sub rect
        else if (e->x() < splitPos)
        {
            rect.setRight(splitPos);
            tooltipText = vItem.item->property->description();
        }
        // value sub rect
        else if (vItem.needTooltip)
        {
            rect.setLeft(splitPos);
            tooltipText = vItem.item->delegate->toolTip();
        }

        QToolTip::showText(  e->globalPos()
                           , tooltipText
                           , this
                           , rect);
    }
    else
    {
        QToolTip::hideText();
    }
}

void PropertyView::updateItemsTree()
{
    m_itemsTree.reset(createItemsTree(m_property, m_delegateFactory));
    invalidateVisibleItems();
}

PropertyView::Item *PropertyView::createItemsTree(Property *rootProperty, PropertyDelegateFactory &factory)
{
    if (!rootProperty)
        return nullptr;

    Item *item(new Item());
    item->property = rootProperty;
    item->delegate.reset(factory.createDelegate(*rootProperty));

    if (item->delegate)
    {
        // apply attributes
        const PropertyDelegateInfo * delegateInfo = rootProperty->delegate();
        if (delegateInfo)
        {
            item->delegate->applyAttributes(delegateInfo->attributes);
        }

        // process delegate subproperties
        for (int i = 0, n = item->delegate->subPropertyCount(); i < n; ++i)
        {
            Property *child = item->delegate->subProperty(i);
            if (child)
            {
                QSharedPointer<Item> childItem(createItemsTree(child, factory));
                childItem->parent = item;
                item->children.append(childItem);
            }
        }
    }

    // process root property subproperties
    foreach (Property *child, rootProperty->childProperties())
    {
        QSharedPointer<Item> childItem(createItemsTree(child, factory));
        childItem->parent = item;
        item->children.append(childItem);
    }

    return item;
}

void PropertyView::invalidateVisibleItems()
{
    m_visibleItemsValid = false;
    update();
}

void PropertyView::validateVisibleItems() const
{
    if (m_visibleItemsValid)
        return;

    m_visibleItems.clear();
    fillVisibleItems(m_itemsTree.data(), (m_style & PropertyViewStyleShowRoot) ? 0 : -1);

    updateVScrollbar();

    m_visibleItemsValid = true;
}

void PropertyView::fillVisibleItems(Item *item, int level) const
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

bool PropertyView::acceptItem(Item &item) const
{
    return item.property->isVisible();
}

void PropertyView::updateVScrollbar() const
{
    int viewportHeight = viewport()->height();
    int virtualHeight = m_itemHeight * m_visibleItems.size();

    verticalScrollBar()->setSingleStep(m_itemHeight);
    verticalScrollBar()->setPageStep(viewportHeight);
    verticalScrollBar()->setRange(0, qMax(0, virtualHeight - viewportHeight + 2));
}

void PropertyView::updateStyleStuff()
{
    QStyleOptionViewItem opt;
    opt.init(this);
    m_itemHeight = style()->sizeFromContents(QStyle::CT_ItemViewItem, &opt, QSize(0, 0), this).height();
    if (m_itemHeight == 0)
        m_itemHeight = opt.fontMetrics.height();

    m_itemHeight = (int)((float)m_itemHeight * m_itemHeightRatio);

    m_propertySetBackdroundColor = m_linesColor = palette().color(QPalette::Button);
            //style()->styleHint(QStyle::SH_Table_GridLineColor, &opt, this, 0);

    m_leadMargin = style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing);
}

bool PropertyView::ensureVisibleItemByIndex(int index)
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

void PropertyView::invalidateVisibleItemsActions()
{
    for (auto it = m_visibleItems.begin(); it != m_visibleItems.end(); ++it)
    {
        (*it).actionsValid = false;
        (*it).actions.clear();
    }
}

int PropertyView::splitPosition() const
{
    return (int)((float)viewport()->rect().width() * m_splitRatio);
}

void PropertyView::updateSplitRatio(float splitRatio)
{
    m_splitRatio = qBound(0.f, splitRatio, 1.f);
    // firce to regenerate actions
    invalidateVisibleItemsActions();
    // repaint
    viewport()->update();
}

void PropertyView::OnPropertyDidChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason)
{
    if (reason & PropertyChangeReasonChildren)
    {
        // regrow tree
        updateItemsTree();
    }
    else if (reason & PropertyChangeReasonState)
    {
        invalidateVisibleItems();
        viewport()->update();
    }
    else if (reason & PropertyChangeReasonValue)
    {
        viewport()->update();
    }
}

} // end namespace Qtinuum
