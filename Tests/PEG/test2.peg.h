#ifndef TEST2_H
#define TEST2_H

#include "PropertyCore.h"

class QtnPropertySetA: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetA)

public:
    // constructor declaration
    explicit QtnPropertySetA(QObject* parent = 0);
    // destructor declaration
    virtual ~QtnPropertySetA();
    // assignment declaration
    QtnPropertySetA& operator=(const QtnPropertySetA& other);
    
    // start children declarations
    QtnPropertyBool& b;
    // end children declarations

protected:
    // cloning implementation
    QtnPropertySet* createNewImpl(QObject* parentForNew) const override;
    QtnPropertySet* createCopyImpl(QObject* parentForCopy) const override;
    // copy values implementation
    bool copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask) override;

private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();

        public:
            void setOwner(int a);
    
};

#endif // TEST2_H
