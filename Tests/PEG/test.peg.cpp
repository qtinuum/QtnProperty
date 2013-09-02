#include "test.peg.h"

#include <vector>

PropertySetTest1::PropertySetTest1(QObject *parent)
    : Qtinuum::Property(parent)
    , a(*new Qtinuum::PropertyInt(this))
    , text(*new Qtinuum::PropertyQString(this))
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetTest1::~PropertySetTest1()
{
    disconnectSlots();
}

void PropertySetTest1::init()
{
    static QString Test1_name = tr("Test1");
    setName(Test1_name);
    static QString description = "Test property_set description";
    setDescription(description);
    setState(0);
    
    // start children initialization
    static QString a_name = tr("a");
    a.setName(a_name);
    static QString a_description = "Descripion";
    a.setDescription(a_description);
    a.setMaxValue(10);
    a.setStepValue(-1);
    a.setValue(5);
    static QString text_name = tr("text");
    text.setName(text_name);
    static QString text_description = "defrf\"sde\"""deerf3rf"
    "derf r g\r\nreg r{}""dfrgerg"
    "fwrewre";
    text.setDescription(text_description);
    text.setValue(QString("#^{};"));
    // end children initialization
}

void PropertySetTest1::connectSlots()
{
}

void PropertySetTest1::disconnectSlots()
{
}

void PropertySetTest1::connectDelegates()
{
}

PropertySetTest2::PropertySetTest2(QObject *parent)
    : Qtinuum::Property(parent)
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetTest2::~PropertySetTest2()
{
    disconnectSlots();
}

void PropertySetTest2::init()
{
    static QString Test2_name = tr("Test2");
    setName(Test2_name);
}

void PropertySetTest2::connectSlots()
{
}

void PropertySetTest2::disconnectSlots()
{
}

void PropertySetTest2::connectDelegates()
{
}

PropertySetYY::PropertySetYY(QObject *parent)
    : Qtinuum::Property(parent)
    , rect(*new Qtinuum::PropertyQRect(this))
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetYY::~PropertySetYY()
{
    disconnectSlots();
}

void PropertySetYY::init()
{
    static QString yy_name = tr("yy");
    setName(yy_name);
    static QString description = tr("ss")+tr("ss");
    setDescription(description);
    
    // start children initialization
    static QString rect_name = tr("rect");
    rect.setName(rect_name);
    rect.setValue(QRect(10, 10, 10, 10));
    // end children initialization
}

void PropertySetYY::connectSlots()
{
}

void PropertySetYY::disconnectSlots()
{
}

void PropertySetYY::connectDelegates()
{
}

PropertySetAA::PropertySetAA(QObject *parent)
    : Qtinuum::Property(parent)
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetAA::~PropertySetAA()
{
    disconnectSlots();
}

void PropertySetAA::init()
{
    static QString aa_name = tr("aa");
    setName(aa_name);
}

void PropertySetAA::connectSlots()
{
}

void PropertySetAA::disconnectSlots()
{
}

void PropertySetAA::connectDelegates()
{
}

               // AA cpp code
           

PropertySetSS::PropertySetSS(QObject *parent)
    : qtn::Property(parent)
    , a(*new Qtinuum::PropertyBool(this))
    , aa(*new PropertySetAA(this))
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetSS::~PropertySetSS()
{
    disconnectSlots();
}

void PropertySetSS::init()
{
    static QString iis_name = tr("iis");
    setName(iis_name);
    
    // start children initialization
    static QString a_name = tr("a");
    a.setName(a_name);
    a.setValue(true);
    static QString aa_name = tr("aa");
    aa.setName(aa_name);
    // end children initialization
}

void PropertySetSS::connectSlots()
{
}

void PropertySetSS::disconnectSlots()
{
}

void PropertySetSS::connectDelegates()
{
}

PropertySetTest3::PropertySetTest3(QObject *parent)
    : qtn::Property(parent)
    , yy(*new PropertySetYY(this))
    , iis(*new PropertySetSS(this))
    , u(*new Qtinuum::PropertyBool(this))
    , xx(*new PropertySetTest2(this))
    , tt(*new PropertySetTest2(this))
    , s(*new PropertySetSS(this))
    , ww(*new Qtinuum::PropertyBool(this))
    , bc(*new Qtinuum::PropertyBoolCallback(this))
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetTest3::~PropertySetTest3()
{
    disconnectSlots();
}

void PropertySetTest3::init()
{
    static QString Test3_name = tr("Test3");
    setName(Test3_name);
    
    // start children initialization
    static QString yy_name = tr("yy");
    yy.setName(yy_name);
    static QString yy_description = tr("ss")+tr("ss");
    yy.setDescription(yy_description);
    static QString iis_name = tr("iis");
    iis.setName(iis_name);
    static QString u_name = tr("u");
    u.setName(u_name);
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
    static QString bc_name = tr("bc");
    bc.setName(bc_name);
    bc.setCallbackValueAccepted([](bool value)->bool {
            if (value) {
                return true;
            } else {
                return false;
            }
        });
    bc.setCallbackValueEqual([](bool value)->bool { return false; });
    bc.setCallbackValueGet([this]()->bool {
                return ww;
        });
    bc.setCallbackValueSet([this](bool value) {
            m_s = value;
        });
    // end children initialization
}

void PropertySetTest3::connectSlots()
{
    QObject::connect(this, &Qtinuum::Property::propertyDidChange, this, &PropertySetTest3::on_propertyDidChange);
    QObject::connect(&u, &Qtinuum::Property::propertyWillChange, this, &PropertySetTest3::on_u_propertyWillChange);
    QObject::connect(&u, &Qtinuum::Property::propertyDidChange, this, &PropertySetTest3::on_u_propertyDidChange);
    QObject::connect(&s.a, &Qtinuum::Property::propertyValueAccept, this, &PropertySetTest3::on_s_a_propertyValueAccept);
    QObject::connect(&s, &Qtinuum::Property::propertyWillChange, this, &PropertySetTest3::on_s_propertyWillChange);
}

void PropertySetTest3::disconnectSlots()
{
    QObject::disconnect(this, &Qtinuum::Property::propertyDidChange, this, &PropertySetTest3::on_propertyDidChange);
    QObject::disconnect(&u, &Qtinuum::Property::propertyWillChange, this, &PropertySetTest3::on_u_propertyWillChange);
    QObject::disconnect(&u, &Qtinuum::Property::propertyDidChange, this, &PropertySetTest3::on_u_propertyDidChange);
    QObject::disconnect(&s.a, &Qtinuum::Property::propertyValueAccept, this, &PropertySetTest3::on_s_a_propertyValueAccept);
    QObject::disconnect(&s, &Qtinuum::Property::propertyWillChange, this, &PropertySetTest3::on_s_propertyWillChange);
}

void PropertySetTest3::on_propertyDidChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason)
{
    
        // this propertyDidChange
    
}

void PropertySetTest3::on_u_propertyWillChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason, Qtinuum::PropertyValuePtr newValue)
{
    
        // u.propertyWillChange code
    
}

void PropertySetTest3::on_u_propertyDidChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason)
{
    
            // sub u propertyDidChange
        
}

void PropertySetTest3::on_s_a_propertyValueAccept(const Qtinuum::Property *property, Qtinuum::PropertyValuePtr valueToAccept, bool* accept)
{
    
            // s.a.propertyValueAccept
        
}

void PropertySetTest3::on_s_propertyWillChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason, Qtinuum::PropertyValuePtr newValue)
{
    
            yy.rect = QRect(1, 1, 1, 1);
            // ss propertyWillChange
        
}

void PropertySetTest3::connectDelegates()
{
}

        void aaa()
        {

        }
    
static Qtinuum::EnumInfo &create_LANGUAGE_info()
{
    QVector<Qtinuum::EnumValueInfo> staticValues;
    staticValues.append(Qtinuum::EnumValueInfo(LANGUAGE::ENG, "English"));
    
    static Qtinuum::EnumInfo enumInfo(staticValues);
    return enumInfo;
}

Qtinuum::EnumInfo &LANGUAGE::info()
{
    static Qtinuum::EnumInfo &enumInfo = create_LANGUAGE_info();
    return enumInfo;
}
static Qtinuum::EnumInfo &create_TYPE_info()
{
    QVector<Qtinuum::EnumValueInfo> staticValues;
    
    static Qtinuum::EnumInfo enumInfo(staticValues);
    return enumInfo;
}

Qtinuum::EnumInfo &TYPE::info()
{
    static Qtinuum::EnumInfo &enumInfo = create_TYPE_info();
    return enumInfo;
}
static Qtinuum::EnumInfo &create_COLOR_info()
{
    QVector<Qtinuum::EnumValueInfo> staticValues;
    staticValues.append(Qtinuum::EnumValueInfo(COLOR::RED, "Red"));
    staticValues.append(Qtinuum::EnumValueInfo(COLOR::BLUE, "Blue", Qtinuum::EnumValueStateHidden | Qtinuum::EnumValueStateObsolete));
    staticValues.append(Qtinuum::EnumValueInfo(COLOR::YELLOW, "Yellow"));
    
    static Qtinuum::EnumInfo enumInfo(staticValues);
    return enumInfo;
}

Qtinuum::EnumInfo &COLOR::info()
{
    static Qtinuum::EnumInfo &enumInfo = create_COLOR_info();
    return enumInfo;
}
static Qtinuum::EnumInfo &create_MASK_info()
{
    QVector<Qtinuum::EnumValueInfo> staticValues;
    staticValues.append(Qtinuum::EnumValueInfo(MASK::ONE, "One"));
    staticValues.append(Qtinuum::EnumValueInfo(MASK::TWO, "Two"));
    staticValues.append(Qtinuum::EnumValueInfo(MASK::FOUR, "Four"));
    
    static Qtinuum::EnumInfo enumInfo(staticValues);
    return enumInfo;
}

Qtinuum::EnumInfo &MASK::info()
{
    static Qtinuum::EnumInfo &enumInfo = create_MASK_info();
    return enumInfo;
}

PropertySetAllPropertyTypes::PropertySetAllPropertyTypes(int a, const bool s, std::function<void()> f, QObject *parent)
    : Qtinuum::Property(parent)
    , bp(*new Qtinuum::PropertyBool(this))
    , bpc(*new Qtinuum::PropertyBoolCallback(this))
    , ip(*new Qtinuum::PropertyInt(this))
    , ipc(*new Qtinuum::PropertyIntCallback(this))
    , up(*new Qtinuum::PropertyUInt(this))
    , upc(*new Qtinuum::PropertyUIntCallback(this))
    , fp(*new Qtinuum::PropertyFloat(this))
    , fpc(*new Qtinuum::PropertyFloatCallback(this))
    , dp(*new Qtinuum::PropertyDouble(this))
    , dpc(*new Qtinuum::PropertyDoubleCallback(this))
    , sp(*new Qtinuum::PropertyQString(this))
    , spc(*new Qtinuum::PropertyQStringCallback(this))
    , rp(*new Qtinuum::PropertyQRect(this))
    , rpc(*new Qtinuum::PropertyQRectCallback(this))
    , pp(*new Qtinuum::PropertyQPoint(this))
    , ppc(*new Qtinuum::PropertyQPointCallback(this))
    , szp(*new Qtinuum::PropertyQSize(this))
    , szpc(*new Qtinuum::PropertyQSizeCallback(this))
    , ep(*new Qtinuum::PropertyEnum(this))
    , epc(*new Qtinuum::PropertyEnumCallback(this))
    , efp(*new Qtinuum::PropertyEnumFlags(this))
    , efpc(*new Qtinuum::PropertyEnumFlagsCallback(this))
    , cp(*new Qtinuum::PropertyQColor(this))
    , cpc(*new Qtinuum::PropertyQColorCallback(this))
    , fnp(*new Qtinuum::PropertyQFont(this))
    , fnpc(*new Qtinuum::PropertyQFontCallback(this))
    , _b(s)
{
    
        _b = true;
        _i =12;
        _ui = 9;
        _f = 0.2f;
        _d = 32.4;
        _s = "name";
        _r = QRect(10, 10, 10, 10);
        _p = QPoint(9, 2);
        _sz = QSize(33, 21);
        _e = COLOR::RED;
        _ef = MASK::ONE|MASK::FOUR;
        _cl = QColor(Qt::red);
        _fn = QFont("Arial", 19);
    
    init();
    connectSlots();
    connectDelegates();
}

PropertySetAllPropertyTypes::~PropertySetAllPropertyTypes()
{
    
        _b = false;
    
    disconnectSlots();
}

void PropertySetAllPropertyTypes::init()
{
    static QString AllPropertyTypes_name = tr("AllPropertyTypes");
    setName(AllPropertyTypes_name);
    
    // start children initialization
    static QString bp_name = tr("bp");
    bp.setName(bp_name);
    bp.setId(1);
    static QString bpc_name = tr("bpc");
    bpc.setName(bpc_name);
    bpc.setCallbackValueGet([this]() { return _b; });
    bpc.setCallbackValueSet([this](bool v) { _b = v; });
    bpc.setId(2);
    static QString ip_name = tr("ip");
    ip.setName(ip_name);
    ip.setId(3);
    static QString ipc_name = tr("ipc");
    ipc.setName(ipc_name);
    ipc.setCallbackValueGet([this]() { return _i; });
    ipc.setCallbackValueSet([this](qint32 v) { _i =v; });
    ipc.setId(4);
    static QString up_name = tr("up");
    up.setName(up_name);
    up.setId(5);
    static QString upc_name = tr("upc");
    upc.setName(upc_name);
    upc.setCallbackValueGet([this]() { return _ui; });
    upc.setCallbackValueSet([this](quint32 v) { _ui = v; });
    upc.setId(6);
    static QString fp_name = tr("fp");
    fp.setName(fp_name);
    fp.setId(7);
    static QString fpc_name = tr("fpc");
    fpc.setName(fpc_name);
    fpc.setCallbackValueGet([this]() { return _f; });
    fpc.setCallbackValueSet([this](float v) { _f = v; });
    fpc.setId(8);
    static QString dp_name = tr("dp");
    dp.setName(dp_name);
    dp.setId(9);
    static QString dpc_name = tr("dpc");
    dpc.setName(dpc_name);
    dpc.setCallbackValueGet([this]() { return _d; });
    dpc.setCallbackValueSet([this](double v) { _d = v; });
    dpc.setId(10);
    static QString sp_name = tr("sp");
    sp.setName(sp_name);
    sp.setId(11);
    static QString spc_name = tr("spc");
    spc.setName(spc_name);
    spc.setCallbackValueGet([this]() { return _s; });
    spc.setCallbackValueSet([this](QString v) { _s = v; });
    spc.setId(12);
    static QString rp_name = tr("rp");
    rp.setName(rp_name);
    rp.setId(13);
    static QString rpc_name = tr("rpc");
    rpc.setName(rpc_name);
    rpc.setCallbackValueGet([this]() { return _r; });
    rpc.setCallbackValueSet([this](QRect v) { _r = v; });
    rpc.setId(14);
    static QString pp_name = tr("pp");
    pp.setName(pp_name);
    pp.setId(15);
    static QString ppc_name = tr("ppc");
    ppc.setName(ppc_name);
    ppc.setCallbackValueGet([this]() { return _p; });
    ppc.setCallbackValueSet([this](QPoint v) { _p = v; });
    ppc.setId(16);
    static QString szp_name = tr("szp");
    szp.setName(szp_name);
    szp.setId(17);
    static QString szpc_name = tr("szpc");
    szpc.setName(szpc_name);
    szpc.setCallbackValueGet([this]() { return _sz; });
    szpc.setCallbackValueSet([this](QSize v) { _sz = v; });
    szpc.setId(18);
    static QString ep_name = tr("ep");
    ep.setName(ep_name);
    ep.setEnumInfo(&COLOR::info());
    ep.setId(19);
    ep.setValue(COLOR::BLUE);
    static QString epc_name = tr("epc");
    epc.setName(epc_name);
    epc.setCallbackValueGet([this]() { return _e; });
    epc.setCallbackValueSet([this](qtn::EnumValueType v) { _e = v; });
    epc.setEnumInfo(&COLOR::info());
    epc.setId(20);
    static QString efp_name = tr("efp");
    efp.setName(efp_name);
    efp.setEnumInfo(&MASK::info());
    efp.setId(21);
    efp.setValue(MASK::ONE|MASK::FOUR);
    static QString efpc_name = tr("efpc");
    efpc.setName(efpc_name);
    efpc.setCallbackValueGet([this]() { return _ef; });
    efpc.setCallbackValueSet([this](qtn::EnumFlagsValueType v) { _ef = v; });
    efpc.setEnumInfo(&MASK::info());
    efpc.setId(22);
    static QString cp_name = tr("cp");
    cp.setName(cp_name);
    cp.setId(23);
    cp.setValue(QColor(Qt::blue));
    static QString cpc_name = tr("cpc");
    cpc.setName(cpc_name);
    cpc.setCallbackValueGet([this]() { return _cl; });
    cpc.setCallbackValueSet([this](QColor v) { _cl = v; });
    cpc.setId(24);
    static QString fnp_name = tr("fnp");
    fnp.setName(fnp_name);
    fnp.setId(25);
    fnp.setValue(QFont("Courier", 10));
    static QString fnpc_name = tr("fnpc");
    fnpc.setName(fnpc_name);
    fnpc.setCallbackValueGet([this]() { return _fn; });
    fnpc.setCallbackValueSet([this](QFont v) { _fn = v; });
    fnpc.setId(26);
    // end children initialization
}

void PropertySetAllPropertyTypes::connectSlots()
{
}

void PropertySetAllPropertyTypes::disconnectSlots()
{
}

void PropertySetAllPropertyTypes::connectDelegates()
{
}
