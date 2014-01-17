#include "TestGeneratedProperty.h"
#include "PEG/test.peg.h"
#include <QtTest/QtTest>

void TestGeneratedProperty::test1()
{
    QtnPropertySetTest1 p;
    QVERIFY(p.name() == "Test1");

    p.a = 2;
    QVERIFY(p.a == 2);
    p.a = 15;
    QVERIFY(p.a.maxValue() == 10);
    QVERIFY(p.a == 2);

    p.a.incrementValue();
    QVERIFY(p.a == 1);
    QVERIFY(p.a > 0 && p.a >= 0 && p.a < 2 && p.a <= 1 && p.a != 3);

    QVERIFY(p.text == "#^{};");
    QVERIFY(p.text.description() == "defrf\"sde\"""deerf3rfderf r g\r\nreg r{}dfrgergfwrewre");

    p.text = tr("new value");
    QVERIFY(p.text == "new value");
}

void TestGeneratedProperty::test2()
{
    QtnPropertySetTest3 n(this);
    n.setName("SS");

    QVERIFY(n.name() == "SS");
    QVERIFY(n.yy.rect == QRect(10, 10, 10, 10));
    QVERIFY(!n.s.a);
    n.s.a = true;
    QVERIFY(n.s.a);
    QVERIFY(n.yy.rect == QRect(1, 1, 1, 1));

    QVERIFY(n.ww == n.bc && n.bc == false);
    n.ww = true;
    QVERIFY(n.ww == n.bc && n.ww == true);
    n.bc = false;
    QVERIFY(n.bc);
    n.m_s = false;
    n.bc = true;
    QVERIFY(n.m_s);
}

void TestGeneratedProperty::testAllPropertyTypes()
{
    QtnPropertySetAllPropertyTypes p;

    QVERIFY(p.ep == COLOR::BLUE);
    p.ep = COLOR::RED;
    switch (p.ep)
    {
    case COLOR::BLUE:
    case COLOR::YELLOW:
        QFAIL("ep expected as COLOR::RED");
        break;

    case COLOR::RED:
        break;

    default:
        QFAIL("ep expected as COLOR::RED");
    }
}
