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

#ifndef PROPERTY_DELEGATE_BOOL_H
#define PROPERTY_DELEGATE_BOOL_H

#include "../PropertyDelegate.h"

class QtnPropertyBoolBase;

class QTN_PW_EXPORT QtnPropertyDelegateBoolCheck: public QtnPropertyDelegateTyped<QtnPropertyBoolBase>
{
    Q_DISABLE_COPY(QtnPropertyDelegateBoolCheck)

public:
    QtnPropertyDelegateBoolCheck(QtnPropertyBoolBase& owner)
        : QtnPropertyDelegateTyped<QtnPropertyBoolBase>(owner)
    {
    }

protected:
    void drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip = nullptr) const override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
};

class QTN_PW_EXPORT QtnPropertyDelegateBoolCombobox: public QtnPropertyDelegateTyped<QtnPropertyBoolBase>
{
    Q_DISABLE_COPY(QtnPropertyDelegateBoolCombobox)

public:
    QtnPropertyDelegateBoolCombobox(QtnPropertyBoolBase& owner);

protected:
    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
    bool propertyValueToStr(QString& strValue) const override;

private:
    QString m_labels[2];
};

#endif // PROPERTY_DELEGATE_BOOL_H
