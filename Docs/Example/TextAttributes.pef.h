#ifndef TEXTATTRIBUTES_H
#define TEXTATTRIBUTES_H

#include "Core/PropertyCore.h"

class QtnPropertySetTextAttributes: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetTextAttributes)

public:
    // constructor declaration
    explicit QtnPropertySetTextAttributes(QObject* parent = 0);
    // destructor declaration
    virtual ~QtnPropertySetTextAttributes();
    // assignment declaration
    QtnPropertySetTextAttributes& operator=(const QtnPropertySetTextAttributes& other);
    
    // start children declarations
    QtnPropertyBool& WordWrap;
    QtnPropertyInt& Height;
    QtnPropertyQColor& Color;
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
};

#endif // TEXTATTRIBUTES_H
