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
	QVERIFY(p.text.description() ==
		"defrf\"sde\""
		"deerf3rfderf r g\r\nreg r{}dfrgergfwrewre");

	p.text = QString("new value");
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
				s.setVersion(QDataStream::Qt_4_2);
				QVERIFY(allProperties.save(s));
			}

			QCOMPARE(data.toBase64(),
				QByteArray(
					"GYQCAAAEWwACAAAAAAAAAAACAAAADRmEAgAABBEAAgAAAAAAAAAAAgAAAA"
					"4ZhAIAAAALAAIAAAAAAAAAAAAAAAAPGYQCAAAACwACAAAAAAAAAAABAAAA"
					"EBmEAgAAAA4AAgAAAQAAAAAAAAAAAAAAABEZhAIAAAAOAAIAAAAAAAAAAA"
					"AAAAwAAAASGYQCAAAADgACAAABAAAAAAAAAAAAAAAAExmEAgAAAA4AAgAA"
					"AAAAAAAAAAAACQAAABQZhAIAAAAOAAIAAAEAAAAAAAAAAAAAAAAVGYQCAA"
					"AADgACAAAAAAAAAAA+"
					"TMzNAAAAFhmEAgAAABIAAgAAAQAAAAAAAAAAAAAAAAAAAAAXGYQCAAAAEg"
					"ACAAAAAAAAAABAQDMzMzMzMwAAABgZhAIAAAAOAAIAAAAAAAAAAP////"
					"8AAAAZGYQCAAAAFgACAAAAAAAAAAAAAAAIAG4AYQBtAGUAAAAaGYQCAAAA"
					"GgACAAAAAAAAAAAAAAAAAAAAAP//////////"
					"AAAAGxmEAgAAABoAAgAAAAAAAAAAAAAACgAAAAoAAAATAAAAEwAAABwZhA"
					"IAAAASAAIAAAAAAAAAAAAAAAAAAAAAAAAAHRmEAgAAABIAAgAAAAAAAAAA"
					"AAAACQAAAAIAAAAeGYQCAAAAEgACAAAAAAAAAAD//////////"
					"wAAAB8ZhAIAAAASAAIAAAAAAAAAAAAAACEAAAAVAAAAIBmEAgAAAA4AAgA"
					"AAAAAAAAAAAAAFgAAACEZhAIAAAAOAAIAAAAAAAAAAAAAAAoAAAAiGYQCA"
					"AAADgACAAAAAAAAAAAAAAAFAAAAIxmEAgAAAA4AAgAAAAAAAAAAAAAABQA"
					"AACQZhAIAAAAVAAIAAAAAAAAAAAH//wAAAAD//"
					"wAAAAAAJRmEAgAAABUAAgAAAAAAAAAAAf////"
					"8AAAAAAAAAAAAmGYQCAAAALQACAAAAAAAAAAAAAAAOAEMAbwB1AHIAaQBl"
					"AHJAJAAAAAAAAP////"
					"8FAQAyEAAAACcZhAIAAAApAAIAAAAAAAAAAAAAAAoAQQByAGkAYQBsQDMA"
					"AAAAAAD/////"
					"BQEAMhAAAAApGYQCAAAAGgACAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
					"AAKhmEAgAAABoAAgAAAAAAAAAAQCPMzMzMzM1AAZmZmZmZmgAAACsZhAIA"
					"AAAqAAIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
					"AAAAAALBmEAgAAACoAAgAAAAAAAAAAQCQzMzMzMzNAJGZmZmZmZkAkmZmZ"
					"mZmaQCTMzMzMzM0AAAAtGYQCAAAAGgACAAAAAAAAAAC/8AAAAAAAAL/"
					"wAAAAAAAAAAAALhmEAgAAABoAAgAAAAAAAAAAQECAAAAAAABANeZmZmZmZ"
					"v////"
					"8AAAAyGYQCAAAAJQACAAAAAAAAAAACAAAAMxmEAgAAAAsAAgAAAAAAAAAA"
					"Af//////////"));
			QCOMPARE(data.size(), 1122);

			{
				QDataStream s(&data, QIODevice::ReadOnly);
				s.setVersion(QDataStream::Qt_4_2);
				QVERIFY(allProperties.load(s));
			}

			QString result;
			QVERIFY(allProperties.toStr(result));

			QCOMPARE(result.size(), 1129);
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
	QCOMPARE(res.size(), 1633);
	res = d.toJson(QJsonDocument::Compact);
	QCOMPARE(res.size(), 959);

	QVERIFY(p.fromJson(o));
}
