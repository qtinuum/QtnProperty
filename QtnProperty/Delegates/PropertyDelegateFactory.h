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

#ifndef QTN_PROPERTY_DELEGATE_FACTORY_H
#define QTN_PROPERTY_DELEGATE_FACTORY_H

#include "PropertyDelegate.h"
#include <QMap>

class QTN_PW_EXPORT QtnPropertyDelegateFactory
{
    Q_DISABLE_COPY(QtnPropertyDelegateFactory)

public:
    typedef QtnPropertyDelegate *CreateFunction(QtnPropertyBase&);

    explicit QtnPropertyDelegateFactory(const QtnPropertyDelegateFactory* superFactory = nullptr);

    QtnPropertyDelegate* createDelegate(QtnPropertyBase& owner) const;

    bool registerDelegateDefault(const QMetaObject* propertyMetaObject, CreateFunction* createFunction, const QByteArray& delegateName = "");
    bool registerDelegate(const QMetaObject* propertyMetaObject, CreateFunction* createFunction, const QByteArray& delegateName);

    static QtnPropertyDelegateFactory& staticInstance();

private:
    const QtnPropertyDelegateFactory* m_superFactory;

    struct CreateItem
    {
        CreateItem()
            : defaultCreateFunction(0)
        {
        }

        CreateFunction* defaultCreateFunction;
        QMap<QByteArray, CreateFunction*> createFunctions;
    };

    QMap<QByteArray, CreateItem> m_createItems;
};

template <typename PropertyDelegateClass, typename PropertyClass>
QtnPropertyDelegate* qtnCreateDelegate(QtnPropertyBase& owner)
{
    PropertyClass* theOwner = qobject_cast<PropertyClass*>(&owner);
    if (!theOwner)
        return nullptr;

    return new PropertyDelegateClass(*theOwner);
}

#endif // QTN_PROPERTY_DELEGATE_FACTORY_H
