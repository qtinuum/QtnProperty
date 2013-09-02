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

#ifndef QTN_PROPERTY_DELEGATE_H
#define QTN_PROPERTY_DELEGATE_H

#include "../PropertyWidgetAPI.h"
#include "../../Core/Property.h"
//#include "SignalLite.h"

#include <functional>
#include <QStylePainter>

class QKeyEvent;

namespace Qtinuum
{

class PropertyDelegateFactory;

class QTN_PW_EXPORT InplaceInfo
{
public:
    QEvent *activationEvent;

    InplaceInfo()
        : activationEvent(0)
    {
    }
};

QTN_PW_EXPORT QString elidedText(const QPainter &painter, const QString &text, const QRect &rect, bool *elided = 0);

class QTN_PW_EXPORT PropertyDelegate
{
    Q_DISABLE_COPY(PropertyDelegate)

public:
    virtual ~PropertyDelegate() {}

    // for complex properties like PropertyQFont
    int subPropertyCount() const;
    Property *subProperty(int index);

    // tune up with attributes
    void applyAttributes(const PropertyDelegateAttributes &attributes);

    void drawValue(QStylePainter &painter, const QRect &rect, const QStyle::State &state, bool *needTooltip = 0) const;
    QString toolTip() const;
    bool acceptKeyPressedForInplaceEdit(QKeyEvent *keyEvent) const;
    QWidget *createValueEditor(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo = 0);

protected:
    PropertyDelegate() {}

    virtual int subPropertyCountImpl() const { return 0; }
    virtual Property *subPropertyImpl(int index) { return 0; }
    virtual void applyAttributesImpl(const PropertyDelegateAttributes &attributes) {}
    virtual void drawValueImpl(QStylePainter &painter, const QRect &rect, const QStyle::State &state, bool *needTooltip = 0) const;
    virtual QString toolTipImpl() const;
    virtual bool acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const;
    virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo = 0) = 0;

    // override this if property value can be displayed as string
    virtual bool propertyValueToStr(QString &strValue) const { return false; }

    // helper functions
    static void drawValueText(const QString &text, QStylePainter &painter, const QRect &rect, const QStyle::State &state, bool *needTooltip = 0);
    QWidget *createValueEditorLineEdit(QWidget *parent, const QRect &rect, bool readOnly, InplaceInfo *inplaceInfo = 0);
};

template <typename PropertyClass>
class PropertyDelegateTyped: public PropertyDelegate
{
    Q_DISABLE_COPY(PropertyDelegateTyped)

public:
    const PropertyClass &owner() const { return m_owner; }
    PropertyClass &owner() { return m_owner; }

protected:
    PropertyDelegateTyped(PropertyClass &owner)
        : m_owner(owner)
    {
    }

    ~PropertyDelegateTyped()
    {
    }

private:
    PropertyClass &m_owner;
};

template <typename PropertyClass>
class PropertyDelegateTypedEx: public PropertyDelegateTyped<PropertyClass>
{
    Q_DISABLE_COPY(PropertyDelegateTypedEx)

protected:
    PropertyDelegateTypedEx(PropertyClass &owner)
        : PropertyDelegateTyped<PropertyClass>(owner)
    {
    }

    ~PropertyDelegateTypedEx()
    {
    }

    int subPropertyCountImpl() const override { return m_subProperties.size(); }
    Property *subPropertyImpl(int index) override { return m_subProperties[index].data(); }

    void addSubProperty(Property *subProperty)
    {
        Q_ASSERT(subProperty);
        if (!subProperty)
            return;

        m_subProperties.append(QSharedPointer<Property>(subProperty));
    }

private:
    QList<QSharedPointer<Property>> m_subProperties;
};

} // end namespace Qtinuum

#endif // QTN_PROPERTY_DELEGATE_H
