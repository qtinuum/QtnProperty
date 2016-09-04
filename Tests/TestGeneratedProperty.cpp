#include "TestGeneratedProperty.h"
#include "PEG/test.peg.h"
#include "PEG/test2.peg.h"
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

void TestGeneratedProperty::testLoadSave()
{
    {
        QtnPropertySetAllPropertyTypes p;
        QtnPropertySetA p2;

        {
            QtnPropertySet allProperties(nullptr);
            allProperties.addChildProperty(&p, false);
            allProperties.addChildProperty(&p2, false);

            QByteArray data;

            {
                QDataStream s(&data, QIODevice::WriteOnly);
                s.setVersion(10);
                QVERIFY(allProperties.save(s));
                //printf("QDataStream: %d\n", s.version());
            }

            //printf(data.toBase64().data());
            //QCOMPARE(data.toBase64(), QByteArray("GYQBAAADngABAAAAAAAAAAABAAAADRmEAQAAA1QAAQAAAAAAAAAAAQAAAA4ZhAEAAAALAAEAAAAIAAAAAAAAAAAPGYQBAAAACwABAAAACAAAAAABAAAAEBmEAQAAAA4AAQAAAAgAAAAAAAAAAAAAABEZhAEAAAAOAAEAAAAIAAAAAAAAAAwAAAASGYQBAAAADgABAAAACAAAAAAAAAAAAAAAExmEAQAAAA4AAQAAAAgAAAAAAAAACQAAABQZhAEAAAASAAEAAAAIAAAAAAAAAAAAAAAAAAAAFRmEAQAAABIAAQAAAAgAAAAAP8mZmaAAAAAAAAAWGYQBAAAAEgABAAAACAAAAAAAAAAAAAAAAAAAABcZhAEAAAASAAEAAAAIAAAAAEBAMzMzMzMzAAAAGBmEAQAAAA4AAQAAAAgAAAAA/////wAAABkZhAEAAAAWAAEAAAAIAAAAAAAAAAgAbgBhAG0AZQAAABoZhAEAAAAaAAEAAAAIAAAAAAAAAAAAAAAA//////////8AAAAbGYQBAAAAGgABAAAACAAAAAAAAAAKAAAACgAAABMAAAATAAAAHBmEAQAAABIAAQAAAAgAAAAAAAAAAAAAAAAAAAAdGYQBAAAAEgABAAAACAAAAAAAAAAJAAAAAgAAAB4ZhAEAAAASAAEAAAAIAAAAAP//////////AAAAHxmEAQAAABIAAQAAAAgAAAAAAAAAIQAAABUAAAAgGYQBAAAADgABAAAACAAAAAAAAAAWAAAAIRmEAQAAAA4AAQAAAAgAAAAAAAAACgAAACIZhAEAAAAOAAEAAAAIAAAAAAAAAAUAAAAjGYQBAAAADgABAAAACAAAAAAAAAAFAAAAJBmEAQAAABUAAQAAAAgAAAAAAf//AAAAAP//AAAAAAAlGYQBAAAAFQABAAAACAAAAAAB/////wAAAAAAAAAAACYZhAEAAAA/AAEAAAAIAAAAAAAAAA4AQwBvAHUAcgBpAGUAcv////9AJAAAAAAAAP////8FAAEAMhAAZAEAAAAAAAAAAAAAAAAAJxmEAQAAADsAAQAAAAgAAAAAAAAACgBBAHIAaQBhAGz/////QDMAAAAAAAD/////BQABADIQAGQBAAAAAAAAAAAAAAAAACgZhAEAAAAKAAEAAAAIAAAAAP////8AAAAyGYQBAAAAJQABAAAAAAAAAAABAAAAMxmEAQAAAAsAAQAAAAgAAAAAAf//////////"));
            //QCOMPARE(data.size(), 933);

            {
                QDataStream s(&data, QIODevice::ReadOnly);
                QVERIFY(allProperties.load(s));
            }

            QString result;
            QVERIFY(allProperties.toStr(result));

            QCOMPARE(result.size(), 925);
        }
    }
}

void TestGeneratedProperty::testJson()
{
    QtnPropertySetAllPropertyTypes p;

    QJsonObject o;
    QVERIFY(p.toJson(o));

    QJsonDocument d(o);
    auto res = d.toJson();
    QCOMPARE(res.size(), 1321);
    res = d.toJson(QJsonDocument::Compact);
    QCOMPARE(res.size(), 752);

    QVERIFY(p.fromJson(o));
}
