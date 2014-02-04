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
    
    static const QtnEnumInfo& info();
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
    
    static const QtnEnumInfo& info();
    static const unsigned int values_count = 3;
};

class QtnPropertySetInternal: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetInternal)

public:
    // constructor declaration
    explicit QtnPropertySetInternal(QObject* parent = 0);
    // destructor declaration
    virtual ~QtnPropertySetInternal();
    // assignment declaration
    QtnPropertySetInternal& operator=(const QtnPropertySetInternal& other);
    
    // start children declarations
    QtnPropertyQString& name;
    QtnPropertyQRect& rect;
    QtnPropertyQString& item1;
    QtnPropertyQString& item2;
    QtnPropertyQString& item3;
    QtnPropertyQString& item4;
    QtnPropertyQString& item5;
    QtnPropertyBool& item6;
    QtnPropertyBool& item7;
    QtnPropertyEnum& item8;
    QtnPropertyQColor& item9;
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

class QtnPropertySetSubProperties: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetSubProperties)

public:
    // constructor declaration
    explicit QtnPropertySetSubProperties(QObject* parent = 0);
    // destructor declaration
    virtual ~QtnPropertySetSubProperties();
    // assignment declaration
    QtnPropertySetSubProperties& operator=(const QtnPropertySetSubProperties& other);
    
    // start children declarations
    QtnPropertyInt& intProp;
    QtnPropertyUInt& uintProp;
    QtnPropertyDouble& doubleProp;
    QtnPropertyFloat& floatProp;
    QtnPropertyBool& boolProp;
    QtnPropertyEnum& enumProp;
    QtnPropertyQColor& colorProp;
    QtnPropertyEnumFlags& flagsProp;
    QtnPropertyQString& fileName1;
    QtnPropertyQString& fileName2;
    QtnPropertyQPoint& pointProp;
    QtnPropertyQSize& sizeProp;
    QtnPropertyQFont& fontProp;
    QtnPropertyQString& listProp;
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

class QtnPropertySetMain: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetMain)

public:
    // constructor declaration
    explicit QtnPropertySetMain(QObject* parent = 0);
    // destructor declaration
    virtual ~QtnPropertySetMain();
    // assignment declaration
    QtnPropertySetMain& operator=(const QtnPropertySetMain& other);
    
    // start children declarations
    QtnPropertyInt& count;
    QtnPropertyBool& isValid;
    QtnPropertySetInternal& Internal;
    QtnPropertyBool& flag;
    QtnPropertySetSubProperties& Supplementary;
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
    
    // start slot declarations
    void on_isValid_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);
    // end slot declarations
};

class QtnPropertySetSubPropertySetType: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetSubPropertySetType)

public:
    // constructor declaration
    explicit QtnPropertySetSubPropertySetType(QObject* parent = 0);
    // destructor declaration
    virtual ~QtnPropertySetSubPropertySetType();
    // assignment declaration
    QtnPropertySetSubPropertySetType& operator=(const QtnPropertySetSubPropertySetType& other);
    
    // start children declarations
    QtnPropertyBool& SwitchProperty;
    QtnPropertyQStringCallback& ReadOnlyString;
    QtnPropertyQString& FileNameProperty;
    QtnPropertyQString& FolderNameProperty;
    QtnPropertyQString& StringFromList;
    QtnPropertyQColor& CircleShapeColor;
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

class QtnPropertySetSamplePS: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetSamplePS)

public:
    // constructor declaration
    explicit QtnPropertySetSamplePS(QObject* parent = 0);
    // destructor declaration
    virtual ~QtnPropertySetSamplePS();
    // assignment declaration
    QtnPropertySetSamplePS& operator=(const QtnPropertySetSamplePS& other);
    
    // start children declarations
    QtnPropertyBool& BoolProperty;
    QtnPropertyDouble& DoubleProperty;
    QtnPropertyFloat& FloatProperty;
    QtnPropertyInt& IntProperty;
    QtnPropertyUInt& UIntProperty;
    QtnPropertyEnum& EnumProperty;
    QtnPropertyEnumFlags& EnumFlagsProperty;
    QtnPropertyQString& QStringValue;
    QtnPropertyQPoint& QPointProperty;
    QtnPropertyQSize& QSizeProperty;
    QtnPropertyQRect& QRectProperty;
    QtnPropertyQColor& QColorProperty;
    QtnPropertyQFont& QFontProperty;
    QtnPropertyBool& EnableSubPropertySet;
    QtnPropertySetSubPropertySetType& SubPropertySet;
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
    
    // start slot declarations
    void on_EnableSubPropertySet_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);
    // end slot declarations
};

#endif // DEMO_H
