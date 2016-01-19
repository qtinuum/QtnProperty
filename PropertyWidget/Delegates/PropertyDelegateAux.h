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

#ifndef QTN_PROPERTY_DELEGATE_AUX_H
#define QTN_PROPERTY_DELEGATE_AUX_H

#include "../PropertyWidgetAPI.h"

#include <functional>
#include <QStylePainter>

class QEvent;
class QtnPropertyView;
struct QtnPropertyDelegateDrawContext;
struct QtnPropertyDelegateEventContext;

struct QTN_PW_EXPORT QtnPropertyDelegateSubItem
{
    QRect rect;

    std::function<void(QtnPropertyDelegateDrawContext&, const QtnPropertyDelegateSubItem&)> drawHandler;
    std::function<bool(QtnPropertyDelegateEventContext&, const QtnPropertyDelegateSubItem&)> eventHandler;
};

struct QTN_PW_EXPORT QtnPropertyDelegateDrawContext
{
public:
    QStylePainter* painter;
    const QtnPropertyView* widget;

    const QRect rect;
    QMargins margins;
    const int splitPos;

    const bool isActive;
    const bool hasChildren;

    QStyle* style() const;
    void initStyleOption(QStyleOption& option) const;
    const QPalette& palette() const;
    QPalette::ColorGroup colorGroup() const;
};

struct QTN_PW_EXPORT QtnPropertyDelegateEventContext
{
public:
    QEvent* event;
    const QtnPropertyView* widget;
};

#endif // QTN_PROPERTY_DELEGATE_AUX_H
