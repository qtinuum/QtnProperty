#ifndef TEST2_H
#define TEST2_H

#include "PropertyCore.h"

class PropertySetA: public Qtinuum::Property
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetA)

public:
    // constructor declaration
    explicit PropertySetA(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetA();
    
    // start children declarations
    Qtinuum::PropertyBool &b;
    // end children declarations
private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();

        public:
            void setOwner(int a);
    
};

#endif // TEST2_H
