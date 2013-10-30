#include "test2.peg.h"


PropertySetA::PropertySetA(QObject *parent)
    : Qtinuum::PropertySet(parent)
    , b(*new Qtinuum::PropertyBool(this))
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetA::~PropertySetA()
{
    disconnectSlots();
}

PropertySetA& PropertySetA::operator=(const PropertySetA& other)
{
    b = other.b;

    return *this;
}

Qtinuum::PropertySet* PropertySetA::createNewImpl(QObject* parentForNew) const
{
    return new PropertySetA(parentForNew);
}

Qtinuum::PropertySet* PropertySetA::createCopyImpl(QObject* parentForCopy) const
{
    PropertySetA* p = new PropertySetA(parentForCopy);
    *p = *this;
    return p;
}

bool PropertySetA::copyValuesImpl(Qtinuum::PropertySet* propertySetCopyFrom, Qtinuum::PropertyState ignoreMask)
{
    PropertySetA* theCopyFrom = qobject_cast<PropertySetA*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->b.state() & ignoreMask))
    {
        b = theCopyFrom->b;
    }

    return true;
}

void PropertySetA::init()
{
    static QString A_name = tr("A");
    setName(A_name);
    
    // start children initialization
    static QString b_name = tr("b");
    b.setName(b_name);
    static QString b_description = "b property";
    b.setDescription(b_description);
    b.setValue(true);
    // end children initialization
}

void PropertySetA::connectSlots()
{
}

void PropertySetA::disconnectSlots()
{
}

void PropertySetA::connectDelegates()
{
    b.setDelegateCallback([] () -> const Qtinuum::PropertyDelegateInfo * {
        QScopedPointer<Qtinuum::PropertyDelegateInfo> info(new Qtinuum::PropertyDelegateInfo());
        info->name = "Combobox";
        info->attributes["labelFalse"] = "Off";
        info->attributes["labelTrue"] = "On";
        return info.take();
    });
}

        void PropertySetA::setOwner(int a)
        {
            b = (bool)a;
        }
    
