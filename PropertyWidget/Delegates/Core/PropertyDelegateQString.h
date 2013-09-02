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

#ifndef PROPERTY_DELEGATE_QSTRING_H
#define PROPERTY_DELEGATE_QSTRING_H

#include "../PropertyDelegate.h"

namespace Qtinuum
{

class PropertyQStringBase;

class QTN_PW_EXPORT PropertyDelegateQString: public PropertyDelegateTyped<PropertyQStringBase>
{
    Q_DISABLE_COPY(PropertyDelegateQString)

public:
    PropertyDelegateQString(PropertyQStringBase &owner);

protected:
    bool acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const override;
    QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo = 0) override;
    bool propertyValueToStr(QString &strValue) const override;
};

class QTN_PW_EXPORT PropertyDelegateQStringInvalidBase: public PropertyDelegateQString
{
    Q_DISABLE_COPY(PropertyDelegateQStringInvalidBase)

protected:
    PropertyDelegateQStringInvalidBase(PropertyQStringBase &owner);

    void applyAttributesImpl(const PropertyDelegateAttributes &attributes) override;
    void drawValueImpl(QStylePainter &painter, const QRect &rect, const QStyle::State &state, bool *needTooltip = 0) const override;

    virtual bool isPropertyValid() const = 0;

private:
    QColor m_invalidColor;
};

class QTN_PW_EXPORT PropertyDelegateQStringFile: public PropertyDelegateQStringInvalidBase
{
    Q_DISABLE_COPY(PropertyDelegateQStringFile)

public:
    PropertyDelegateQStringFile(PropertyQStringBase &owner);

protected:
    void applyAttributesImpl(const PropertyDelegateAttributes &attributes) override;
    QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo = 0) override;

    bool isPropertyValid() const override;

private:
    PropertyDelegateAttributes m_editorAttributes;
};

class QTN_PW_EXPORT PropertyDelegateQStringList: public PropertyDelegateQString
{
    Q_DISABLE_COPY(PropertyDelegateQStringList)

public:
    PropertyDelegateQStringList(PropertyQStringBase &owner);

protected:
    void applyAttributesImpl(const PropertyDelegateAttributes &attributes) override;
    QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo = 0) override;

private:
    QStringList m_items;
};

} // end namespace Qtinuum

#endif // PROPERTY_DELEGATE_QSTRING_H
