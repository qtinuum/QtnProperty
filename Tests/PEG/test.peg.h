#ifndef TEST_H
#define TEST_H

#include "Enum.h"
#include "PropertyCore.h"
#include "PropertyGUI.h"

class PropertySetTest1: public Qtinuum::Property
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetTest1)

public:
    // constructor declaration
    explicit PropertySetTest1(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetTest1();
    
    // start children declarations
    Qtinuum::PropertyInt &a;
    Qtinuum::PropertyQString &text;
    // end children declarations
private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();
};

class PropertySetTest2: public Qtinuum::Property
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetTest2)

public:
    // constructor declaration
    explicit PropertySetTest2(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetTest2();
private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();
};

    // declaration
    void aaa();


class PropertySetYY: public Qtinuum::Property
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetYY)

public:
    // constructor declaration
    explicit PropertySetYY(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetYY();
    
    // start children declarations
    Qtinuum::PropertyQRect &rect;
    // end children declarations
private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();
};

class PropertySetAA: public Qtinuum::Property
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetAA)

public:
    // constructor declaration
    explicit PropertySetAA(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetAA();
private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();

               private:
                   bool m_s;
           
};

class PropertySetSS: public qtn::Property
    , private QRect
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetSS)

public:
    // constructor declaration
    explicit PropertySetSS(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetSS();
    
    // start children declarations
    Qtinuum::PropertyBool &a;
    PropertySetAA &aa;
    // end children declarations
private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();
};

class PropertySetTest3: public qtn::Property
    , private QString
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetTest3)

public:
    // constructor declaration
    explicit PropertySetTest3(QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetTest3();
    
    // start children declarations
    PropertySetYY &yy;
    PropertySetSS &iis;
    Qtinuum::PropertyBool &u;
    PropertySetTest2 &xx;
    PropertySetTest2 &tt;
    PropertySetSS &s;
    Qtinuum::PropertyBool &ww;
    Qtinuum::PropertyBoolCallback &bc;
    // end children declarations
private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();
    
    // start slot declarations
    void on_propertyDidChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason);
    void on_u_propertyWillChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason, Qtinuum::PropertyValuePtr newValue);
    void on_u_propertyDidChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason);
    void on_s_a_propertyValueAccept(const Qtinuum::Property *property, Qtinuum::PropertyValuePtr valueToAccept, bool* accept);
    void on_s_propertyWillChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason, Qtinuum::PropertyValuePtr newValue);
    // end slot declarations

        public:
            bool m_s;
    
};

class LANGUAGE
{
public:
    enum Enum
    {
        ENG = 3
    };
    
    static Qtinuum::EnumInfo &info();
    static const unsigned int values_count = 1;
};

class TYPE
{
public:
    
    static Qtinuum::EnumInfo &info();
    static const unsigned int values_count = 0;
};

class COLOR
{
public:
    enum Enum
    {
        RED = 10,
        BLUE = 22,
        YELLOW = 1
    };
    
    static Qtinuum::EnumInfo &info();
    static const unsigned int values_count = 3;
};

class MASK
{
public:
    enum Enum
    {
        ONE = 1,
        TWO = 2,
        FOUR = 4
    };
    
    static Qtinuum::EnumInfo &info();
    static const unsigned int values_count = 3;
};

class PropertySetAllPropertyTypes: public Qtinuum::Property
{
    Q_OBJECT
    //Q_DISABLE_COPY(PropertySetAllPropertyTypes)

public:
    // constructor declaration
    explicit PropertySetAllPropertyTypes(int a, const bool s, std::function<void()> f, QObject *parent = 0);
    // destructor declaration
    virtual ~PropertySetAllPropertyTypes();
    
    // start children declarations
    Qtinuum::PropertyBool &bp;
    Qtinuum::PropertyBoolCallback &bpc;
    Qtinuum::PropertyInt &ip;
    Qtinuum::PropertyIntCallback &ipc;
    Qtinuum::PropertyUInt &up;
    Qtinuum::PropertyUIntCallback &upc;
    Qtinuum::PropertyFloat &fp;
    Qtinuum::PropertyFloatCallback &fpc;
    Qtinuum::PropertyDouble &dp;
    Qtinuum::PropertyDoubleCallback &dpc;
    Qtinuum::PropertyQString &sp;
    Qtinuum::PropertyQStringCallback &spc;
    Qtinuum::PropertyQRect &rp;
    Qtinuum::PropertyQRectCallback &rpc;
    Qtinuum::PropertyQPoint &pp;
    Qtinuum::PropertyQPointCallback &ppc;
    Qtinuum::PropertyQSize &szp;
    Qtinuum::PropertyQSizeCallback &szpc;
    Qtinuum::PropertyEnum &ep;
    Qtinuum::PropertyEnumCallback &epc;
    Qtinuum::PropertyEnumFlags &efp;
    Qtinuum::PropertyEnumFlagsCallback &efpc;
    Qtinuum::PropertyQColor &cp;
    Qtinuum::PropertyQColorCallback &cpc;
    Qtinuum::PropertyQFont &fnp;
    Qtinuum::PropertyQFontCallback &fnpc;
    // end children declarations
private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();

    private:
        bool _b;
        qint32 _i;
        quint32 _ui;
        float _f;
        double _d;
        QString _s;
        QRect _r;
        QPoint _p;
        QSize _sz;
        qtn::EnumValueType _e;
        qtn::EnumFlagsValueType _ef;
        QColor _cl;
        QFont _fn;
    
};

#endif // TEST_H
