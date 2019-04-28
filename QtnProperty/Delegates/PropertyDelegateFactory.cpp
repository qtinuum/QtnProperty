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

#include "PropertyDelegateFactory.h"
#include <QDebug>

void regPropertySetDelegates(QtnPropertyDelegateFactory& factory);
QtnPropertyDelegate* qtnCreateDelegateError(QtnPropertyBase& owner, QString error);

QtnPropertyDelegateFactory::QtnPropertyDelegateFactory(const QtnPropertyDelegateFactory* superFactory)
    : m_superFactory(superFactory)
{
    // default delegate for property set
    regPropertySetDelegates(*this);
}

QtnPropertyDelegate* QtnPropertyDelegateFactory::createDelegate(QtnPropertyBase &owner) const
{
    CreateFunction* createFunction = nullptr;

    QByteArray delegateName;
    if (auto propertyDelegate = owner.delegate())
        delegateName = propertyDelegate->name;

    const QMetaObject* metaObject = owner.metaObject();
    while (metaObject && !createFunction)
    {
        // try to find delegate factory by class name
        auto it = m_createItems.find(metaObject->className());

        if (it != m_createItems.end())
        {
            // try to find delegate factory by delegate name
            const CreateItem& createItem = it.value();

            if (delegateName.isEmpty())
            {
                createFunction = createItem.defaultCreateFunction;
            }
            else
            {
                auto jt = createItem.createFunctions.find(delegateName);
                //Q_ASSERT(jt != createItem.createFunctions.end());
                if (jt != createItem.createFunctions.end())
                    createFunction = jt.value();
            }
        }

        metaObject = metaObject->superClass();
    }

    if (createFunction)
    {
        // call factory function
        return (*createFunction)(owner);
    }

    if (m_superFactory)
        return m_superFactory->createDelegate(owner);

    // create delegate stub
    if (delegateName.isEmpty())
    {
        qWarning() << "Cannot find default delegate for property" << owner.name();
        qWarning() << "Did you forget to register default delegate for " << owner.metaObject()->className() << "type?";
    }
    else
    {
        qWarning() << "Cannot find delegate with name" << delegateName << "for property" << owner.name();
        qWarning() << "Did you forget to register" << delegateName << "delegate for" << owner.metaObject()->className() << "type?";
    }

    return qtnCreateDelegateError(owner, QString("Delegate <%1> unknown").arg(QString::fromLatin1(delegateName)));
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

//extern QIcon qtnResetIcon;
void regBoolDelegates(QtnPropertyDelegateFactory& factory);
void regDoubleDelegates(QtnPropertyDelegateFactory& factory);
void regEnumDelegates(QtnPropertyDelegateFactory& factory);
void regEnumFlagsDelegates(QtnPropertyDelegateFactory& factory);
void regFloatDelegates(QtnPropertyDelegateFactory& factory);
void regIntDelegates(QtnPropertyDelegateFactory& factory);
void regQPointDelegates(QtnPropertyDelegateFactory& factory);
void regQPointFDelegates(QtnPropertyDelegateFactory& factory);
void regGeoPointDelegates(QtnPropertyDelegateFactory& factory);
void regGeoCoordDelegates(QtnPropertyDelegateFactory& factory);
void regQRectDelegates(QtnPropertyDelegateFactory& factory);
void regQRectFDelegates(QtnPropertyDelegateFactory& factory);
void regQSizeDelegates(QtnPropertyDelegateFactory& factory);
void regQSizeFDelegates(QtnPropertyDelegateFactory& factory);
void regQStringDelegates(QtnPropertyDelegateFactory& factory);
void regUIntDelegates(QtnPropertyDelegateFactory& factory);
void regQColorDelegates(QtnPropertyDelegateFactory& factory);
void regQFontDelegates(QtnPropertyDelegateFactory& factory);
void regButtonDelegates(QtnPropertyDelegateFactory& factory);
void regQPenStyleDelegates(QtnPropertyDelegateFactory& factory);
void regQPenDelegates(QtnPropertyDelegateFactory& factory);
void regQBrushStyleDelegates(QtnPropertyDelegateFactory& factory);

class QtnPropertyDelegateFactoryDefault : public QtnPropertyDelegateFactory
{
public:
    QtnPropertyDelegateFactoryDefault()
    {
        regBoolDelegates(*this);
        regDoubleDelegates(*this);
        regEnumDelegates(*this);
        regEnumFlagsDelegates(*this);
        regFloatDelegates(*this);
        regIntDelegates(*this);
        regQPointDelegates(*this);
        regQPointFDelegates(*this);
        regGeoPointDelegates(*this);
        regGeoCoordDelegates(*this);
        regQRectDelegates(*this);
        regQRectFDelegates(*this);
        regQSizeDelegates(*this);
        regQSizeFDelegates(*this);
        regQStringDelegates(*this);
        regUIntDelegates(*this);
        regQColorDelegates(*this);
        regQFontDelegates(*this);
        regButtonDelegates(*this);
        regQPenStyleDelegates(*this);
        regQPenDelegates(*this);
        regQBrushStyleDelegates(*this);
    }
};

QtnPropertyDelegateFactory& QtnPropertyDelegateFactory::staticInstance()
{
    static QtnPropertyDelegateFactoryDefault factory;
    return factory;
}
