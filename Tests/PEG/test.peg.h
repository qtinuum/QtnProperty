#ifndef TEST_H
#define TEST_H

#include "QtnProperty/Enum.h"
#include "QtnProperty/PropertyCore.h"
#include "QtnProperty/PropertyGUI.h"

class QtnPropertySetTest1: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetTest1)

public:
    // constructor declaration
    explicit QtnPropertySetTest1(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetTest1() override;
    // assignment declaration
    QtnPropertySetTest1& operator=(const QtnPropertySetTest1& other);
    
    // start children declarations
    QtnPropertyInt& a;
    QtnPropertyQString& text;
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

class QtnPropertySetTest2: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetTest2)

public:
    // constructor declaration
    explicit QtnPropertySetTest2(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetTest2() override;
    // assignment declaration
    QtnPropertySetTest2& operator=(const QtnPropertySetTest2& other);

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

    // declaration
    void aaa();


class QtnPropertySetYY: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetYY)

public:
    // constructor declaration
    explicit QtnPropertySetYY(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetYY() override;
    // assignment declaration
    QtnPropertySetYY& operator=(const QtnPropertySetYY& other);
    
    // start children declarations
    QtnPropertyQRect& rect;
    QtnPropertyQString& s;
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

class QtnPropertySetAA: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetAA)

public:
    // constructor declaration
    explicit QtnPropertySetAA(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetAA() override;
    // assignment declaration
    QtnPropertySetAA& operator=(const QtnPropertySetAA& other);

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

               private:
                   bool m_s;
           
};

class QtnPropertySetSS: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetSS)

public:
    // constructor declaration
    explicit QtnPropertySetSS(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetSS() override;
    // assignment declaration
    QtnPropertySetSS& operator=(const QtnPropertySetSS& other);
    
    // start children declarations
    QtnPropertyBool& a;
    QtnPropertySetAA& aa;
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

class QtnPropertySetTest3: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetTest3)

public:
    // constructor declaration
    explicit QtnPropertySetTest3(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetTest3() override;
    // assignment declaration
    QtnPropertySetTest3& operator=(const QtnPropertySetTest3& other);
    
    // start children declarations
    QtnPropertySetYY& yy;
    QtnPropertySetSS& iis;
    QtnPropertyBool& u;
    QtnPropertySetTest2& xx;
    QtnPropertySetTest2& tt;
    QtnPropertySetSS& s;
    QtnPropertyBool& ww;
    QtnPropertyBoolCallback& bc;
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
    void on_propertyDidChange(QtnPropertyChangeReason reason);
    void on_u_propertyWillChange(QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue, int typeId);
    void on_u_propertyDidChange(QtnPropertyChangeReason reason);
    void on_s_a_propertyValueAccept(QtnPropertyValuePtr valueToAccept, bool* accept);
    void on_s_a_propertyWillChange(QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue, int typeId);
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
    
    static const QtnEnumInfo& info();
    static const unsigned int values_count = 1;
};

class TYPE
{
public:
    
    static const QtnEnumInfo& info();
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
    
    static const QtnEnumInfo& info();
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
    
    static const QtnEnumInfo& info();
    static const unsigned int values_count = 3;
};

class QtnPropertySetAllPropertyTypes: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetAllPropertyTypes)

public:
    // constructor declaration
    explicit QtnPropertySetAllPropertyTypes(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetAllPropertyTypes() override;
    // assignment declaration
    QtnPropertySetAllPropertyTypes& operator=(const QtnPropertySetAllPropertyTypes& other);
    
    // start children declarations
    QtnPropertyBool& bp;
    QtnPropertyBoolCallback& bpc;
    QtnPropertyInt& ip;
    QtnPropertyIntCallback& ipc;
    QtnPropertyUInt& up;
    QtnPropertyUIntCallback& upc;
    QtnPropertyFloat& fp;
    QtnPropertyFloatCallback& fpc;
    QtnPropertyDouble& dp;
    QtnPropertyDoubleCallback& dpc;
    QtnPropertyQString& sp;
    QtnPropertyQStringCallback& spc;
    QtnPropertyQRect& rp;
    QtnPropertyQRectCallback& rpc;
    QtnPropertyQPoint& pp;
    QtnPropertyQPointCallback& ppc;
    QtnPropertyQSize& szp;
    QtnPropertyQSizeCallback& szpc;
    QtnPropertyEnum& ep;
    QtnPropertyEnumCallback& epc;
    QtnPropertyEnumFlags& efp;
    QtnPropertyEnumFlagsCallback& efpc;
    QtnPropertyQColor& cp;
    QtnPropertyQColorCallback& cpc;
    QtnPropertyQFont& fnp;
    QtnPropertyQFontCallback& fnpc;
    QtnPropertyButton& bttn;
    QtnPropertyQPointF& ppf;
    QtnPropertyQPointFCallback& ppfc;
    QtnPropertyQRectF& rpf;
    QtnPropertyQRectFCallback& rpfc;
    QtnPropertyQSizeF& szpf;
    QtnPropertyQSizeFCallback& szpfc;
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

    private:
        bool _b;
        qint32 _i;
        quint32 _ui;
        float _f;
        double _d;
        QString _s;
        QRect _r;
        QRectF _rf;
        QPoint _p;
        QPointF _pf;
        QSize _sz;
        QSizeF _szf;
        QtnEnumValueType _e;
        QtnEnumFlagsValueType _ef;
        QColor _cl;
        QFont _fn;
    
};

class MY_TYPE
{
public:
    enum Enum
    {
        MY_TYPE1 = 1,
        MY_TYPE2 = 2
    };
    
    static const QtnEnumInfo& info();
    static const unsigned int values_count = 2;
};

class QtnPropertySetTest12: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetTest12)

public:
    // constructor declaration
    explicit QtnPropertySetTest12(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetTest12() override;
    // assignment declaration
    QtnPropertySetTest12& operator=(const QtnPropertySetTest12& other);
    
    // start children declarations
    QtnPropertyEnum& p;
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

#endif // TEST_H
