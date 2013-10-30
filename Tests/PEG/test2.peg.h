#ifndef TEST2_H
#define TEST2_H

#include "PropertyCore.h"

class PropertySetA: public Qtinuum::PropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetA)

public:
    // constructor declaration
    explicit PropertySetA(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetA();
    // assignment declaration
    PropertySetA& operator=(const PropertySetA& other);
    
    // start children declarations
    Qtinuum::PropertyBool &b;
    // end children declarations

protected:
    // cloning implementation
    Qtinuum::PropertySet* createNewImpl(QObject* parentForNew) const override;
    Qtinuum::PropertySet* createCopyImpl(QObject* parentForCopy) const override;
    // copy values implementation
    bool copyValuesImpl(Qtinuum::PropertySet* propertySetCopyFrom, Qtinuum::PropertyState ignoreMask) override;

private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();

        public:
            void setOwner(int a);
    
};

#endif // TEST2_H
