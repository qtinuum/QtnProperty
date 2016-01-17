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

#include "PropertyDelegateFactory.h"
#include <QDebug>

QtnPropertyDelegateFactory::QtnPropertyDelegateFactory(const QtnPropertyDelegateFactory* superFactory)
    : m_superFactory(superFactory)
{
}

QtnPropertyDelegate* QtnPropertyDelegateFactory::createDelegate(QtnProperty& owner) const
{
    const QMetaObject* metaObject = owner.metaObject();
    while (metaObject)
    {
        // try to find delegate factory by class name
        auto it = m_createItems.find(metaObject->className());

        if (it != m_createItems.end())
        {
            // try to find delegate factory by delegate name
            const CreateItem& createItem = it.value();
            const QtnPropertyDelegateInfo* propertyDelegate = owner.delegate();
            QByteArray delegateName;
            if (propertyDelegate)
                delegateName = propertyDelegate->name;

            CreateFunction* createFunction = nullptr;

            if (delegateName.isEmpty())
            {
                createFunction = createItem.defaultCreateFunction;
            }
            else
            {
                auto jt = createItem.createFunctions.find(delegateName);
                Q_ASSERT(jt != createItem.createFunctions.end());
                if (jt != createItem.createFunctions.end())
                    createFunction = jt.value();
            }

            if (!createFunction)
                return nullptr;

            // call factory function
            return (*createFunction)(owner);
        }

        metaObject = metaObject->superClass();
    }

    if (m_superFactory)
        return m_superFactory->createDelegate(owner);

    return nullptr;
}

bool QtnPropertyDelegateFactory::registerDelegateDefault(const QMetaObject* propertyMetaObject, CreateFunction* createFunction, const QByteArray& delegateName)
{
    Q_ASSERT(propertyMetaObject);
    Q_ASSERT(createFunction);

    // find or create creation record
    CreateItem& createItem = m_createItems[propertyMetaObject->className()];
    // register default create function
    createItem.defaultCreateFunction = createFunction;

    if (!delegateName.isEmpty())
        return registerDelegate(propertyMetaObject, createFunction, delegateName);

    return true;
}

bool QtnPropertyDelegateFactory::registerDelegate(const QMetaObject* propertyMetaObject, CreateFunction* createFunction, const QByteArray& delegateName)
{
    Q_ASSERT(propertyMetaObject);
    Q_ASSERT(createFunction);
    Q_ASSERT(!delegateName.isEmpty());

    // find or create creation record
    CreateItem& createItem = m_createItems[propertyMetaObject->className()];
    // register create function
    createItem.createFunctions[delegateName] = createFunction;

    return true;
}


QtnPropertyDelegateFactory& QtnPropertyDelegateFactory::staticInstance()
{
    static QtnPropertyDelegateFactory factory;
    return factory;
}

void regBoolDelegates();
void regDoubleDelegates();
void regEnumDelegates();
void regEnumFlagsDelegates();
void regFloatDelegates();
void regIntDelegates();
void regQPointDelegates();
void regQRectDelegates();
void regQSizeDelegates();
void regQStringDelegates();
void regUIntDelegates();
void regQColorDelegates();
void regQFontDelegates();

bool initQtnPropertyWidget()
{
    static bool initialized = false;

    if (initialized)
        return false;

    regBoolDelegates();
    regDoubleDelegates();
    regEnumDelegates();
    regEnumFlagsDelegates();
    regFloatDelegates();
    regIntDelegates();
    regQPointDelegates();
    regQRectDelegates();
    regQSizeDelegates();
    regQStringDelegates();
    regUIntDelegates();
    regQColorDelegates();
    regQFontDelegates();

    initialized = true;
    return true;
}

bool initializeQtnPropertyWidget = initQtnPropertyWidget();

