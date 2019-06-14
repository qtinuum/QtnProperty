#ifndef TEXTATTRIBUTES_H
#define TEXTATTRIBUTES_H

#include "QtnProperty/PropertyCore.h"
#include "QtnProperty/PropertyGUI.h"

class QtnPropertySetTextAttributes: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetTextAttributes)

public:
    // constructor declaration
    explicit QtnPropertySetTextAttributes(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetTextAttributes() override;
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
