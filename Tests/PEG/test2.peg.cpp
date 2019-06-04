#include "test2.peg.h"


QtnPropertySetA::QtnPropertySetA(QObject* parent)
    : QtnPropertySet(parent)
    , b(*qtnCreateProperty<QtnPropertyBool>(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetA::~QtnPropertySetA()
{
    disconnectSlots();
}

QtnPropertySetA& QtnPropertySetA::operator=(const QtnPropertySetA& other)
{
    Q_UNUSED(other);

    b = other.b;

    return *this;
}

QtnPropertySet* QtnPropertySetA::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetA(parentForNew);
}

QtnPropertySet* QtnPropertySetA::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetA* p = new QtnPropertySetA(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetA::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetA* theCopyFrom = qobject_cast<QtnPropertySetA*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->b.state() & ignoreMask))
    {
        b = theCopyFrom->b;
    }

    return true;
}

void QtnPropertySetA::init()
{
    static QString A_name = tr("A");
    setName(A_name);
    setId(50);
    
    // start children initialization
    static QString b_name = tr("b");
    b.setName(b_name);
    static QString b_description = "b property";
    b.setDescription(b_description);
    b.setId(51);
    b.setValue(true);
    // end children initialization
}

void QtnPropertySetA::connectSlots()
{
}

void QtnPropertySetA::disconnectSlots()
{
}

void QtnPropertySetA::connectDelegates()
{
    b.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "Combobox";
        info->attributes["labelFalse"] = "Off";
        info->attributes["labelTrue"] = "On";
        return info.take();
    });
}

        void QtnPropertySetA::setOwner(int a)
        {
            b = (bool)a;
        }
    
