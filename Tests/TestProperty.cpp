#include "TestProperty.h"
#include "PEG/test.peg.h"
#include <QtTest/QtTest>
#include <QtScript/QScriptEngine>

static bool ret_true()
{
    return true;
}

static void verifyInitialValues(PropertySetAllPropertyTypes& pp)
{
    QVERIFY(pp.bp == false);
    QVERIFY(pp.bpc == true);
    QVERIFY(pp.ip == 0);
    QVERIFY(pp.ipc == 12);
    QVERIFY(pp.up == 0u);
    QVERIFY(pp.upc == 9u);
    QVERIFY(pp.fp == 0.f);
    QVERIFY(pp.fpc == 0.2f);
    QVERIFY(pp.dp == 0.);
    QVERIFY(pp.dpc == 32.4);
    QVERIFY(pp.sp == "");
    QVERIFY(pp.spc == "name");
    QVERIFY(pp.rp == QRect());
    QVERIFY(pp.rpc == QRect(10, 10, 10, 10));
    QVERIFY(pp.pp == QPoint());
    QVERIFY(pp.ppc == QPoint(9, 2));
    QVERIFY(pp.szp == QSize());
    QVERIFY(pp.szpc == QSize(33, 21));
    QVERIFY(pp.ep == COLOR::BLUE);
    QVERIFY(pp.epc == COLOR::RED);
    QVERIFY(pp.efp == (MASK::ONE|MASK::FOUR));
    QVERIFY(pp.efpc == (MASK::ONE|MASK::FOUR));
    QVERIFY(pp.cp == QColor(Qt::blue));
    QVERIFY(pp.cpc == QColor(Qt::red));
    QVERIFY(pp.fnp == QFont("Courier", 10));
    QVERIFY(pp.fnpc == QFont("Arial", 19));
}

static void modify(PropertySetAllPropertyTypes& pp)
{
    pp.bp = true;
    pp.bpc = false;
    pp.ip = 12;
    pp.ipc = 2;
    pp.up = 3u;
    pp.upc = 32u;
    pp.fp = 0.32f;
    pp.fpc = 32.2f;
    pp.dp = 12.;
    pp.dpc = 2.4;
    pp.sp = "test";
    pp.spc = "name#1";
    pp.rp = QRect(12, 10, 16, 17);
    pp.rpc = QRect(4, 3, 1, 0);
    pp.pp = QPoint(21, 9);
    pp.ppc = QPoint(15, 0);
    pp.szp = QSize(90, 87);
    pp.szpc = QSize(0, 1);
    pp.ep = COLOR::RED;
    pp.epc = COLOR::BLUE;
    pp.efp = (MASK::FOUR);
    pp.efpc = (MASK::ONE);
    pp.cp = QColor(Qt::red);
    pp.cpc = QColor(Qt::blue);
    pp.fnp = QFont("Courier New", 11);
    pp.fnpc = QFont("Myfont", 0);
}

static void verifyModified(PropertySetAllPropertyTypes& pp)
{
    QVERIFY(pp.bp == true);
    QVERIFY(pp.bpc == false);
    QVERIFY(pp.ip == 12);
    QVERIFY(pp.ipc == 2);
    QVERIFY(pp.up == 3u);
    QVERIFY(pp.upc == 32u);
    QVERIFY(pp.fp == 0.32f);
    QVERIFY(pp.fpc == 32.2f);
    QVERIFY(pp.dp == 12.);
    QVERIFY(pp.dpc == 2.4);
    QVERIFY(pp.sp == "test");
    QVERIFY(pp.spc == "name#1");
    QVERIFY(pp.rp == QRect(12, 10, 16, 17));
    QVERIFY(pp.rpc == QRect(4, 3, 1, 0));
    QVERIFY(pp.pp == QPoint(21, 9));
    QVERIFY(pp.ppc == QPoint(15, 0));
    QVERIFY(pp.szp == QSize(90, 87));
    QVERIFY(pp.szpc == QSize(0, 1));
    QVERIFY(pp.ep == COLOR::RED);
    QVERIFY(pp.epc == COLOR::BLUE);
    QVERIFY(pp.efp == (MASK::FOUR));
    QVERIFY(pp.efpc == (MASK::ONE));
    QVERIFY(pp.cp == QColor(Qt::red));
    QVERIFY(pp.cpc == QColor(Qt::blue));
    QVERIFY(pp.fnp == QFont("Courier New", 11));
    QVERIFY(pp.fnpc == QFont("Myfont", 0));
}

using namespace Qtinuum;

void TestProperty::name()
{
    PropertyInt p(this);
    p.setName(tr("PropertyName"));
    QCOMPARE(p.name(), QString("PropertyName"));

    PropertySet p1(nullptr);
    p1.setName("Another Name");
    QCOMPARE(p1.name(), QString("Another Name"));
}

void TestProperty::description()
{
    PropertyBool p(this);
    p.setDescription(tr("PropertyDescription"));
    QCOMPARE(p.description(), QString("PropertyDescription"));

    PropertySet p1(nullptr);
    p1.setDescription("Another description\nwith multiline.");
    QCOMPARE(p1.description(), QString("Another description\nwith multiline."));
}

void TestProperty::id()
{
    PropertySet p(nullptr);
    QCOMPARE(p.id(), PropertyIDInvalid);
    p.setId(12);
    QCOMPARE(p.id(), 12);

    PropertyUInt p1(&p);
    QCOMPARE(p1.id(), PropertyIDInvalid);
    p1.setId(5);
    QCOMPARE(p1.id(), 5);

}

void TestProperty::state()
{
    PropertySet ps(0);
    PropertyInt p(&ps);

    QCOMPARE(ps.state(), PropertyStateNone);
    QCOMPARE(p.state(), PropertyStateNone);

    PropertyState master_state = PropertyStateInvisible|PropertyStateNonSimple;

    ps.setState(master_state);
    QCOMPARE(ps.state(), master_state);
    QCOMPARE(ps.stateLocal(), master_state);
    QCOMPARE(ps.stateInherited(), PropertyStateNone);
    QCOMPARE(p.state(), master_state);
    QCOMPARE(p.stateLocal(), PropertyStateNone);
    QCOMPARE(p.stateInherited(), master_state);

    p.addState(PropertyStateCollapsed);
    QCOMPARE(ps.state(), master_state);
    QCOMPARE(ps.stateLocal(), master_state);
    QCOMPARE(ps.stateInherited(), PropertyStateNone);
    QCOMPARE(p.state(), master_state|PropertyStateCollapsed);
    QCOMPARE(p.stateLocal(), PropertyStateCollapsed);
    QCOMPARE(p.stateInherited(), master_state);

    ps.removeState(PropertyStateInvisible);
    QCOMPARE(ps.state(), PropertyStateNonSimple);
    QCOMPARE(ps.stateLocal(), PropertyStateNonSimple);
    QCOMPARE(ps.stateInherited(), PropertyStateNone);
    QCOMPARE(p.state(), PropertyStateNonSimple|PropertyStateCollapsed);
    QCOMPARE(p.stateLocal(), PropertyStateCollapsed);
    QCOMPARE(p.stateInherited(), PropertyStateNonSimple);

    p.switchState(PropertyStateCollapsed, false);
    ps.switchState(PropertyStateInvisible, true);
    QCOMPARE(ps.state(), master_state);
    QCOMPARE(ps.stateLocal(), master_state);
    QCOMPARE(ps.stateInherited(), PropertyStateNone);
    QCOMPARE(p.state(), master_state);
    QCOMPARE(p.stateLocal(), PropertyStateNone);
    QCOMPARE(p.stateInherited(), master_state);

    QCOMPARE(ps.isEditableByUser(), false);

    ps.removeState(master_state|PropertyStateCollapsed);
    QCOMPARE(ps.isEditableByUser(), true);
    QCOMPARE(ps.state(), PropertyStateNone);

    int call_count = 0;
    QObject::connect(&ps, &Property::propertyWillChange, [&call_count](const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason, PropertyValuePtr newValue) {
        ++call_count;
    });

    QObject::connect(&ps, &Property::propertyDidChange, [&call_count](const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason) {
        ++call_count;
    });

    ps.addState(PropertyStateNone, true);
    QCOMPARE(call_count, 2);

    ps.removeState(PropertyStateCollapsed);
    QCOMPARE(call_count, 2);

    ps.removeState(PropertyStateCollapsed, true);
    QCOMPARE(call_count, 4);
}

void TestProperty::stateChange()
{
    PropertyFloat p(this);
    QObject::connect(&p, &Property::propertyWillChange, this, &TestProperty::checkPropertyStateIsNonSimple);

    p.addState(PropertyStateNonSimple);
}

void TestProperty::propertyDelegate()
{
    PropertyBool p(this);
    QVERIFY(!p.delegate());

    PropertyDelegateInfo info;
    info.name = "delegateName";
    p.setDelegate(info);
    QVERIFY(p.delegate());
    QVERIFY(p.delegate()->name == "delegateName");
    QVERIFY(p.delegate()->attributes.isEmpty());

    info.attributes["int"] = 4;
    p.setDelegate(info);
    QVERIFY(p.delegate()->attributes.size() == 1);
    QVERIFY(p.delegate()->attributes["int"] == 4);

    info.attributes["str"] = tr("Text");
    p.setDelegate(info);
    QVERIFY(p.delegate()->attributes.size() == 2);
    QVERIFY(p.delegate()->attributes["str"] == "Text");

    info.attributes["int"] = 12;
    p.setDelegate(info);
    QVERIFY(p.delegate()->attributes.size() == 2);
    QVERIFY(p.delegate()->attributes["int"] == 12);
}

void TestProperty::propertyDelegateCallback()
{
    PropertyBool p(this);
    QVERIFY(!p.delegate());

    p.setDelegateCallback([]()->const PropertyDelegateInfo* {
        PropertyDelegateInfo *info(new PropertyDelegateInfo());
        info->name = "delegate";
        info->attributes["one"] = 1;
        info->attributes["color"] = QColor(Qt::black);
        return info;
    });

    QVERIFY(p.delegate());
    QVERIFY(p.delegate()->name == "delegate");
    QVERIFY(p.delegate()->attributes.size() == 2);
    QVERIFY(p.delegate()->attributes["one"] == 1);
    QVERIFY(p.delegate()->attributes["color"] == QColor(Qt::black));
}

void TestProperty::propertyBool()
{
    {
        PropertyBoolCallback p(this);

        bool hasException = false;
        try
        {
            if (p) {}
        }
        catch (const std::exception &e)
        {
            Q_UNUSED(e);
            hasException = true;
        }
        QVERIFY(hasException);

        p.setCallbackValueGet([]()->bool {
            return false;
        });
        QVERIFY(p.value() == false);

        p.setCallbackValueSet([](bool newValue) {
            QVERIFY(newValue == true);
        });

        p = true;

        p.setCallbackValueSet([](bool newValue) {
            QVERIFY(newValue == false);
        });
        p.setValue(false);

        QVERIFY(!p);
        if (p) QFAIL("p expected as false");

        p.setCallbackValueGet(&ret_true);
        QVERIFY(p);

        switch (p)
        {
            case false: QFAIL("p expected as true");
        }

        PropertyBoolBase& p1 = p;
        QVERIFY(p1);
        if (p1 == false) QFAIL("p1 expected as true");

        switch (p1)
        {
            case false: QFAIL("p1 expected as true");
        }

        PropertyBoolCallback p2(this);
        bool val = true;

        p2.setCallbackValueGet([&val] ()->bool {
                return val;
        });

        p2.setCallbackValueSet([&val] (bool newValue) {
            val = newValue;
        });

        QVERIFY(p2);

        p2 = false;
        QVERIFY(!val);

        p2 = p;
        QVERIFY(p);

        p2 = !p;
        QVERIFY(!p2);

        p1 = p2;
        QVERIFY(p1 && p && !p2);

        PropertyBool p3(this);
        p2 = p3 = true;
        QVERIFY(p2);
    }

    {
        PropertyBool p(this);
        QVERIFY(!p);
        if (p) QFAIL("p expected as false");

        p = true;
        QVERIFY(p == true);

        p = (p != true);
        QVERIFY(!p);

        switch (p + 4)
        {
            case 4: break;
            default: QFAIL("p expected as false");
        }

        p = 5;
        QVERIFY(p);

        QVERIFY(p != false);
        QVERIFY(p == true);
        QVERIFY(p && (p > false));
        QVERIFY(p >= true);
        QVERIFY(!(p <= false));

        PropertyBoolCallback p2(this);
        p2.setCallbackValueGet([]()->bool { return false; });
        p = p2;
        QVERIFY(!p);
    }
}

void TestProperty::propertyInt()
{
    PropertyInt p(this);
    p.setMaxValue(10);
    p.setMinValue(1);
    p.setValue(5);
    QVERIFY(p.value() == 5);

    p.setMaxValue(3);
    QVERIFY(p.value() == 3);

    p = 1;
    QVERIFY(p == 1);
    QVERIFY(p > 0);
    QVERIFY(p >= 0);
    QVERIFY(p < 2);
    QVERIFY(p <= 2);
}

void TestProperty::propertyString()
{
    PropertyQString p(this);
    QVERIFY(p == tr(""));
    p = tr("hello");
    QVERIFY(p == tr("hello"));
    p = "latin string";
    QVERIFY(p == "latin string");

    const QString &val = p;
    QVERIFY(val == "latin string");
}

void TestProperty::propertyRect()
{
    PropertyQRect p(this);
    QVERIFY(p == QRect());
    QRect rect(1, 2, 3, 4);
    p = rect;
    QVERIFY(p == rect);

    PropertyQRectCallback pc(this);
    QRect pc_value;
    pc.setCallbackValueGet([&pc_value]()->const QRect& { return pc_value; });
    pc.setCallbackValueSet([&pc_value](const QRect& value) { pc_value = value; });

    pc = QRect(10, 10, 10, 10);
    QCOMPARE(pc.value(), QRect(10, 10, 10, 10));

    pc = p;
    QVERIFY(pc.value() == p.value());
    QVERIFY(pc == p);

    PropertyQRectBase *pb = &pc;
    *pb = QRect(3, 4, 5, 6);
    QVERIFY(pc.value() == pb->value() && (*pb) == QRect(3, 4, 5, 6));
}

void TestProperty::propertyEnum()
{
    PropertyEnum p(this);
    p = COLOR::BLUE;
    QVERIFY(p != COLOR::BLUE);
    QVERIFY(!p.setValue(COLOR::RED));

    p.setEnumInfo(&COLOR::info());
    QVERIFY(p.setValue(COLOR::RED));
    QVERIFY(p == COLOR::RED);

    PropertyEnumCallback pc(this);
    pc.setCallbackValueGet([]()->EnumValueType {return COLOR::YELLOW; });
    QVERIFY(pc == COLOR::YELLOW);
}

void TestProperty::propertyEnumFlags()
{
    PropertyEnumFlags p(this);
    p.setEnumInfo(&MASK::info());
    p = MASK::ONE;
    //QVERIFY(p == MASK::ONE);
    p = MASK::TWO | MASK::FOUR;
    QVERIFY(p & MASK::FOUR);
    QVERIFY(!(p & MASK::ONE));
}

static void testSerializationState(PropertyBase& p)
{
    p.setState(PropertyStateCollapsed);

    QByteArray data;

    {
        QDataStream s(&data, QIODevice::WriteOnly);
        QVERIFY(p.save(s));
    }

    p.setState(PropertyStateImmutable);
    QCOMPARE(p.state(), PropertyStateImmutable);

    {
        QDataStream s(&data, QIODevice::ReadOnly);
        QVERIFY(p.load(s));
    }

    QCOMPARE(p.state(), PropertyStateCollapsed);
}

void TestProperty::serializationState()
{
    {
        PropertyInt p(this);
        testSerializationState(p);
    }

    {
        PropertySet p(nullptr);
        testSerializationState(p);
    }
}

void TestProperty::serializationChildren()
{
    PropertySet ps(this);
    ps.setState(PropertyStateCollapsed);

    QByteArray data;

    Property* p2(new PropertyInt(&ps));
    p2->setId(1);
    p2->setState(PropertyStateImmutable);

    PropertyBool p3(&ps);
    p3.setId(2);
    p3.setState(PropertyStateNonSerialized);

    {
        QDataStream s(&data, QIODevice::WriteOnly);
        s << ps;
        QCOMPARE(s.status(), QDataStream::Ok);
    }

    QCOMPARE(p2->state(), (PropertyStateImmutable|PropertyStateCollapsed));
    QCOMPARE(p3.state(), (PropertyStateNonSerialized|PropertyStateCollapsed));

    p2->removeState(PropertyStateImmutable);
    p3.addState(PropertyStateImmutable);

    QCOMPARE(p2->state(), PropertyStateCollapsed);
    QCOMPARE(p3.state(), (PropertyStateNonSerialized|PropertyStateImmutable|PropertyStateCollapsed));

    {
        QDataStream s(&data, QIODevice::ReadOnly);
        s >> ps;
        QCOMPARE(s.status(), QDataStream::Ok);
    }

    QCOMPARE(p2->state(), (PropertyStateImmutable|PropertyStateCollapsed));
    QCOMPARE(p3.state(), (PropertyStateNonSerialized|PropertyStateImmutable|PropertyStateCollapsed));

    delete p2;
    p2 = 0;

    {
        QDataStream s(&data, QIODevice::ReadOnly);
        s >> ps;
        QCOMPARE(s.status(), QDataStream::Ok);
    }
}

void TestProperty::serializationValue()
{
    PropertyBool p(this);
    p = true;
    QVERIFY(p);

    QByteArray data;

    {
        QDataStream s(&data, QIODevice::WriteOnly);
        s << p;
        QCOMPARE(s.status(), QDataStream::Ok);
    }

    p = false;
    QVERIFY(!p);

    {
        QDataStream s(&data, QIODevice::ReadOnly);
        s >> p;
        QCOMPARE(s.status(), QDataStream::Ok);
    }

    QVERIFY(p);

    PropertySetAllPropertyTypes pp(this);

    verifyInitialValues(pp);

    {
        QDataStream s(&data, QIODevice::WriteOnly);
        s << pp;
        QCOMPARE(s.status(), QDataStream::Ok);
    }

    modify(pp);
    verifyModified(pp);

    {
        QDataStream s(&data, QIODevice::ReadWrite);
        s >> pp;
        QCOMPARE(s.status(), QDataStream::Ok);
    }

    verifyInitialValues(pp);
}

void TestProperty::createNew()
{
    {
        PropertySetAllPropertyTypes pp(this);

        verifyInitialValues(pp);

        modify(pp);

        QScopedPointer<PropertySetAllPropertyTypes> pn;
        pn.reset(qobject_cast<PropertySetAllPropertyTypes*>(pp.createNew(this)));
        QVERIFY(pn);
        QVERIFY(pn.data() != &pp);
        QCOMPARE(pn->parent(), this);

        verifyInitialValues(*pn);
    }
}

void TestProperty::createCopy()
{
    {
        PropertySetAllPropertyTypes pp(this);

        verifyInitialValues(pp);

        modify(pp);

        QScopedPointer<PropertySetAllPropertyTypes> pn;
        pn.reset(qobject_cast<PropertySetAllPropertyTypes*>(pp.createCopy(this)));
        QVERIFY(pn);
        QVERIFY(pn.data() != &pp);
        QCOMPARE(pn->parent(), this);

        verifyModified(*pn);
    }
}

void TestProperty::copyValues()
{
    {
        PropertySetAllPropertyTypes pp(this);
        verifyInitialValues(pp);
        modify(pp);

        PropertySetAllPropertyTypes pp1(this);
        verifyInitialValues(pp1);
        pp1.copyValues(&pp, PropertyStateNone);
        verifyModified(pp1);
    }

    {
        PropertySetAllPropertyTypes pp(this);
        verifyInitialValues(pp);
        modify(pp);

        PropertySetAllPropertyTypes pp1(this);
        verifyInitialValues(pp1);
        QCOMPARE(pp1.bp.value(), false);
        pp.bp.addState(PropertyStateImmutable);
        QCOMPARE(pp.bp.state(), PropertyStateImmutable);

        pp1.copyValues(&pp, PropertyStateImmutable);
        QCOMPARE(pp1.bp.value(), false);

        pp1.copyValues(&pp);
        verifyModified(pp1);
    }
}

void TestProperty::propertyAssignment()
{
    {
        PropertyDouble p1(this);
        PropertyDouble p2(this);
        PropertyDoubleCallback p3(this);
        double d = 54.32;

        p1 = 0.5;
        p2 = 1.3;
        p3.setCallbackValueGet([]()->double { return 23.4; });
        p3.setCallbackValueSet([&d](double v) { d = v; });

        QCOMPARE(p1.value(), 0.5);
        QCOMPARE(p2.value(), 1.3);
        QCOMPARE(p3.value(), 23.4);

        p2 = p1;
        QCOMPARE(p1.value(), 0.5);
        QCOMPARE(p2.value(), 0.5);

        p1 = p3;
        QCOMPARE(p3.value(), 23.4);
        QCOMPARE(p1.value(), 23.4);

        p3 = p2;
        QCOMPARE(p2.value(), 0.5);
        QCOMPARE(p3.value(), 23.4);
        QCOMPARE(d, 0.5);
    }

    {
        PropertySetAllPropertyTypes pp(this);

        verifyInitialValues(pp);

        PropertySetAllPropertyTypes pp1(this);

        verifyInitialValues(pp1);

        modify(pp);

        verifyModified(pp);

        pp1 = pp;

        verifyModified(pp1);
    }
}

void TestProperty::propertyScripting()
{

    {
        PropertyBool b(this);
        b.setName("isValid");
        b.setDescription("isValid property.");
        b.setId(15);

        QScriptEngine eng;
        scriptRegisterPropertyTypes(&eng);
        eng.globalObject().setProperty("b", eng.newQObject(&b));

        QScriptValue val = eng.evaluate("b.name");
        QCOMPARE(val.toString(), b.name());

        val = eng.evaluate("b.name = \"a\"");
        QCOMPARE(b.name(), QString("isValid"));

        val = eng.evaluate("b.description");
        QCOMPARE(val.toString(), QString("isValid property."));

        val = eng.evaluate("b.description = 'new description'");
        QCOMPARE(b.description(), QString("isValid property."));

        val = eng.evaluate("var id = b.id");
        val = eng.evaluate("id");
        QVERIFY(val.isNumber());
        QCOMPARE(val.toNumber(), 15.);

        val = eng.evaluate("b.id = 92");
        QCOMPARE(b.id(), 15);

        val = eng.evaluate("b.isEditable");
        QCOMPARE(val.toBool(), true);

        val = eng.evaluate("b.isEditable = false");
        QCOMPARE(b.isEditableByUser(), true);

        b.addState(PropertyStateImmutable);
        val = eng.evaluate("b.isEditable");
        QCOMPARE(val.toBool(), false);

        b.setState(PropertyStateInvisible);
        val = eng.evaluate("b.isEditable");
        QCOMPARE(val.toBool(), false);

        val = eng.evaluate("b.value");
        QCOMPARE(val.toBool(), false);

        b = true;
        val = eng.evaluate("b.value");
        QCOMPARE(val.toBool(), true);

        val = eng.evaluate("b.value = false");
        QCOMPARE(b.value(), true);

        b.setState(PropertyStateNone);
        val = eng.evaluate("b.value = false");
        QCOMPARE(b.value(), false);

        eng.evaluate("var b_changed = false;"
                     "function on_b_changed() {"
                     "  b_changed = true;"
                     "}");

        val = eng.evaluate("b_changed");
        QCOMPARE(val.toBool(), false);

        val = eng.evaluate("on_b_changed()");
        val = eng.evaluate("b_changed");
        QCOMPARE(val.toBool(), true);

        eng.evaluate("var success = true;");
        eng.evaluate("var f1 = function(p1, p2, reason) {"
                     "  if (p1 != p2) success = false;"
                     "  if (p1.name != 'isValid') success = false;"
                     "  if (p1.state != Qtinuum.PropertyStateNone) success = false;"
                     "  if (p1.id != 15) success = false;"
                     "  if (reason != Qtinuum.PropertyChangeReasonNewValue) success = false;"
                     "  if (p1.value != true) success = false;"
                     "}");
        eng.evaluate("b.propertyDidChange.connect(f1);");
        eng.evaluate("b.value = true;");
        val = eng.evaluate("success");
        QCOMPARE(val.toBool(), true);

        eng.evaluate("b.propertyDidChange.disconnect(f1);");
        eng.evaluate("b.value = true;");
        val = eng.evaluate("success");
        QCOMPARE(val.toBool(), true);
    }
}

void TestProperty::variantConversions()
{
    {
        PropertyBool b(this);
        QVariant value = true;

        QCOMPARE(b.value(), false);
        QVERIFY(b.fromVariant(value));
        QCOMPARE(b.value(), true);

        value = 17;
        QVERIFY(!b.fromVariant(value));
        QCOMPARE(b.value(), true);

        QVERIFY(b.toVariant(value));
        QCOMPARE(value.toBool(), true);

        b = false;
        QVERIFY(b.toVariant(value));
        QCOMPARE(value.toBool(), false);
    }

    {
        PropertySetAllPropertyTypes pp(this);
        QVariant value;
        bool ok = false;

        QVERIFY(pp.bp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Bool);
        QCOMPARE(value.toBool(), false);
        value = true;
        QVERIFY(pp.bp.fromVariant(value));
        QCOMPARE(pp.bp.value(), true);
        value = QPoint(3, 5);
        QVERIFY(!pp.bp.fromVariant(value));
        QCOMPARE(pp.bp.value(), true);

        QVERIFY(pp.bpc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Bool);
        QCOMPARE(value.toBool(), true);
        value = false;
        QVERIFY(pp.bpc.fromVariant(value));
        QCOMPARE(pp.bpc.value(), false);
        value = QPoint(3, 5);
        QVERIFY(!pp.bpc.fromVariant(value));
        QCOMPARE(pp.bpc.value(), false);

        QVERIFY(pp.ip.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Int);
        QCOMPARE(value.toInt(&ok), 0);
        QVERIFY(ok);
        value = 13;
        QVERIFY(pp.ip.fromVariant(value));
        QCOMPARE(pp.ip.value(), 13);
        value = QPoint(3, 5);
        QVERIFY(!pp.ip.fromVariant(value));
        QCOMPARE(pp.ip.value(), 13);

        QVERIFY(pp.ipc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Int);
        QCOMPARE(value.toInt(&ok), 12);
        QVERIFY(ok);
        value = -83;
        QVERIFY(pp.ipc.fromVariant(value));
        QCOMPARE(pp.ipc.value(), -83);
        value = QPoint(3, 5);
        QVERIFY(!pp.ipc.fromVariant(value));
        QCOMPARE(pp.ipc.value(), -83);

        QVERIFY(pp.up.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::UInt);
        QCOMPARE(value.toUInt(&ok), 0u);
        QVERIFY(ok);
        value = 98234;
        QVERIFY(pp.up.fromVariant(value));
        QCOMPARE(pp.up.value(), 98234u);
        value = QPoint(3, 5);
        QVERIFY(!pp.up.fromVariant(value));
        QCOMPARE(pp.up.value(), 98234u);

        QVERIFY(pp.upc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::UInt);
        QCOMPARE(value.toUInt(&ok), 9u);
        QVERIFY(ok);
        value = 76u;
        QVERIFY(pp.upc.fromVariant(value));
        QCOMPARE(pp.upc.value(), 76u);
        value = QPoint(3, 5);
        QVERIFY(!pp.upc.fromVariant(value));
        QCOMPARE(pp.upc.value(), 76u);

        QVERIFY(pp.fp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Float);
        QCOMPARE(value.toFloat(&ok), 0.f);
        QVERIFY(ok);
        value = 0.32f;
        QVERIFY(pp.fp.fromVariant(value));
        QCOMPARE(pp.fp.value(), 0.32f);
        value = QPoint(3, 5);
        QVERIFY(!pp.fp.fromVariant(value));
        QCOMPARE(pp.fp.value(), 0.32f);

        QVERIFY(pp.fpc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Float);
        QCOMPARE(value.toFloat(&ok), 0.2f);
        QVERIFY(ok);
        value = -0.32f;
        QVERIFY(pp.fpc.fromVariant(value));
        QCOMPARE(pp.fpc.value(), -0.32f);
        value = QPoint(3, 5);
        QVERIFY(!pp.fpc.fromVariant(value));
        QCOMPARE(pp.fpc.value(), -0.32f);

        QVERIFY(pp.dp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Double);
        QCOMPARE(value.toDouble(&ok), 0.);
        QVERIFY(ok);
        value = -23.4;
        QVERIFY(pp.dp.fromVariant(value));
        QCOMPARE(pp.dp.value(), -23.4);
        value = QPoint(3, 5);
        QVERIFY(!pp.dp.fromVariant(value));
        QCOMPARE(pp.dp.value(), -23.4);

        QVERIFY(pp.dpc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Double);
        QCOMPARE(value.toDouble(&ok), 32.4);
        QVERIFY(ok);
        value = 543.22;
        QVERIFY(pp.dpc.fromVariant(value));
        QCOMPARE(pp.dpc.value(), 543.22);
        value = QPoint(3, 5);
        QVERIFY(!pp.dpc.fromVariant(value));
        QCOMPARE(pp.dpc.value(), 543.22);

        pp.sp = tr("hello");
        QVERIFY(pp.sp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QString);
        QCOMPARE(value.toString(), tr("hello"));
        value = tr("world");
        QVERIFY(pp.sp.fromVariant(value));
        QCOMPARE(pp.sp.value(), tr("world"));
        value = QPoint(3, 5);
        QVERIFY(!pp.sp.fromVariant(value));
        QCOMPARE(pp.sp.value(), tr("world"));

        QVERIFY(pp.spc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QString);
        QCOMPARE(value.toString(), tr("name"));
        value = tr("another");
        QVERIFY(pp.spc.fromVariant(value));
        QCOMPARE(pp.spc.value(), tr("another"));
        value = QPoint(3, 5);
        QVERIFY(!pp.spc.fromVariant(value));
        QCOMPARE(pp.spc.value(), tr("another"));

        QVERIFY(pp.rp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QRect);
        QCOMPARE(value.toRect(), QRect());
        value = QRect(23, 4, 56, 76);
        QVERIFY(pp.rp.fromVariant(value));
        QCOMPARE(pp.rp.value(), QRect(23, 4, 56, 76));
        value = "sss";
        QVERIFY(!pp.rp.fromVariant(value));
        QCOMPARE(pp.rp.value(), QRect(23, 4, 56, 76));

        QVERIFY(pp.rpc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QRect);
        QCOMPARE(value.toRect(), QRect(10, 10, 10, 10));
        value = QRect(-2, 3, 23, 3);
        QVERIFY(pp.rpc.fromVariant(value));
        QCOMPARE(pp.rpc.value(), QRect(-2, 3, 23, 3));
        value = "sss";
        QVERIFY(!pp.rpc.fromVariant(value));
        QCOMPARE(pp.rpc.value(), QRect(-2, 3, 23, 3));

        QVERIFY(pp.pp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QPoint);
        QCOMPARE(value.toPoint(), QPoint());
        value = QPoint(-2, 3);
        QVERIFY(pp.pp.fromVariant(value));
        QCOMPARE(pp.pp.value(), QPoint(-2, 3));
        value = "sss";
        QVERIFY(!pp.pp.fromVariant(value));
        QCOMPARE(pp.pp.value(), QPoint(-2, 3));

        QVERIFY(pp.ppc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QPoint);
        QCOMPARE(value.toPoint(), QPoint(9, 2));
        value = QPoint(100, 100);
        QVERIFY(pp.ppc.fromVariant(value));
        QCOMPARE(pp.ppc.value(), QPoint(100, 100));
        value = "sss";
        QVERIFY(!pp.ppc.fromVariant(value));
        QCOMPARE(pp.ppc.value(), QPoint(100, 100));

        QVERIFY(pp.szp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QSize);
        QCOMPARE(value.toSize(), QSize());
        value = QSize(72, 3);
        QVERIFY(pp.szp.fromVariant(value));
        QCOMPARE(pp.szp.value(), QSize(72, 3));
        value = "sss";
        QVERIFY(!pp.szp.fromVariant(value));
        QCOMPARE(pp.szp.value(), QSize(72, 3));

        QVERIFY(pp.szpc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QSize);
        QCOMPARE(value.toSize(), QSize(33, 21));
        value = QSize(6, 4);
        QVERIFY(pp.szpc.fromVariant(value));
        QCOMPARE(pp.szpc.value(), QSize(6, 4));
        value = "sss";
        QVERIFY(!pp.szpc.fromVariant(value));
        QCOMPARE(pp.szpc.value(), QSize(6, 4));

        QVERIFY(pp.ep.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Int);
        QCOMPARE(value.toInt(), (int)COLOR::BLUE);
        value = (int)COLOR::RED;
        QVERIFY(pp.ep.fromVariant(value));
        QCOMPARE(pp.ep.value(), (int)COLOR::RED);
        value = QSize(6, 4);
        QVERIFY(!pp.ep.fromVariant(value));
        QCOMPARE(pp.ep.value(), (int)COLOR::RED);

        QVERIFY(pp.epc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Int);
        QCOMPARE(value.toInt(), (int)COLOR::RED);
        value = (int)COLOR::YELLOW;
        QVERIFY(pp.epc.fromVariant(value));
        QCOMPARE(pp.epc.value(), (int)COLOR::YELLOW);
        value = QSize(6, 4);
        QVERIFY(!pp.epc.fromVariant(value));
        QCOMPARE(pp.epc.value(), (int)COLOR::YELLOW);

        QVERIFY(pp.efp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Int);
        QCOMPARE(value.toInt(), (int)(MASK::ONE|MASK::FOUR));
        value = (int)MASK::TWO;
        QVERIFY(pp.efp.fromVariant(value));
        QCOMPARE(pp.efp.value(), (int)MASK::TWO);
        value = QSize(6, 4);
        QVERIFY(!pp.efp.fromVariant(value));
        QCOMPARE(pp.efp.value(), (int)MASK::TWO);

        QVERIFY(pp.efpc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::Int);
        QCOMPARE(value.toInt(), (int)(MASK::ONE|MASK::FOUR));
        value = (int)MASK::TWO;
        QVERIFY(pp.efpc.fromVariant(value));
        QCOMPARE(pp.efpc.value(), (int)MASK::TWO);
        value = QSize(6, 4);
        QVERIFY(!pp.efpc.fromVariant(value));
        QCOMPARE(pp.efpc.value(), (int)MASK::TWO);

        QVERIFY(pp.cp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QColor);
        QCOMPARE(value.value<QColor>(), QColor(Qt::blue));
        value = QColor(200, 23, 44);
        QVERIFY(pp.cp.fromVariant(value));
        QCOMPARE(pp.cp.value(), QColor(200, 23, 44));
        value = QSize(6, 4);
        QVERIFY(!pp.cp.fromVariant(value));
        QCOMPARE(pp.cp.value(), QColor(200, 23, 44));

        QVERIFY(pp.cpc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QColor);
        QCOMPARE(value.value<QColor>(), QColor(Qt::red));
        value = QColor(0, 3, 3);
        QVERIFY(pp.cpc.fromVariant(value));
        QCOMPARE(pp.cpc.value(), QColor(0, 3, 3));
        value = QSize(6, 4);
        QVERIFY(!pp.cpc.fromVariant(value));
        QCOMPARE(pp.cpc.value(), QColor(0, 3, 3));

        QVERIFY(pp.fnp.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QFont);
        QCOMPARE(value.value<QFont>(), QFont("Courier", 10));
        value = QFont("Arial", 14);
        QVERIFY(pp.fnp.fromVariant(value));
        QCOMPARE(pp.fnp.value(),QFont("Arial", 14));
        value = QSize(6, 4);
        QVERIFY(!pp.fnp.fromVariant(value));
        QCOMPARE(pp.fnp.value(), QFont("Arial", 14));

        QVERIFY(pp.fnpc.toVariant(value));
        QCOMPARE(value.userType(), (int)QMetaType::QFont);
        QCOMPARE(value.value<QFont>(), QFont("Arial", 19));
        value = QFont("Courier", 9);
        QVERIFY(pp.fnpc.fromVariant(value));
        QCOMPARE(pp.fnpc.value(), QFont("Courier", 9));
        value = QSize(6, 4);
        QVERIFY(!pp.fnpc.fromVariant(value));
        QCOMPARE(pp.fnpc.value(), QFont("Courier", 9));
    }
}

void TestProperty::checkPropertyStateIsNonSimple(const PropertyBase* changedProperty, const PropertyBase* firedProperty, PropertyChangeReason reason, PropertyValuePtr newValue)
{
    QCOMPARE(changedProperty, firedProperty);
    QCOMPARE(reason, PropertyChangeReasonStateLocal);
    auto state = CastPropertyValue<PropertyState>(newValue);
    QVERIFY(state);
    QCOMPARE(*state, PropertyStateNonSimple);
}
