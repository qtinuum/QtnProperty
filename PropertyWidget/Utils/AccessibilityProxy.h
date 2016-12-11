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

#ifndef QTN_ACCESSIBILITY_PROXY_H
#define QTN_ACCESSIBILITY_PROXY_H

#include "../PropertyWidgetAPI.h"
#include "../../Core/PropertySet.h"
#include <QObject>
#include <QRect>

class QtnPropertyView;

class QTN_PW_EXPORT QtnAccessibilityProxy: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QtnAccessibilityProxy)

public:
    explicit QtnAccessibilityProxy(QtnPropertyView* owner = 0);
    ~QtnAccessibilityProxy();

public slots:
    QtnPropertyView* owner();
    QtnPropertyBase* activeProperty();
    QtnPropertySet* propertySet();
    QtnPropertyBase* findProperty(QString nameOrPath);
    QtnPropertyBase* propertyUnderPoint(QPoint point);
    void ensureVisibleProperty(QtnPropertyBase* property);

    QRect propertyNameRect(QtnPropertyBase* property);
    QRect propertyValueRect(QtnPropertyBase* property);
    QRect propertyActionRect(QtnPropertyBase* property, int actionIndex);

    QString propertyDelegateName(QtnPropertyBase* property);

private:
    QtnPropertyView* m_owner;
};

Q_DECLARE_METATYPE(QtnAccessibilityProxy*)


#endif // QTN_ACCESSIBILITY_PROXY_H
