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

#include "PropertyDelegateFactory.h"

namespace Qtinuum
{

PropertyDelegateFactory::PropertyDelegateFactory(const PropertyDelegateFactory* superFactory)
    : m_superFactory(superFactory)
{
}

PropertyDelegate* PropertyDelegateFactory::createDelegate(Property& owner) const
{
    const QMetaObject* metaObject = owner.metaObject();
    while (metaObject)
    {
        // try to find delegate factory by class name
        auto it = m_createItems.find(metaObject->className());

        if (it != m_createItems.end())
        {
            // try to find delegate factory by name
            const CreateItem& createItem = it.value();
            const PropertyDelegateInfo* propertyDelegate = owner.delegate();
            QByteArray delegateName;
            if (propertyDelegate)
                delegateName = propertyDelegate->name;

            CreateFunction* createFunction = 0;

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
                return 0;

            // call factory function
            return (*createFunction)(owner);
        }

        metaObject = metaObject->superClass();
    }

    if (m_superFactory)
        return m_superFactory->createDelegate(owner);

    return 0;
}

bool PropertyDelegateFactory::registerDelegateDefault(const QMetaObject* propertyMetaObject, CreateFunction* createFunction, const QByteArray& delegateName)
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

bool PropertyDelegateFactory::registerDelegate(const QMetaObject* propertyMetaObject, CreateFunction* createFunction, const QByteArray& delegateName)
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

PropertyDelegateFactory& PropertyDelegateFactory::staticInstance()
{
    static PropertyDelegateFactory factory;
    return factory;
}


} // end namespace Qtinuum
