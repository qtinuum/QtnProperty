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

#ifndef QTN_PROPERTYVIEW_H
#define QTN_PROPERTYVIEW_H

#include "PropertyWidgetAPI.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "../Core/Property.h"

#include <QAbstractScrollArea>
#include <QScopedPointer>

class QRubberBand;
class QHelpEvent;

namespace Qtinuum
{

class QTN_PW_EXPORT PropertyViewFilter
{
public:
    virtual bool accept(const Property *property) const = 0;

    virtual ~PropertyViewFilter() {}

protected:
    PropertyViewFilter() {}
};

enum PropertyViewStyleFlag
{
    PropertyViewStyleNone = 0x0000,
    PropertyViewStyleShowRoot = 0x0001,
    PropertyViewStyleLiveSplit = 0x0002,
    PropertyViewStyleDblClickActivation = 0x0004
    //PropertyViewStyle = 0x0008
};
Q_DECLARE_FLAGS(PropertyViewStyle, PropertyViewStyleFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(PropertyViewStyle)


class QTN_PW_EXPORT PropertyView: public QAbstractScrollArea
{
    Q_OBJECT
    Q_DISABLE_COPY(PropertyView)

public:
    explicit PropertyView(QWidget* parent = nullptr, Property *property = nullptr);
    ~PropertyView();

    const Property *property() const { return m_property; }
    Property *property() { return m_property; }
    void setProperty(Property *newProperty);

    Property *activeProperty() { return m_activeProperty; }
    const Property *activeProperty() const { return m_activeProperty; }
    bool setActiveProperty(Property *newActiveProperty);

    bool ensureVisible(const Property *property);

    float itemHeightRatio() const { return m_itemHeightRatio; }
    bool setItemHeightRatio(float itemHeightRatio);

    PropertyViewStyle propertyViewStyle() const { return m_style; }
    void setPropertyViewStyle(PropertyViewStyle style);
    void addPropertyViewStyle(PropertyViewStyle style);
    void removePropertyViewStyle(PropertyViewStyle style);

Q_SIGNALS:
    // emits when active property has changed
    void activePropertyChanged(Property *activeProperty);

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
    void tooltipEvent(QHelpEvent *e);

private:

    struct Item
    {
        Property* property;
        QScopedPointer<PropertyDelegate> delegate;
        int level;

        Item *parent;
        QList<QSharedPointer<Item> > children;

        Item()
            : property(nullptr),
              level(0),
              parent(nullptr)
        {}

        bool collapsed() const { return property->stateLocal() & PropertyStateCollapsed; }
        bool isPropertySet() const { return !delegate; }
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

        VisibleItem()
            : item(nullptr),
              level(0),
              hasChildren(false),
              actionsValid(false),
              needTooltip(false)
        {
        }
    };

private:
    void updateItemsTree();
    static Item *createItemsTree(Property *rootProperty, PropertyDelegateFactory &factory);

    void invalidateVisibleItems();
    void validateVisibleItems() const;
    void fillVisibleItems(Item *item, int level) const;
    bool acceptItem(Item &item) const;

    void drawBranchNode(QStylePainter &painter, QRect &rect, const VisibleItem &vItem);
    void drawPropertySetItem(QStylePainter &painter, const QRect &rect, const VisibleItem &vItem);
    void drawPropertyItem(QStylePainter &painter, const QRect &rect, const VisibleItem &vItem);

    void changeActivePropertyByIndex(int index);
    int visibleItemIndexByPoint(QPoint pos) const;
    int visibleItemIndexByProperty(const Property *property) const;
    QRect visibleItemRect(int index) const;

    bool processItemActionByMouse(int index, QMouseEvent *e);

    void updateVScrollbar() const;
    void updateStyleStuff();

    bool ensureVisibleItemByIndex(int index);
    void invalidateVisibleItemsActions();

    int splitPosition() const;
    void updateSplitRatio(float splitRatio);

    void OnPropertyDidChange(const Property *changedProperty, const Property *firedProperty, PropertyChangeReason reason);

private:
    Property *m_property;
    Property *m_activeProperty;

    PropertyDelegateFactory m_delegateFactory;

    QScopedPointer<Item> m_itemsTree;

    mutable QList<VisibleItem> m_visibleItems;
    mutable bool m_visibleItemsValid;

    PropertyViewStyle m_style;
    int m_itemHeight;
    float m_itemHeightRatio;
    int m_leadMargin;
    QColor m_linesColor;
    QColor m_propertySetBackdroundColor;

    float m_splitRatio;
    QRubberBand *m_rubberBand;
};

} // end namespace Qtinuum

#endif // QTN_PROPERTYVIEW_H
