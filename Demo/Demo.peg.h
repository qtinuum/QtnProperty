#ifndef DEMO_H
#define DEMO_H

#include "../Core/PropertyCore.h"
#include "../Core/PropertyGUI.h"

class COLOR
{
public:
    enum Enum
    {
        red = 1,
        blue = 2,
        green = 3
    };
    
    static Qtinuum::EnumInfo& info();
    static const unsigned int values_count = 3;
};

class FLAGS
{
public:
    enum Enum
    {
        opt1 = 1,
        opt2 = 2,
        opt3 = 4
    };
    
    static Qtinuum::EnumInfo& info();
    static const unsigned int values_count = 3;
};

class PropertySetInternal: public Qtinuum::PropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetInternal)

public:
    // constructor declaration
    explicit PropertySetInternal(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetInternal();
    // assignment declaration
    PropertySetInternal& operator=(const PropertySetInternal& other);
    
    // start children declarations
    Qtinuum::PropertyQString &name;
    Qtinuum::PropertyQRect &rect;
    Qtinuum::PropertyQString &item1;
    Qtinuum::PropertyQString &item2;
    Qtinuum::PropertyQString &item3;
    Qtinuum::PropertyQString &item4;
    Qtinuum::PropertyQString &item5;
    Qtinuum::PropertyBool &item6;
    Qtinuum::PropertyBool &item7;
    Qtinuum::PropertyEnum &item8;
    Qtinuum::PropertyQColor &item9;
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
};

class PropertySetSubProperties: public Qtinuum::PropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetSubProperties)

public:
    // constructor declaration
    explicit PropertySetSubProperties(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetSubProperties();
    // assignment declaration
    PropertySetSubProperties& operator=(const PropertySetSubProperties& other);
    
    // start children declarations
    Qtinuum::PropertyInt &intProp;
    Qtinuum::PropertyUInt &uintProp;
    Qtinuum::PropertyDouble &doubleProp;
    Qtinuum::PropertyFloat &floatProp;
    Qtinuum::PropertyBool &boolProp;
    Qtinuum::PropertyEnum &enumProp;
    Qtinuum::PropertyQColor &colorProp;
    Qtinuum::PropertyEnumFlags &flagsProp;
    Qtinuum::PropertyQString &fileName1;
    Qtinuum::PropertyQString &fileName2;
    Qtinuum::PropertyQPoint &pointProp;
    Qtinuum::PropertyQSize &sizeProp;
    Qtinuum::PropertyQFont &fontProp;
    Qtinuum::PropertyQString &listProp;
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
};

class PropertySetMain: public Qtinuum::PropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetMain)

public:
    // constructor declaration
    explicit PropertySetMain(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetMain();
    // assignment declaration
    PropertySetMain& operator=(const PropertySetMain& other);
    
    // start children declarations
    Qtinuum::PropertyInt &count;
    Qtinuum::PropertyBool &isValid;
    PropertySetInternal &Internal;
    Qtinuum::PropertyBool &flag;
    PropertySetSubProperties &Supplementary;
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
    
    // start slot declarations
    void on_isValid_propertyDidChange(const Qtinuum::PropertyBase* changedProperty, const Qtinuum::PropertyBase* firedProperty, Qtinuum::PropertyChangeReason reason);
    // end slot declarations
};

#endif // DEMO_H
