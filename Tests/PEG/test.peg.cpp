#include "test.peg.h"

#include <vector>

QtnPropertySetTest1::QtnPropertySetTest1(QObject* parent)
    : QtnPropertySet(parent)
    , a(*qtnCreateProperty<QtnPropertyInt>(this))
    , text(*qtnCreateProperty<QtnPropertyQString>(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetTest1::~QtnPropertySetTest1()
{
    disconnectSlots();
}

QtnPropertySetTest1& QtnPropertySetTest1::operator=(const QtnPropertySetTest1& other)
{
    Q_UNUSED(other);

    a = other.a;
    text = other.text;

    return *this;
}

QtnPropertySet* QtnPropertySetTest1::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetTest1(parentForNew);
}

QtnPropertySet* QtnPropertySetTest1::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetTest1* p = new QtnPropertySetTest1(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetTest1::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetTest1* theCopyFrom = qobject_cast<QtnPropertySetTest1*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->a.state() & ignoreMask))
    {
        a = theCopyFrom->a;
    }

    if (!(theCopyFrom->text.state() & ignoreMask))
    {
        text = theCopyFrom->text;
    }

    return true;
}

void QtnPropertySetTest1::init()
{
    static QString Test1_name = tr("Test1");
    setName(Test1_name);
    static QString description = "Test property_set description";
    setDescription(description);
    setId(1);
    setState(0);
    
    // start children initialization
    static QString a_name = tr("a");
    a.setName(a_name);
    static QString a_description = "Descripion";
    a.setDescription(a_description);
    a.setId(2);
    a.setMaxValue(10);
    a.setStepValue(-1);
    a.setValue(5);
    static QString text_name = tr("text");
    text.setName(text_name);
    static QString text_description = "defrf\"sde\"""deerf3rf"
    "derf r g\r\nreg r{}""dfrgerg"
    "fwrewre";
    text.setDescription(text_description);
    text.setId(3);
    text.setValue(QString("#^{};"));
    // end children initialization
}

void QtnPropertySetTest1::connectSlots()
{
}

void QtnPropertySetTest1::disconnectSlots()
{
}

void QtnPropertySetTest1::connectDelegates()
{
}

QtnPropertySetTest2::QtnPropertySetTest2(QObject* parent)
    : QtnPropertySet(parent)
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetTest2::~QtnPropertySetTest2()
{
    disconnectSlots();
}

QtnPropertySetTest2& QtnPropertySetTest2::operator=(const QtnPropertySetTest2& other)
{
    Q_UNUSED(other);


    return *this;
}

QtnPropertySet* QtnPropertySetTest2::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetTest2(parentForNew);
}

QtnPropertySet* QtnPropertySetTest2::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetTest2* p = new QtnPropertySetTest2(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetTest2::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetTest2* theCopyFrom = qobject_cast<QtnPropertySetTest2*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    return true;
}

void QtnPropertySetTest2::init()
{
    static QString Test2_name = tr("Test2");
    setName(Test2_name);
    setId(4);
}

void QtnPropertySetTest2::connectSlots()
{
}

void QtnPropertySetTest2::disconnectSlots()
{
}

void QtnPropertySetTest2::connectDelegates()
{
}

QtnPropertySetYY::QtnPropertySetYY(QObject* parent)
    : QtnPropertySet(parent)
    , rect(*qtnCreateProperty<QtnPropertyQRect>(this))
    , s(*qtnCreateProperty<QtnPropertyQString>(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetYY::~QtnPropertySetYY()
{
    disconnectSlots();
}

QtnPropertySetYY& QtnPropertySetYY::operator=(const QtnPropertySetYY& other)
{
    Q_UNUSED(other);

    rect = other.rect;
    s = other.s;

    return *this;
}

QtnPropertySet* QtnPropertySetYY::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetYY(parentForNew);
}

QtnPropertySet* QtnPropertySetYY::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetYY* p = new QtnPropertySetYY(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetYY::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetYY* theCopyFrom = qobject_cast<QtnPropertySetYY*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->rect.state() & ignoreMask))
    {
        rect = theCopyFrom->rect;
    }

    if (!(theCopyFrom->s.state() & ignoreMask))
    {
        s = theCopyFrom->s;
    }

    return true;
}

void QtnPropertySetYY::init()
{
    static QString yy_name = tr("yy");
    setName(yy_name);
    static QString description = tr("ss")+tr("ss");
    setDescription(description);
    setId(6);
    
    // start children initialization
    static QString rect_name = tr("rect");
    rect.setName(rect_name);
    rect.setValue(QRect(10, 10, 10, 10));
    static QString s_name = tr("s");
    s.setName(s_name);
    // end children initialization
}

void QtnPropertySetYY::connectSlots()
{
}

void QtnPropertySetYY::disconnectSlots()
{
}

void QtnPropertySetYY::connectDelegates()
{
}

QtnPropertySetAA::QtnPropertySetAA(QObject* parent)
    : QtnPropertySet(parent)
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetAA::~QtnPropertySetAA()
{
    disconnectSlots();
}

QtnPropertySetAA& QtnPropertySetAA::operator=(const QtnPropertySetAA& other)
{
    Q_UNUSED(other);


    return *this;
}

QtnPropertySet* QtnPropertySetAA::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetAA(parentForNew);
}

QtnPropertySet* QtnPropertySetAA::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetAA* p = new QtnPropertySetAA(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetAA::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetAA* theCopyFrom = qobject_cast<QtnPropertySetAA*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    return true;
}

void QtnPropertySetAA::init()
{
    static QString aa_name = tr("aa");
    setName(aa_name);
    setId(9);
}

void QtnPropertySetAA::connectSlots()
{
}

void QtnPropertySetAA::disconnectSlots()
{
}

void QtnPropertySetAA::connectDelegates()
{
}

               // AA cpp code
           

QtnPropertySetSS::QtnPropertySetSS(QObject* parent)
    : QtnPropertySet(parent)
    , a(*qtnCreateProperty<QtnPropertyBool>(this))
    , aa(*qtnCreateProperty<QtnPropertySetAA>(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetSS::~QtnPropertySetSS()
{
    disconnectSlots();
}

QtnPropertySetSS& QtnPropertySetSS::operator=(const QtnPropertySetSS& other)
{
    Q_UNUSED(other);

    a = other.a;
    aa = other.aa;

    return *this;
}

QtnPropertySet* QtnPropertySetSS::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetSS(parentForNew);
}

QtnPropertySet* QtnPropertySetSS::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetSS* p = new QtnPropertySetSS(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetSS::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetSS* theCopyFrom = qobject_cast<QtnPropertySetSS*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->a.state() & ignoreMask))
    {
        a = theCopyFrom->a;
    }

    aa.copyValues(&theCopyFrom->aa, ignoreMask);

    return true;
}

void QtnPropertySetSS::init()
{
    static QString iis_name = tr("iis");
    setName(iis_name);
    setId(7);
    
    // start children initialization
    static QString a_name = tr("a");
    a.setName(a_name);
    a.setId(8);
    a.setValue(true);
    static QString aa_name = tr("aa");
    aa.setName(aa_name);
    aa.setId(9);
    // end children initialization
}

void QtnPropertySetSS::connectSlots()
{
}

void QtnPropertySetSS::disconnectSlots()
{
}

void QtnPropertySetSS::connectDelegates()
{
}

QtnPropertySetTest3::QtnPropertySetTest3(QObject* parent)
    : QtnPropertySet(parent)
    , yy(*qtnCreateProperty<QtnPropertySetYY>(this))
    , iis(*qtnCreateProperty<QtnPropertySetSS>(this))
    , u(*qtnCreateProperty<QtnPropertyBool>(this))
    , xx(*qtnCreateProperty<QtnPropertySetTest2>(this))
    , tt(*qtnCreateProperty<QtnPropertySetTest2>(this))
    , s(*qtnCreateProperty<QtnPropertySetSS>(this))
    , ww(*qtnCreateProperty<QtnPropertyBool>(this))
    , bc(*qtnCreateProperty<QtnPropertyBoolCallback>(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetTest3::~QtnPropertySetTest3()
{
    disconnectSlots();
}

QtnPropertySetTest3& QtnPropertySetTest3::operator=(const QtnPropertySetTest3& other)
{
    Q_UNUSED(other);

    yy = other.yy;
    iis = other.iis;
    u = other.u;
    xx = other.xx;
    tt = other.tt;
    s = other.s;
    ww = other.ww;
    bc = other.bc;

    return *this;
}

QtnPropertySet* QtnPropertySetTest3::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetTest3(parentForNew);
}

QtnPropertySet* QtnPropertySetTest3::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetTest3* p = new QtnPropertySetTest3(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetTest3::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetTest3* theCopyFrom = qobject_cast<QtnPropertySetTest3*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    yy.copyValues(&theCopyFrom->yy, ignoreMask);

    iis.copyValues(&theCopyFrom->iis, ignoreMask);

    if (!(theCopyFrom->u.state() & ignoreMask))
    {
        u = theCopyFrom->u;
    }

    xx.copyValues(&theCopyFrom->xx, ignoreMask);

    tt.copyValues(&theCopyFrom->tt, ignoreMask);

    s.copyValues(&theCopyFrom->s, ignoreMask);

    if (!(theCopyFrom->ww.state() & ignoreMask))
    {
        ww = theCopyFrom->ww;
    }

    if (!(theCopyFrom->bc.state() & ignoreMask))
    {
        bc = theCopyFrom->bc;
    }

    return true;
}

void QtnPropertySetTest3::init()
{
    static QString Test3_name = tr("Test3");
    setName(Test3_name);
    setId(5);
    
    // start children initialization
    static QString yy_name = tr("yy");
    yy.setName(yy_name);
    static QString yy_description = tr("ss")+tr("ss");
    yy.setDescription(yy_description);
    yy.setId(6);
    static QString iis_name = tr("iis");
    iis.setName(iis_name);
    iis.setId(7);
    static QString u_name = tr("u");
    u.setName(u_name);
    u.setId(10);
    u.setValue(true);
    static QString xx_name = tr("xx");
    xx.setName(xx_name);
    static QString tt_name = tr("tt");
    tt.setName(tt_name);
    static QString s_name = tr("s");
    s.setName(s_name);
    s.a.setValue(false);
    static QString ww_name = tr("ww");
    ww.setName(ww_name);
    ww.setId(11);
    static QString bc_name = tr("bc");
    bc.setName(bc_name);
    bc.setCallbackValueAccepted([](bool value)->bool {
            if (value) {
                return true;
            } else {
                return false;
            }
        });
    bc.setCallbackValueEqual([](bool)->bool { return false; });
    bc.setCallbackValueGet([this]()->bool {
                return ww;
        });
    bc.setCallbackValueSet([this](bool value) {
            m_s = value;
        });
    bc.setId(12);
    // end children initialization
}

void QtnPropertySetTest3::connectSlots()
{
    QObject::connect(this, &QtnProperty::propertyDidChange, this, &QtnPropertySetTest3::on_propertyDidChange);
    QObject::connect(&u, &QtnProperty::propertyWillChange, this, &QtnPropertySetTest3::on_u_propertyWillChange);
    QObject::connect(&u, &QtnProperty::propertyDidChange, this, &QtnPropertySetTest3::on_u_propertyDidChange);
    QObject::connect(&s.a, &QtnProperty::propertyValueAccept, this, &QtnPropertySetTest3::on_s_a_propertyValueAccept);
    QObject::connect(&s, &QtnProperty::propertyWillChange, this, &QtnPropertySetTest3::on_s_propertyWillChange);
}

void QtnPropertySetTest3::disconnectSlots()
{
    QObject::disconnect(this, &QtnProperty::propertyDidChange, this, &QtnPropertySetTest3::on_propertyDidChange);
    QObject::disconnect(&u, &QtnProperty::propertyWillChange, this, &QtnPropertySetTest3::on_u_propertyWillChange);
    QObject::disconnect(&u, &QtnProperty::propertyDidChange, this, &QtnPropertySetTest3::on_u_propertyDidChange);
    QObject::disconnect(&s.a, &QtnProperty::propertyValueAccept, this, &QtnPropertySetTest3::on_s_a_propertyValueAccept);
    QObject::disconnect(&s, &QtnProperty::propertyWillChange, this, &QtnPropertySetTest3::on_s_propertyWillChange);
}

void QtnPropertySetTest3::on_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
    Q_UNUSED(changedProperty); Q_UNUSED(firedProperty); Q_UNUSED(reason);
    
        // this propertyDidChange
    
}

void QtnPropertySetTest3::on_u_propertyWillChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue)
{
    Q_UNUSED(changedProperty); Q_UNUSED(firedProperty); Q_UNUSED(reason); Q_UNUSED(newValue);
    
        // u.propertyWillChange code
    
}

void QtnPropertySetTest3::on_u_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
    Q_UNUSED(changedProperty); Q_UNUSED(firedProperty); Q_UNUSED(reason);
    
            // sub u propertyDidChange
        
}

void QtnPropertySetTest3::on_s_a_propertyValueAccept(const QtnProperty *property, QtnPropertyValuePtr valueToAccept, bool* accept)
{
    Q_UNUSED(property); Q_UNUSED(valueToAccept); Q_UNUSED(accept);
    
            // s.a.propertyValueAccept
        
}

void QtnPropertySetTest3::on_s_propertyWillChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue)
{
    Q_UNUSED(changedProperty); Q_UNUSED(firedProperty); Q_UNUSED(reason); Q_UNUSED(newValue);
    
            yy.rect = QRect(1, 1, 1, 1);
            // ss propertyWillChange
        
}

void QtnPropertySetTest3::connectDelegates()
{
}

        void aaa()
        {

        }
    
static QtnEnumInfo& create_LANGUAGE_info()
{
    QVector<QtnEnumValueInfo> staticValues;
    staticValues.append(QtnEnumValueInfo(LANGUAGE::ENG, "ENG", "English"));
    
    static QtnEnumInfo enumInfo("LANGUAGE", staticValues);
    return enumInfo;
}

const QtnEnumInfo& LANGUAGE::info()
{
    static QtnEnumInfo& enumInfo = create_LANGUAGE_info();
    return enumInfo;
}
static QtnEnumInfo& create_TYPE_info()
{
    QVector<QtnEnumValueInfo> staticValues;
    
    static QtnEnumInfo enumInfo("TYPE", staticValues);
    return enumInfo;
}

const QtnEnumInfo& TYPE::info()
{
    static QtnEnumInfo& enumInfo = create_TYPE_info();
    return enumInfo;
}
static QtnEnumInfo& create_COLOR_info()
{
    QVector<QtnEnumValueInfo> staticValues;
    staticValues.append(QtnEnumValueInfo(COLOR::RED, "RED", "Red"));
    staticValues.append(QtnEnumValueInfo(COLOR::BLUE, "BLUE", "Blue", QtnEnumValueStateHidden | QtnEnumValueStateObsolete));
    staticValues.append(QtnEnumValueInfo(COLOR::YELLOW, "YELLOW", "Yellow"));
    
    static QtnEnumInfo enumInfo("COLOR", staticValues);
    return enumInfo;
}

const QtnEnumInfo& COLOR::info()
{
    static QtnEnumInfo& enumInfo = create_COLOR_info();
    return enumInfo;
}
static QtnEnumInfo& create_MASK_info()
{
    QVector<QtnEnumValueInfo> staticValues;
    staticValues.append(QtnEnumValueInfo(MASK::ONE, "ONE", "One"));
    staticValues.append(QtnEnumValueInfo(MASK::TWO, "TWO", "Two"));
    staticValues.append(QtnEnumValueInfo(MASK::FOUR, "FOUR", "Four"));
    
    static QtnEnumInfo enumInfo("MASK", staticValues);
    return enumInfo;
}

const QtnEnumInfo& MASK::info()
{
    static QtnEnumInfo& enumInfo = create_MASK_info();
    return enumInfo;
}

QtnPropertySetAllPropertyTypes::QtnPropertySetAllPropertyTypes(QObject* parent)
    : QtnPropertySet(parent)
    , bp(*qtnCreateProperty<QtnPropertyBool>(this))
    , bpc(*qtnCreateProperty<QtnPropertyBoolCallback>(this))
    , ip(*qtnCreateProperty<QtnPropertyInt>(this))
    , ipc(*qtnCreateProperty<QtnPropertyIntCallback>(this))
    , up(*qtnCreateProperty<QtnPropertyUInt>(this))
    , upc(*qtnCreateProperty<QtnPropertyUIntCallback>(this))
    , fp(*qtnCreateProperty<QtnPropertyFloat>(this))
    , fpc(*qtnCreateProperty<QtnPropertyFloatCallback>(this))
    , dp(*qtnCreateProperty<QtnPropertyDouble>(this))
    , dpc(*qtnCreateProperty<QtnPropertyDoubleCallback>(this))
    , sp(*qtnCreateProperty<QtnPropertyQString>(this))
    , spc(*qtnCreateProperty<QtnPropertyQStringCallback>(this))
    , rp(*qtnCreateProperty<QtnPropertyQRect>(this))
    , rpc(*qtnCreateProperty<QtnPropertyQRectCallback>(this))
    , pp(*qtnCreateProperty<QtnPropertyQPoint>(this))
    , ppc(*qtnCreateProperty<QtnPropertyQPointCallback>(this))
    , szp(*qtnCreateProperty<QtnPropertyQSize>(this))
    , szpc(*qtnCreateProperty<QtnPropertyQSizeCallback>(this))
    , ep(*qtnCreateProperty<QtnPropertyEnum>(this))
    , epc(*qtnCreateProperty<QtnPropertyEnumCallback>(this))
    , efp(*qtnCreateProperty<QtnPropertyEnumFlags>(this))
    , efpc(*qtnCreateProperty<QtnPropertyEnumFlagsCallback>(this))
    , cp(*qtnCreateProperty<QtnPropertyQColor>(this))
    , cpc(*qtnCreateProperty<QtnPropertyQColorCallback>(this))
    , fnp(*qtnCreateProperty<QtnPropertyQFont>(this))
    , fnpc(*qtnCreateProperty<QtnPropertyQFontCallback>(this))
    , bttn(*qtnCreateProperty<QtnPropertyButton>(this))
    , ppf(*qtnCreateProperty<QtnPropertyQPointF>(this))
    , ppfc(*qtnCreateProperty<QtnPropertyQPointFCallback>(this))
    , rpf(*qtnCreateProperty<QtnPropertyQRectF>(this))
    , rpfc(*qtnCreateProperty<QtnPropertyQRectFCallback>(this))
    , szpf(*qtnCreateProperty<QtnPropertyQSizeF>(this))
    , szpfc(*qtnCreateProperty<QtnPropertyQSizeFCallback>(this))
    , _b(true)
{
    
        _i =12;
        _ui = 9;
        _f = 0.2f;
        _d = 32.4;
        _s = "name";
        _r = QRect(10, 10, 10, 10);
        _rf = QRectF(10.1, 10.2, 10.3, 10.4);
        _p = QPoint(9, 2);
        _pf = QPointF(9.9, 2.2);
        _sz = QSize(33, 21);
        _szf = QSizeF(33.0, 21.9);
        _e = COLOR::RED;
        _ef = MASK::ONE|MASK::FOUR;
        _cl = QColor(Qt::red);
        _fn = QFont("Arial", 19);
    
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetAllPropertyTypes::~QtnPropertySetAllPropertyTypes()
{
    
        _b = false;
    
    disconnectSlots();
}

QtnPropertySetAllPropertyTypes& QtnPropertySetAllPropertyTypes::operator=(const QtnPropertySetAllPropertyTypes& other)
{
    Q_UNUSED(other);

    bp = other.bp;
    bpc = other.bpc;
    ip = other.ip;
    ipc = other.ipc;
    up = other.up;
    upc = other.upc;
    fp = other.fp;
    fpc = other.fpc;
    dp = other.dp;
    dpc = other.dpc;
    sp = other.sp;
    spc = other.spc;
    rp = other.rp;
    rpc = other.rpc;
    pp = other.pp;
    ppc = other.ppc;
    szp = other.szp;
    szpc = other.szpc;
    ep = other.ep;
    epc = other.epc;
    efp = other.efp;
    efpc = other.efpc;
    cp = other.cp;
    cpc = other.cpc;
    fnp = other.fnp;
    fnpc = other.fnpc;
    bttn = other.bttn;
    ppf = other.ppf;
    ppfc = other.ppfc;
    rpf = other.rpf;
    rpfc = other.rpfc;
    szpf = other.szpf;
    szpfc = other.szpfc;

    return *this;
}

QtnPropertySet* QtnPropertySetAllPropertyTypes::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetAllPropertyTypes(parentForNew);
}

QtnPropertySet* QtnPropertySetAllPropertyTypes::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetAllPropertyTypes* p = new QtnPropertySetAllPropertyTypes(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetAllPropertyTypes::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetAllPropertyTypes* theCopyFrom = qobject_cast<QtnPropertySetAllPropertyTypes*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->bp.state() & ignoreMask))
    {
        bp = theCopyFrom->bp;
    }

    if (!(theCopyFrom->bpc.state() & ignoreMask))
    {
        bpc = theCopyFrom->bpc;
    }

    if (!(theCopyFrom->ip.state() & ignoreMask))
    {
        ip = theCopyFrom->ip;
    }

    if (!(theCopyFrom->ipc.state() & ignoreMask))
    {
        ipc = theCopyFrom->ipc;
    }

    if (!(theCopyFrom->up.state() & ignoreMask))
    {
        up = theCopyFrom->up;
    }

    if (!(theCopyFrom->upc.state() & ignoreMask))
    {
        upc = theCopyFrom->upc;
    }

    if (!(theCopyFrom->fp.state() & ignoreMask))
    {
        fp = theCopyFrom->fp;
    }

    if (!(theCopyFrom->fpc.state() & ignoreMask))
    {
        fpc = theCopyFrom->fpc;
    }

    if (!(theCopyFrom->dp.state() & ignoreMask))
    {
        dp = theCopyFrom->dp;
    }

    if (!(theCopyFrom->dpc.state() & ignoreMask))
    {
        dpc = theCopyFrom->dpc;
    }

    if (!(theCopyFrom->sp.state() & ignoreMask))
    {
        sp = theCopyFrom->sp;
    }

    if (!(theCopyFrom->spc.state() & ignoreMask))
    {
        spc = theCopyFrom->spc;
    }

    if (!(theCopyFrom->rp.state() & ignoreMask))
    {
        rp = theCopyFrom->rp;
    }

    if (!(theCopyFrom->rpc.state() & ignoreMask))
    {
        rpc = theCopyFrom->rpc;
    }

    if (!(theCopyFrom->pp.state() & ignoreMask))
    {
        pp = theCopyFrom->pp;
    }

    if (!(theCopyFrom->ppc.state() & ignoreMask))
    {
        ppc = theCopyFrom->ppc;
    }

    if (!(theCopyFrom->szp.state() & ignoreMask))
    {
        szp = theCopyFrom->szp;
    }

    if (!(theCopyFrom->szpc.state() & ignoreMask))
    {
        szpc = theCopyFrom->szpc;
    }

    if (!(theCopyFrom->ep.state() & ignoreMask))
    {
        ep = theCopyFrom->ep;
    }

    if (!(theCopyFrom->epc.state() & ignoreMask))
    {
        epc = theCopyFrom->epc;
    }

    if (!(theCopyFrom->efp.state() & ignoreMask))
    {
        efp = theCopyFrom->efp;
    }

    if (!(theCopyFrom->efpc.state() & ignoreMask))
    {
        efpc = theCopyFrom->efpc;
    }

    if (!(theCopyFrom->cp.state() & ignoreMask))
    {
        cp = theCopyFrom->cp;
    }

    if (!(theCopyFrom->cpc.state() & ignoreMask))
    {
        cpc = theCopyFrom->cpc;
    }

    if (!(theCopyFrom->fnp.state() & ignoreMask))
    {
        fnp = theCopyFrom->fnp;
    }

    if (!(theCopyFrom->fnpc.state() & ignoreMask))
    {
        fnpc = theCopyFrom->fnpc;
    }

    if (!(theCopyFrom->bttn.state() & ignoreMask))
    {
        bttn = theCopyFrom->bttn;
    }

    if (!(theCopyFrom->ppf.state() & ignoreMask))
    {
        ppf = theCopyFrom->ppf;
    }

    if (!(theCopyFrom->ppfc.state() & ignoreMask))
    {
        ppfc = theCopyFrom->ppfc;
    }

    if (!(theCopyFrom->rpf.state() & ignoreMask))
    {
        rpf = theCopyFrom->rpf;
    }

    if (!(theCopyFrom->rpfc.state() & ignoreMask))
    {
        rpfc = theCopyFrom->rpfc;
    }

    if (!(theCopyFrom->szpf.state() & ignoreMask))
    {
        szpf = theCopyFrom->szpf;
    }

    if (!(theCopyFrom->szpfc.state() & ignoreMask))
    {
        szpfc = theCopyFrom->szpfc;
    }

    return true;
}

void QtnPropertySetAllPropertyTypes::init()
{
    static QString AllPropertyTypes_name = tr("AllPropertyTypes");
    setName(AllPropertyTypes_name);
    setId(13);
    
    // start children initialization
    static QString bp_name = tr("bp");
    bp.setName(bp_name);
    bp.setId(14);
    static QString bpc_name = tr("bpc");
    bpc.setName(bpc_name);
    bpc.setCallbackValueGet([this]() { return _b; });
    bpc.setCallbackValueSet([this](bool v) { _b = v; });
    bpc.setId(15);
    static QString ip_name = tr("ip");
    ip.setName(ip_name);
    ip.setId(16);
    static QString ipc_name = tr("ipc");
    ipc.setName(ipc_name);
    ipc.setCallbackValueGet([this]() { return _i; });
    ipc.setCallbackValueSet([this](qint32 v) { _i =v; });
    ipc.setId(17);
    static QString up_name = tr("up");
    up.setName(up_name);
    up.setId(18);
    static QString upc_name = tr("upc");
    upc.setName(upc_name);
    upc.setCallbackValueGet([this]() { return _ui; });
    upc.setCallbackValueSet([this](quint32 v) { _ui = v; });
    upc.setId(19);
    static QString fp_name = tr("fp");
    fp.setName(fp_name);
    fp.setId(20);
    static QString fpc_name = tr("fpc");
    fpc.setName(fpc_name);
    fpc.setCallbackValueGet([this]() { return _f; });
    fpc.setCallbackValueSet([this](float v) { _f = v; });
    fpc.setId(21);
    static QString dp_name = tr("dp");
    dp.setName(dp_name);
    dp.setId(22);
    static QString dpc_name = tr("dpc");
    dpc.setName(dpc_name);
    dpc.setCallbackValueGet([this]() { return _d; });
    dpc.setCallbackValueSet([this](double v) { _d = v; });
    dpc.setId(23);
    static QString sp_name = tr("sp");
    sp.setName(sp_name);
    sp.setId(24);
    static QString spc_name = tr("spc");
    spc.setName(spc_name);
    spc.setCallbackValueGet([this]() { return _s; });
    spc.setCallbackValueSet([this](QString v) { _s = v; });
    spc.setId(25);
    static QString rp_name = tr("rp");
    rp.setName(rp_name);
    rp.setId(26);
    static QString rpc_name = tr("rpc");
    rpc.setName(rpc_name);
    rpc.setCallbackValueGet([this]() { return _r; });
    rpc.setCallbackValueSet([this](QRect v) { _r = v; });
    rpc.setId(27);
    static QString pp_name = tr("pp");
    pp.setName(pp_name);
    pp.setId(28);
    static QString ppc_name = tr("ppc");
    ppc.setName(ppc_name);
    ppc.setCallbackValueGet([this]() { return _p; });
    ppc.setCallbackValueSet([this](QPoint v) { _p = v; });
    ppc.setId(29);
    static QString szp_name = tr("szp");
    szp.setName(szp_name);
    szp.setId(30);
    static QString szpc_name = tr("szpc");
    szpc.setName(szpc_name);
    szpc.setCallbackValueGet([this]() { return _sz; });
    szpc.setCallbackValueSet([this](QSize v) { _sz = v; });
    szpc.setId(31);
    static QString ep_name = tr("ep");
    ep.setName(ep_name);
    ep.setEnumInfo(&COLOR::info());
    ep.setId(32);
    ep.setValue(COLOR::BLUE);
    static QString epc_name = tr("epc");
    epc.setName(epc_name);
    epc.setCallbackValueGet([this]() { return _e; });
    epc.setCallbackValueSet([this](QtnEnumValueType v) { _e = v; });
    epc.setEnumInfo(&COLOR::info());
    epc.setId(33);
    static QString efp_name = tr("efp");
    efp.setName(efp_name);
    efp.setEnumInfo(&MASK::info());
    efp.setId(34);
    efp.setValue(MASK::ONE|MASK::FOUR);
    static QString efpc_name = tr("efpc");
    efpc.setName(efpc_name);
    efpc.setCallbackValueGet([this]() { return _ef; });
    efpc.setCallbackValueSet([this](QtnEnumFlagsValueType v) { _ef = v; });
    efpc.setEnumInfo(&MASK::info());
    efpc.setId(35);
    static QString cp_name = tr("cp");
    cp.setName(cp_name);
    cp.setId(36);
    cp.setValue(QColor(Qt::blue));
    static QString cpc_name = tr("cpc");
    cpc.setName(cpc_name);
    cpc.setCallbackValueGet([this]() { return _cl; });
    cpc.setCallbackValueSet([this](QColor v) { _cl = v; });
    cpc.setId(37);
    static QString fnp_name = tr("fnp");
    fnp.setName(fnp_name);
    fnp.setId(38);
    fnp.setValue(QFont("Courier", 10));
    static QString fnpc_name = tr("fnpc");
    fnpc.setName(fnpc_name);
    fnpc.setCallbackValueGet([this]() { return _fn; });
    fnpc.setCallbackValueSet([this](QFont v) { _fn = v; });
    fnpc.setId(39);
    static QString bttn_name = tr("bttn");
    bttn.setName(bttn_name);
    bttn.setId(40);
    static QString ppf_name = tr("ppf");
    ppf.setName(ppf_name);
    ppf.setId(41);
    static QString ppfc_name = tr("ppfc");
    ppfc.setName(ppfc_name);
    ppfc.setCallbackValueGet([this]() { return _pf; });
    ppfc.setCallbackValueSet([this](QPointF v) { _pf = v; });
    ppfc.setId(42);
    static QString rpf_name = tr("rpf");
    rpf.setName(rpf_name);
    rpf.setId(43);
    static QString rpfc_name = tr("rpfc");
    rpfc.setName(rpfc_name);
    rpfc.setCallbackValueGet([this]() { return _rf; });
    rpfc.setCallbackValueSet([this](QRectF v) { _rf = v; });
    rpfc.setId(44);
    static QString szpf_name = tr("szpf");
    szpf.setName(szpf_name);
    szpf.setId(45);
    static QString szpfc_name = tr("szpfc");
    szpfc.setName(szpfc_name);
    szpfc.setCallbackValueGet([this]() { return _szf; });
    szpfc.setCallbackValueSet([this](QSizeF v) { _szf = v; });
    szpfc.setId(46);
    // end children initialization
}

void QtnPropertySetAllPropertyTypes::connectSlots()
{
}

void QtnPropertySetAllPropertyTypes::disconnectSlots()
{
}

void QtnPropertySetAllPropertyTypes::connectDelegates()
{
}
static QtnEnumInfo& create_MY_TYPE_info()
{
    QVector<QtnEnumValueInfo> staticValues;
    staticValues.append(QtnEnumValueInfo(MY_TYPE::MY_TYPE1, "MY_TYPE1", "My type 1"));
    staticValues.append(QtnEnumValueInfo(MY_TYPE::MY_TYPE2, "MY_TYPE2", "My type 2"));
    
    static QtnEnumInfo enumInfo("MY_TYPE", staticValues);
    return enumInfo;
}

const QtnEnumInfo& MY_TYPE::info()
{
    static QtnEnumInfo& enumInfo = create_MY_TYPE_info();
    return enumInfo;
}

QtnPropertySetTest12::QtnPropertySetTest12(QObject* parent)
    : QtnPropertySet(parent)
    , p(*qtnCreateProperty<QtnPropertyEnum>(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetTest12::~QtnPropertySetTest12()
{
    disconnectSlots();
}

QtnPropertySetTest12& QtnPropertySetTest12::operator=(const QtnPropertySetTest12& other)
{
    Q_UNUSED(other);

    p = other.p;

    return *this;
}

QtnPropertySet* QtnPropertySetTest12::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetTest12(parentForNew);
}

QtnPropertySet* QtnPropertySetTest12::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetTest12* p = new QtnPropertySetTest12(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetTest12::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetTest12* theCopyFrom = qobject_cast<QtnPropertySetTest12*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->p.state() & ignoreMask))
    {
        p = theCopyFrom->p;
    }

    return true;
}

void QtnPropertySetTest12::init()
{
    static QString Test12_name = tr("Test12");
    setName(Test12_name);
    
    // start children initialization
    static QString p_name = tr("p");
    p.setName(p_name);
    p.setEnumInfo(&MY_TYPE::info());
    p.setValue(MY_TYPE::MY_TYPE1);
    // end children initialization
}

void QtnPropertySetTest12::connectSlots()
{
}

void QtnPropertySetTest12::disconnectSlots()
{
}

void QtnPropertySetTest12::connectDelegates()
{
}
