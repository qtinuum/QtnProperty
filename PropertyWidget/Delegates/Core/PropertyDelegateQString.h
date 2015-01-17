/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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
