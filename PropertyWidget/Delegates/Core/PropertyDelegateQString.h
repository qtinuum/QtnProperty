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

class QtnPropertyQStringBase;

class QTN_PW_EXPORT QtnPropertyDelegateQString: public QtnPropertyDelegateTyped<QtnPropertyQStringBase>
{
    Q_DISABLE_COPY(QtnPropertyDelegateQString)

public:
    QtnPropertyDelegateQString(QtnPropertyQStringBase& owner);

protected:
    bool acceptKeyPressedForInplaceEditImpl(QKeyEvent* keyEvent) const override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
    bool propertyValueToStr(QString& strValue) const override;
};

class QTN_PW_EXPORT QtnPropertyDelegateQStringInvalidBase: public QtnPropertyDelegateQString
{
    Q_DISABLE_COPY(QtnPropertyDelegateQStringInvalidBase)

protected:
    QtnPropertyDelegateQStringInvalidBase(QtnPropertyQStringBase& owner);

    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    void drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip = nullptr) const override;

    virtual bool isPropertyValid() const = 0;

private:
    QColor m_invalidColor;
};

class QTN_PW_EXPORT QtnPropertyDelegateQStringFile: public QtnPropertyDelegateQStringInvalidBase
{
    Q_DISABLE_COPY(QtnPropertyDelegateQStringFile)

public:
    QtnPropertyDelegateQStringFile(QtnPropertyQStringBase& owner);

protected:
    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;

    bool isPropertyValid() const override;

private:
    QtnPropertyDelegateAttributes m_editorAttributes;
};

class QTN_PW_EXPORT QtnPropertyDelegateQStringList: public QtnPropertyDelegateQString
{
    Q_DISABLE_COPY(QtnPropertyDelegateQStringList)

public:
    QtnPropertyDelegateQStringList(QtnPropertyQStringBase& owner);

protected:
    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;

private:
    QStringList m_items;
};

#endif // PROPERTY_DELEGATE_QSTRING_H
