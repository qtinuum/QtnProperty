#include "TestProperty.h"
#include "QtnProperty/QObjectPropertySet.h"
#include "PEG/test.peg.h"
#include <QtTest/QtTest>
#include <QtScript/QScriptEngine>

static bool ret_true()
{
	return true;
}

static void verifyInitialValues(QtnPropertySetAllPropertyTypes &pp)
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
	QVERIFY(pp.rpf == QRectF());
	QVERIFY(pp.rpfc == QRectF(10.1, 10.2, 10.3, 10.4));
	QVERIFY(pp.pp == QPoint());
	QVERIFY(pp.ppc == QPoint(9, 2));
	QVERIFY(pp.ppf == QPointF());
	QVERIFY(pp.ppfc == QPointF(9.9, 2.2));
	QVERIFY(pp.szp == QSize());
	QVERIFY(pp.szpc == QSize(33, 21));
	QVERIFY(pp.szpf == QSizeF());
	QVERIFY(pp.szpfc == QSizeF(33.0, 21.9));
	QVERIFY(pp.ep == COLOR::BLUE);
	QVERIFY(pp.epc == COLOR::RED);
	QVERIFY(pp.efp == (MASK::ONE | MASK::FOUR));
	QVERIFY(pp.efpc == (MASK::ONE | MASK::FOUR));
	QVERIFY(pp.cp == QColor(Qt::blue));
	QVERIFY(pp.cpc == QColor(Qt::red));
	QVERIFY(pp.fnp == QFont("Courier", 10));
	QVERIFY(pp.fnpc == QFont("Arial", 19));
}

static void modify(QtnPropertySetAllPropertyTypes &pp)
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
	pp.rpf = QRectF(12.3, 10.4, 16.5, 17.6);
	pp.rpfc = QRectF(4.1, 3.2, 1.3, 0.0);
	pp.pp = QPoint(21, 9);
	pp.ppc = QPoint(15, 0);
	pp.ppf = QPointF(21.21, 9.9);
	pp.ppfc = QPointF(15.15, 0.0);
	pp.szp = QSize(90, 87);
	pp.szpc = QSize(0, 1);
	pp.szpf = QSizeF(90.9, 87.7);
	pp.szpfc = QSizeF(0.1, 1.0);
	pp.ep = COLOR::RED;
	pp.epc = COLOR::BLUE;
	pp.efp = (MASK::FOUR);
	pp.efpc = (MASK::ONE);
	pp.cp = QColor(Qt::red);
	pp.cpc = QColor(Qt::blue);
	pp.fnp = QFont("Courier New", 11);
	pp.fnpc = QFont("Myfont", 0);
}

static void verifyModified(QtnPropertySetAllPropertyTypes &pp)
{
	QCOMPARE(pp.bp.value(), true);
	QCOMPARE(pp.bpc.value(), false);
	QCOMPARE(pp.ip.value(), 12);
	QCOMPARE(pp.ipc.value(), 2);
	QCOMPARE(pp.up.value(), 3u);
	QCOMPARE(pp.upc.value(), 32u);
	QCOMPARE(pp.fp.value(), 0.32f);
	QCOMPARE(pp.fpc.value(), 32.2f);
	QCOMPARE(pp.dp.value(), 12.);
	QCOMPARE(pp.dpc.value(), 2.4);
	QCOMPARE(pp.sp.value(), QString("test"));
	QCOMPARE(pp.spc.value(), QString("name#1"));
	QCOMPARE(pp.rp.value(), QRect(12, 10, 16, 17));
	QCOMPARE(pp.rpc.value(), QRect(4, 3, 1, 0));
	QCOMPARE(pp.rpf.value(), QRectF(12.3, 10.4, 16.5, 17.6));
	QCOMPARE(pp.rpfc.value(), QRectF(4.1, 3.2, 1.3, 0.0));
	QCOMPARE(pp.pp.value(), QPoint(21, 9));
	QCOMPARE(pp.ppc.value(), QPoint(15, 0));
	QCOMPARE(pp.ppf.value(), QPointF(21.21, 9.9));
	QCOMPARE(pp.ppfc.value(), QPointF(15.15, 0.0));
	QCOMPARE(pp.szp.value(), QSize(90, 87));
	QCOMPARE(pp.szpc.value(), QSize(0, 1));
	QCOMPARE(pp.szpf.value(), QSizeF(90.9, 87.7));
	QCOMPARE(pp.szpfc.value(), QSizeF(0.1, 1.0));
	QCOMPARE(pp.ep.value(), (QtnEnumValueType) COLOR::RED);
	QCOMPARE(pp.epc.value(), (QtnEnumValueType) COLOR::BLUE);
	QCOMPARE(pp.efp.value(), (QtnEnumFlagsValueType) MASK::FOUR);
	QCOMPARE(pp.efpc.value(), (QtnEnumFlagsValueType) MASK::ONE);
	QCOMPARE(pp.cp.value(), QColor(Qt::red));
	QCOMPARE(pp.cpc.value(), QColor(Qt::blue));
	QCOMPARE(pp.fnp.value(), QFont("Courier New", 11));
	QCOMPARE(pp.fnpc.value(), QFont("Myfont", 0));
}

void TestProperty::name()
{
	QtnPropertyInt p(this);
	p.setName(QString("PropertyName"));
	QCOMPARE(p.name(), QString("PropertyName"));

	QtnPropertySet p1(nullptr);
	p1.setName("AnotherName");
	QCOMPARE(p1.name(), QString("AnotherName"));
}

void TestProperty::description()
{
	QtnPropertyBool p(this);
	p.setDescription(QString("PropertyDescription"));
	QCOMPARE(p.description(), QString("PropertyDescription"));

	QtnPropertySet p1(nullptr);
	p1.setDescription("Another description\nwith multiline.");
	QCOMPARE(p1.description(), QString("Another description\nwith multiline."));
}

void TestProperty::id()
{
	QtnPropertySet p(nullptr);
	QCOMPARE(p.id(), QtnPropertyIDInvalid);
	p.setId(12);
	QCOMPARE(p.id(), 12);

	QtnPropertyUInt p1(&p);
	QCOMPARE(p1.id(), QtnPropertyIDInvalid);
	p1.setId(5);
	QCOMPARE(p1.id(), 5);
}

void TestProperty::state()
{
	QtnPropertySet ps(0);
	QtnPropertyInt p(&ps);
	ps.addChildProperty(&p);

	QCOMPARE(ps.state(), QtnPropertyStateNone);
	QCOMPARE(p.state(), QtnPropertyStateResettable);

	QtnPropertyState master_state =
		QtnPropertyStateInvisible | QtnPropertyStateNonSimple;

	ps.setState(master_state);
	QCOMPARE(ps.state(), master_state);
	QCOMPARE(ps.stateLocal(), master_state);
	QCOMPARE(ps.stateInherited(), QtnPropertyStateNone);
	QCOMPARE(p.state(), master_state | QtnPropertyStateResettable);
	QCOMPARE(p.stateLocal(), QtnPropertyStateResettable);
	QCOMPARE(p.stateInherited(), master_state);

	p.addState(QtnPropertyStateImmutable);
	QCOMPARE(ps.state(), master_state);
	QCOMPARE(ps.stateLocal(), master_state);
	QCOMPARE(ps.stateInherited(), QtnPropertyStateNone);
	QCOMPARE(p.state(),
		master_state | QtnPropertyStateResettable | QtnPropertyStateImmutable);
	QCOMPARE(
		p.stateLocal(), QtnPropertyStateResettable | QtnPropertyStateImmutable);
	QCOMPARE(p.stateInherited(), master_state);

	ps.removeState(QtnPropertyStateInvisible);
	QCOMPARE(ps.state(), QtnPropertyStateNonSimple);
	QCOMPARE(ps.stateLocal(), QtnPropertyStateNonSimple);
	QCOMPARE(ps.stateInherited(), QtnPropertyStateNone);
	QCOMPARE(p.state(),
		QtnPropertyStateNonSimple | QtnPropertyStateResettable |
			QtnPropertyStateImmutable);
	QCOMPARE(
		p.stateLocal(), QtnPropertyStateResettable | QtnPropertyStateImmutable);
	QCOMPARE(p.stateInherited(), QtnPropertyStateNonSimple);

	p.switchState(QtnPropertyStateResettable, false);
	ps.switchState(QtnPropertyStateInvisible, true);
	QCOMPARE(ps.state(), master_state);
	QCOMPARE(ps.stateLocal(), master_state);
	QCOMPARE(ps.stateInherited(), QtnPropertyStateNone);
	QCOMPARE(p.state(), master_state | QtnPropertyStateImmutable);
	QCOMPARE(p.stateLocal(), QtnPropertyStateImmutable);
	QCOMPARE(p.stateInherited(), master_state);

	QVERIFY(!ps.isEditableByUser());

	ps.removeState(master_state | QtnPropertyStateCollapsed);
	QVERIFY(ps.isEditableByUser());
	QCOMPARE(ps.state(), QtnPropertyStateNone);

	int call_count = 0;
	QObject::connect(&ps, &QtnPropertyBase::propertyWillChange,
		[&call_count](QtnPropertyChangeReason, QtnPropertyValuePtr, int) {
			++call_count;
		});

	QObject::connect(&ps, &QtnPropertyBase::propertyDidChange,
		[&call_count](QtnPropertyChangeReason) { ++call_count; });

	ps.addState(QtnPropertyStateNone, true);
	QCOMPARE(call_count, 2);

	ps.removeState(QtnPropertyStateCollapsed);
	QCOMPARE(call_count, 2);

	ps.removeState(QtnPropertyStateCollapsed, true);
	QCOMPARE(call_count, 4);
}

void TestProperty::stateChange()
{
	QtnPropertyFloat p(this);
	QObject::connect(&p, &QtnPropertyBase::propertyWillChange, this,
		&TestProperty::checkPropertyStateIsNonSimple);

	p.addState(QtnPropertyStateNonSimple);
}

void TestProperty::propertyDelegate()
{
	QtnPropertyBool p(this);
	QVERIFY(!p.delegateInfo());

	QtnPropertyDelegateInfo info;
	info.name = "delegateName";
	p.setDelegateInfo(info);
	QVERIFY(p.delegateInfo());
	QVERIFY(p.delegateInfo()->name == "delegateName");
	QVERIFY(p.delegateInfo()->attributes.isEmpty());

	info.attributes["int"] = 4;
	p.setDelegateInfo(info);
	QVERIFY(p.delegateInfo()->attributes.size() == 1);
	QVERIFY(p.delegateInfo()->attributes["int"] == 4);

	info.attributes["str"] = QString("Text");
	p.setDelegateInfo(info);
	QVERIFY(p.delegateInfo()->attributes.size() == 2);
	QVERIFY(p.delegateInfo()->attributes["str"] == "Text");

	info.attributes["int"] = 12;
	p.setDelegateInfo(info);
	QVERIFY(p.delegateInfo()->attributes.size() == 2);
	QVERIFY(p.delegateInfo()->attributes["int"] == 12);
}

void TestProperty::propertyDelegateCallback()
{
	QtnPropertyBool p(this);
	QVERIFY(!p.delegateInfo());

	p.setDelegateInfoCallback([]() -> QtnPropertyDelegateInfo {
		QtnPropertyDelegateInfo info;
		info.name = "delegate";
		info.attributes["one"] = 1;
		info.attributes["color"] = QColor(Qt::black);
		return info;
	});

	QVERIFY(p.delegateInfo());
	QVERIFY(p.delegateInfo()->name == "delegate");
	QVERIFY(p.delegateInfo()->attributes.size() == 2);
	QVERIFY(p.delegateInfo()->attributes["one"] == 1);
	QVERIFY(p.delegateInfo()->attributes["color"] == QColor(Qt::black));
}

void TestProperty::propertyBool()
{
	{
		QtnPropertyBoolCallback p(this);

		p.setCallbackValueGet([]() -> bool { return false; });
		QVERIFY(p.value() == false);

		p.setCallbackValueSet(
			[](bool newValue, QtnPropertyChangeReason /*reason*/) {
				QVERIFY(newValue == true);
			});

		p = true;

		p.setCallbackValueSet(
			[](bool newValue, QtnPropertyChangeReason /*reason*/) {
				QVERIFY(newValue == false);
			});
		p.setValue(false);

		QVERIFY2(!p, "p expected as false");

		p.setCallbackValueGet(&ret_true);
		QVERIFY2(p, "p expected as true");

		QtnPropertyBoolBase &p1 = p;
		QVERIFY2(p1, "p1 expected as true");

		QtnPropertyBoolCallback p2(this);
		bool val = true;

		p2.setCallbackValueGet([&val]() -> bool { return val; });

		p2.setCallbackValueSet(
			[&val](bool newValue, QtnPropertyChangeReason /*reason*/) {
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

		QtnPropertyBool p3(this);
		p2 = p3 = true;
		QVERIFY(p2);
	}

	{
		QtnPropertyBool p(this);
		QVERIFY2(!p, "p expected as false");

		p = true;
		QVERIFY(p == true);

		p = (p != true);
		QVERIFY(!p);

		switch (int(p) + 4)
		{
			case 4:
				break;
			default:
				QFAIL("p expected as false");
		}

		p = bool(5);
		QVERIFY(p);

		QVERIFY(p != false);
		QVERIFY(p == true);
		QVERIFY(p && (p > false));
		QVERIFY(p >= true);
		QVERIFY(!(p <= false));

		QtnPropertyBoolCallback p2(this);
		p2.setCallbackValueGet([]() -> bool { return false; });
		p = p2;
		QVERIFY(!p);
	}
}

void TestProperty::propertyInt()
{
	QtnPropertyInt p(this);
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
	QtnPropertyQString p(this);
	QVERIFY(p == QString(""));
	p = QString("hello");
	QVERIFY(p == QString("hello"));
	p = "latin string";
	QVERIFY(p == "latin string");

	const QString &val = p;
	QVERIFY(val == "latin string");
}

void TestProperty::propertyRect()
{
	QtnPropertyQRect p(this);
	QVERIFY(p == QRect());
	QRect rect(1, 2, 3, 4);
	p = rect;
	QVERIFY(p == rect);

	QtnPropertyQRectCallback pc(this);
	QRect pc_value;
	pc.setCallbackValueGet([&pc_value]() -> const QRect & { return pc_value; });
	pc.setCallbackValueSet(
		[&pc_value](const QRect &value, QtnPropertyChangeReason /*reason*/) {
			pc_value = value;
		});

	pc = QRect(10, 10, 10, 10);
	QCOMPARE(pc.value(), QRect(10, 10, 10, 10));

	pc = p;
	QVERIFY(pc.value() == p.value());
	QVERIFY(pc == p);

	QtnPropertyQRectBase *pb = &pc;
	*pb = QRect(3, 4, 5, 6);
	QVERIFY(pc.value() == pb->value() && (*pb) == QRect(3, 4, 5, 6));
}

void TestProperty::propertyEnum()
{
	QtnPropertyEnum p(this);
	p = COLOR::BLUE;
	QVERIFY(p != COLOR::BLUE);
	QVERIFY(!p.setValue(COLOR::RED));

	p.setEnumInfo(&COLOR::info());
	QVERIFY(p.setValue(COLOR::RED));
	QVERIFY(p == COLOR::RED);

	QtnPropertyEnumCallback pc(this);
	pc.setCallbackValueGet([]() -> QtnEnumValueType { return COLOR::YELLOW; });
	QVERIFY(pc == COLOR::YELLOW);
}

void TestProperty::propertyEnumFlags()
{
	QtnPropertyEnumFlags p(this);
	p.setEnumInfo(&MASK::info());
	p = MASK::ONE;
	//QVERIFY(p == MASK::ONE);
	p = MASK::TWO | MASK::FOUR;
	QVERIFY(p & MASK::FOUR);
	QVERIFY(!(p & MASK::ONE));
}

void TestProperty::propertyPen()
{
	{
		QtnPropertyQPenStyle p(this);
		p = Qt::SolidLine;
		QCOMPARE(p.value(), Qt::SolidLine);

		p = Qt::DashDotDotLine;
		QCOMPARE(p.value(), Qt::DashDotDotLine);
	}

	{
		QtnPropertyQPen p(this);

		{
			QPen pen(QBrush(QColor(10, 100, 200)), 10, Qt::DashDotLine,
				Qt::RoundCap, Qt::SvgMiterJoin);
			p = pen;
			QCOMPARE(p.value(), pen);
		}

		{
			QString str;
			QVERIFY(p.toStr(str));
			QCOMPARE(str,
				QString("#0a64c8, DashDotLine, 10, RoundCap, "
						"SvgMiterJoin"));
		}

		{
			QString str = "#0000ff, DashLine, 3, SquareCap, BevelJoin";
			QVERIFY(p.fromStr(str));

			QCOMPARE(p.value().color(), QColor(Qt::blue));
			QCOMPARE(p.value().width(), 3);
			QCOMPARE(p.value().style(), Qt::DashLine);
			QCOMPARE(p.value().capStyle(), Qt::SquareCap);
			QCOMPARE(p.value().joinStyle(), Qt::BevelJoin);
		}
	}
}

void TestProperty::propertySet()
{
	QtnPropertySet p(this);

	QtnPropertySet pp(&p);
	pp.setName("pp");
	p.addChildProperty(&pp);

	QtnPropertyBool b(&pp);
	b.setName("b");
	pp.addChildProperty(&b);

	QtnPropertyFloat f(nullptr);
	f.setName("f");
	p.addChildProperty(&f);

	QtnPropertyBool bb(&p);
	bb.setName("b");
	p.addChildProperty(&bb);

	QList<QtnPropertyBase *> res = p.findChildProperties("pp");
	QCOMPARE(res.size(), 1);
	QCOMPARE(res[0], &pp);

	res = p.findChildProperties("b");
	QCOMPARE(res.size(), 2);

	res = p.findChildProperties("b", Qt::FindDirectChildrenOnly);
	QCOMPARE(res.size(), 1);
	QCOMPARE(res[0], &bb);

	res = pp.findChildProperties("b");
	QCOMPARE(res.size(), 1);
	QCOMPARE(res[0], &b);

	res = p.findChildProperties("pp.b");
	QCOMPARE(res.size(), 1);
	QCOMPARE(res[0], &b);
}

static void testSerializationState(QtnPropertyBase &p)
{
	p.setState(QtnPropertyStateCollapsed);

	QByteArray data;

	{
		QDataStream s(&data, QIODevice::WriteOnly);
		QVERIFY(p.save(s));
	}

	p.setState(QtnPropertyStateImmutable);
	QCOMPARE(p.state(), QtnPropertyStateImmutable);

	{
		QDataStream s(&data, QIODevice::ReadOnly);
		QVERIFY(p.load(s));
	}

	QCOMPARE(p.state(), QtnPropertyStateCollapsed);
}

void TestProperty::serializationState()
{
	{
		QtnPropertyInt p(this);
		testSerializationState(p);
	}

	{
		QtnPropertySet p(nullptr);
		testSerializationState(p);
	}
}

void TestProperty::serializationChildren()
{
	QtnPropertySet ps(this);
	ps.setState(QtnPropertyStateCollapsed);

	QByteArray data;

	QtnProperty *p2(qtnCreateProperty<QtnPropertyInt>(&ps));
	p2->setId(1);
	p2->setState(QtnPropertyStateImmutable);

	QtnPropertyBool p3(&ps);
	p3.setId(2);
	p3.setState(QtnPropertyStateNonSerialized);
	ps.addChildProperty(&p3);

	{
		QDataStream s(&data, QIODevice::WriteOnly);
		s << ps;
		QCOMPARE(s.status(), QDataStream::Ok);
	}

	QCOMPARE(
		p2->state(), (QtnPropertyStateImmutable | QtnPropertyStateCollapsed));
	QCOMPARE(p3.state(),
		(QtnPropertyStateNonSerialized | QtnPropertyStateCollapsed));

	p2->removeState(QtnPropertyStateImmutable);
	p3.addState(QtnPropertyStateImmutable);

	QCOMPARE(p2->state(), QtnPropertyStateCollapsed);
	QCOMPARE(p3.state(),
		(QtnPropertyStateNonSerialized | QtnPropertyStateImmutable |
			QtnPropertyStateCollapsed));

	{
		QDataStream s(&data, QIODevice::ReadOnly);
		s >> ps;
		QCOMPARE(s.status(), QDataStream::Ok);
	}

	QCOMPARE(
		p2->state(), (QtnPropertyStateImmutable | QtnPropertyStateCollapsed));
	QCOMPARE(p3.state(),
		(QtnPropertyStateNonSerialized | QtnPropertyStateImmutable |
			QtnPropertyStateCollapsed));

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
	QtnPropertyBool p(this);
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

	QtnPropertySetAllPropertyTypes pp(this);

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
		QtnPropertySetAllPropertyTypes pp(this);

		verifyInitialValues(pp);

		modify(pp);

		QScopedPointer<QtnPropertySetAllPropertyTypes> pn;
		pn.reset(
			qobject_cast<QtnPropertySetAllPropertyTypes *>(pp.createNew(this)));
		QVERIFY(pn);
		QVERIFY(pn.data() != &pp);
		QCOMPARE(pn->parent(), this);

		verifyInitialValues(*pn);
	}
}

void TestProperty::createCopy()
{
	{
		QtnPropertySetAllPropertyTypes pp(this);

		verifyInitialValues(pp);

		modify(pp);

		QScopedPointer<QtnPropertySetAllPropertyTypes> pn;
		pn.reset(qobject_cast<QtnPropertySetAllPropertyTypes *>(
			pp.createCopy(this)));
		QVERIFY(pn);
		QVERIFY(pn.data() != &pp);
		QCOMPARE(pn->parent(), this);

		verifyModified(*pn);
	}
}

void TestProperty::copyValues()
{
	{
		QtnPropertySetAllPropertyTypes pp(this);
		verifyInitialValues(pp);
		modify(pp);

		QtnPropertySetAllPropertyTypes pp1(this);
		verifyInitialValues(pp1);
		pp1.copyValues(&pp, QtnPropertyStateNone);
		verifyModified(pp1);
	}

	{
		QtnPropertySetAllPropertyTypes pp(this);
		verifyInitialValues(pp);
		modify(pp);

		QtnPropertySetAllPropertyTypes pp1(this);
		verifyInitialValues(pp1);
		QCOMPARE(pp1.bp.value(), false);
		pp.bp.addState(QtnPropertyStateImmutable);
		QCOMPARE(pp.bp.state(), QtnPropertyStateImmutable);

		pp1.copyValues(&pp, QtnPropertyStateImmutable);
		QCOMPARE(pp1.bp.value(), false);

		pp1.copyValues(&pp);
		verifyModified(pp1);
	}
}

void TestProperty::propertyAssignment()
{
	{
		QtnPropertyDouble p1(this);
		QtnPropertyDouble p2(this);
		QtnPropertyDoubleCallback p3(this);
		double d = 54.32;

		p1 = 0.5;
		p2 = 1.3;
		p3.setCallbackValueGet([]() -> double { return 23.4; });
		p3.setCallbackValueSet(
			[&d](double v, QtnPropertyChangeReason /*reason*/) { d = v; });

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
		QtnPropertySetAllPropertyTypes pp(this);

		verifyInitialValues(pp);

		QtnPropertySetAllPropertyTypes pp1(this);

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
		QtnPropertyBool b(this);
		b.setName("isValid");
		b.setDescription("isValid property.");
		b.setId(15);

		QScriptEngine eng;
		qtnScriptRegisterPropertyTypes(&eng);
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

		b.addState(QtnPropertyStateImmutable);
		val = eng.evaluate("b.isEditable");
		QCOMPARE(val.toBool(), false);

		b.setState(QtnPropertyStateInvisible);
		val = eng.evaluate("b.isEditable");
		QCOMPARE(val.toBool(), true);
		val = eng.evaluate("b.isEditableByUser");
		QCOMPARE(val.toBool(), false);

		val = eng.evaluate("b.state");
		QCOMPARE(val.toUInt32(), 2u);

		val = eng.evaluate("b.state == QtnPropertyStateInvisible");
		QCOMPARE(val.toBool(), true);

		val = eng.evaluate("b.state == QtnPropertyStateNonSerialized");
		QCOMPARE(val.toBool(), false);

		val = eng.evaluate("b.value");
		QCOMPARE(val.toBool(), false);

		b = true;
		val = eng.evaluate("b.value");
		QCOMPARE(val.toBool(), true);

		val = eng.evaluate("b.value = false");
		QCOMPARE(b.value(), false);

		eng.evaluate("b.state = 0");
		QCOMPARE(b.state(), QtnPropertyStateInvisible);

		b.setState(QtnPropertyStateNone);
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
		eng.evaluate(
			"var f1 = function(reason) {"
			"  if (this.name != 'isValid') success = false;"
			"  if (this.state != QtnPropertyStateNone) success = false;"
			"  if (this.id != 15) success = false;"
			"  if (reason != QtnPropertyChangeReasonNewValue) success = false;"
			"  if (this.value != true) success = false;"
			"}");
		eng.evaluate("b.propertyDidChange.connect(b, f1);");
		eng.evaluate("b.value = true;");
		val = eng.evaluate("success");
		QCOMPARE(val.toBool(), true);

		eng.evaluate("b.propertyDidChange.disconnect(b, f1);");
		eng.evaluate("b.value = true;");
		val = eng.evaluate("success");
		QCOMPARE(val.toBool(), true);
	}
}

void TestProperty::variantConversions()
{
	{
		QtnPropertyBool b(this);
		QVariant value = true;

		QCOMPARE(b.value(), false);
		QVERIFY(b.fromVariant(value));
		QCOMPARE(b.value(), true);

		value = QRect();
		QVERIFY(!b.fromVariant(value));
		QCOMPARE(b.value(), true);

		QVERIFY(b.toVariant(value));
		QCOMPARE(value.toBool(), true);

		b = false;
		QVERIFY(b.toVariant(value));
		QCOMPARE(value.toBool(), false);
	}

	{
		QtnPropertySetAllPropertyTypes pp(this);
		QVariant value;
		bool ok = false;

		QVERIFY(pp.bp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Bool);
		QCOMPARE(value.toBool(), false);
		value = true;
		QVERIFY(pp.bp.fromVariant(value));
		QCOMPARE(pp.bp.value(), true);
		value = QPoint(3, 5);
		QVERIFY(!pp.bp.fromVariant(value));
		QCOMPARE(pp.bp.value(), true);

		QVERIFY(pp.bpc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Bool);
		QCOMPARE(value.toBool(), true);
		value = false;
		QVERIFY(pp.bpc.fromVariant(value));
		QCOMPARE(pp.bpc.value(), false);
		value = QPoint(3, 5);
		QVERIFY(!pp.bpc.fromVariant(value));
		QCOMPARE(pp.bpc.value(), false);

		QVERIFY(pp.ip.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Int);
		QCOMPARE(value.toInt(&ok), 0);
		QVERIFY(ok);
		value = 13;
		QVERIFY(pp.ip.fromVariant(value));
		QCOMPARE(pp.ip.value(), 13);
		value = QPoint(3, 5);
		QVERIFY(!pp.ip.fromVariant(value));
		QCOMPARE(pp.ip.value(), 13);

		QVERIFY(pp.ipc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Int);
		QCOMPARE(value.toInt(&ok), 12);
		QVERIFY(ok);
		value = -83;
		QVERIFY(pp.ipc.fromVariant(value));
		QCOMPARE(pp.ipc.value(), -83);
		value = QPoint(3, 5);
		QVERIFY(!pp.ipc.fromVariant(value));
		QCOMPARE(pp.ipc.value(), -83);

		QVERIFY(pp.up.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::UInt);
		QCOMPARE(value.toUInt(&ok), 0u);
		QVERIFY(ok);
		value = 98234;
		QVERIFY(pp.up.fromVariant(value));
		QCOMPARE(pp.up.value(), 98234u);
		value = QPoint(3, 5);
		QVERIFY(!pp.up.fromVariant(value));
		QCOMPARE(pp.up.value(), 98234u);

		QVERIFY(pp.upc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::UInt);
		QCOMPARE(value.toUInt(&ok), 9u);
		QVERIFY(ok);
		value = 76u;
		QVERIFY(pp.upc.fromVariant(value));
		QCOMPARE(pp.upc.value(), 76u);
		value = QPoint(3, 5);
		QVERIFY(!pp.upc.fromVariant(value));
		QCOMPARE(pp.upc.value(), 76u);

		QVERIFY(pp.fp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Float);
		QCOMPARE(value.toFloat(&ok), 0.f);
		QVERIFY(ok);
		value = 0.32f;
		QVERIFY(pp.fp.fromVariant(value));
		QCOMPARE(pp.fp.value(), 0.32f);
		value = QPoint(3, 5);
		QVERIFY(!pp.fp.fromVariant(value));
		QCOMPARE(pp.fp.value(), 0.32f);

		QVERIFY(pp.fpc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Float);
		QCOMPARE(value.toFloat(&ok), 0.2f);
		QVERIFY(ok);
		value = -0.32f;
		QVERIFY(pp.fpc.fromVariant(value));
		QCOMPARE(pp.fpc.value(), -0.32f);
		value = QPoint(3, 5);
		QVERIFY(!pp.fpc.fromVariant(value));
		QCOMPARE(pp.fpc.value(), -0.32f);

		QVERIFY(pp.dp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Double);
		QCOMPARE(value.toDouble(&ok), 0.);
		QVERIFY(ok);
		value = -23.4;
		QVERIFY(pp.dp.fromVariant(value));
		QCOMPARE(pp.dp.value(), -23.4);
		value = QPoint(3, 5);
		QVERIFY(!pp.dp.fromVariant(value));
		QCOMPARE(pp.dp.value(), -23.4);

		QVERIFY(pp.dpc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Double);
		QCOMPARE(value.toDouble(&ok), 32.4);
		QVERIFY(ok);
		value = 543.22;
		QVERIFY(pp.dpc.fromVariant(value));
		QCOMPARE(pp.dpc.value(), 543.22);
		value = QPoint(3, 5);
		QVERIFY(!pp.dpc.fromVariant(value));
		QCOMPARE(pp.dpc.value(), 543.22);

		pp.sp = QString("hello");
		QVERIFY(pp.sp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QString);
		QCOMPARE(value.toString(), QString("hello"));
		value = QString("world");
		QVERIFY(pp.sp.fromVariant(value));
		QCOMPARE(pp.sp.value(), QString("world"));
		value = QPoint(3, 5);
		QVERIFY(!pp.sp.fromVariant(value));
		QCOMPARE(pp.sp.value(), QString("world"));

		QVERIFY(pp.spc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QString);
		QCOMPARE(value.toString(), QString("name"));
		value = QString("another");
		QVERIFY(pp.spc.fromVariant(value));
		QCOMPARE(pp.spc.value(), QString("another"));
		value = QPoint(3, 5);
		QVERIFY(!pp.spc.fromVariant(value));
		QCOMPARE(pp.spc.value(), QString("another"));

		QVERIFY(pp.rp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QRect);
		QCOMPARE(value.toRect(), QRect());
		value = QRect(23, 4, 56, 76);
		QVERIFY(pp.rp.fromVariant(value));
		QCOMPARE(pp.rp.value(), QRect(23, 4, 56, 76));
		value = "sss";
		QVERIFY(!pp.rp.fromVariant(value));
		QCOMPARE(pp.rp.value(), QRect(23, 4, 56, 76));

		QVERIFY(pp.rpc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QRect);
		QCOMPARE(value.toRect(), QRect(10, 10, 10, 10));
		value = QRect(-2, 3, 23, 3);
		QVERIFY(pp.rpc.fromVariant(value));
		QCOMPARE(pp.rpc.value(), QRect(-2, 3, 23, 3));
		value = "sss";
		QVERIFY(!pp.rpc.fromVariant(value));
		QCOMPARE(pp.rpc.value(), QRect(-2, 3, 23, 3));

		QVERIFY(pp.rpf.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QRectF);
		QCOMPARE(value.toRectF(), QRectF());
		value = QRectF(23.1, 4.2, 56.3, 76.4);
		QVERIFY(pp.rpf.fromVariant(value));
		QCOMPARE(pp.rpf.value(), QRectF(23.1, 4.2, 56.3, 76.4));
		value = "sss";
		QVERIFY(!pp.rpf.fromVariant(value));
		QCOMPARE(pp.rpf.value(), QRectF(23.1, 4.2, 56.3, 76.4));

		QVERIFY(pp.rpfc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QRectF);
		QCOMPARE(value.toRectF(), QRectF(10.1, 10.2, 10.3, 10.4));
		value = QRectF(-2.7, 3.6, 23.5, 3.4);
		QVERIFY(pp.rpfc.fromVariant(value));
		QCOMPARE(pp.rpfc.value(), QRectF(-2.7, 3.6, 23.5, 3.4));
		value = "sss";
		QVERIFY(!pp.rpfc.fromVariant(value));
		QCOMPARE(pp.rpfc.value(), QRectF(-2.7, 3.6, 23.5, 3.4));

		QVERIFY(pp.pp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QPoint);
		QCOMPARE(value.toPoint(), QPoint());
		value = QPoint(-2, 3);
		QVERIFY(pp.pp.fromVariant(value));
		QCOMPARE(pp.pp.value(), QPoint(-2, 3));
		value = "sss";
		QVERIFY(!pp.pp.fromVariant(value));
		QCOMPARE(pp.pp.value(), QPoint(-2, 3));

		QVERIFY(pp.ppc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QPoint);
		QCOMPARE(value.toPoint(), QPoint(9, 2));
		value = QPoint(100, 100);
		QVERIFY(pp.ppc.fromVariant(value));
		QCOMPARE(pp.ppc.value(), QPoint(100, 100));
		value = "sss";
		QVERIFY(!pp.ppc.fromVariant(value));
		QCOMPARE(pp.ppc.value(), QPoint(100, 100));

		QVERIFY(pp.ppf.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QPointF);
		QCOMPARE(value.toPointF(), QPointF());
		value = QPointF(-2.2, 3.3);
		QVERIFY(pp.ppf.fromVariant(value));
		QCOMPARE(pp.ppf.value(), QPointF(-2.2, 3.3));
		value = "sss";
		QVERIFY(!pp.ppf.fromVariant(value));
		QCOMPARE(pp.ppf.value(), QPointF(-2.2, 3.3));

		QVERIFY(pp.ppfc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QPointF);
		QCOMPARE(value.toPointF(), QPointF(9.9, 2.2));
		value = QPointF(100.100, 100.100);
		QVERIFY(pp.ppfc.fromVariant(value));
		QCOMPARE(pp.ppfc.value(), QPointF(100.100, 100.100));
		value = "sss";
		QVERIFY(!pp.ppfc.fromVariant(value));
		QCOMPARE(pp.ppfc.value(), QPointF(100.100, 100.100));

		QVERIFY(pp.szp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QSize);
		QCOMPARE(value.toSize(), QSize());
		value = QSize(72, 3);
		QVERIFY(pp.szp.fromVariant(value));
		QCOMPARE(pp.szp.value(), QSize(72, 3));
		value = "sss";
		QVERIFY(!pp.szp.fromVariant(value));
		QCOMPARE(pp.szp.value(), QSize(72, 3));

		QVERIFY(pp.szpc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QSize);
		QCOMPARE(value.toSize(), QSize(33, 21));
		value = QSize(6, 4);
		QVERIFY(pp.szpc.fromVariant(value));
		QCOMPARE(pp.szpc.value(), QSize(6, 4));
		value = "sss";
		QVERIFY(!pp.szpc.fromVariant(value));
		QCOMPARE(pp.szpc.value(), QSize(6, 4));

		QVERIFY(pp.szpf.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QSizeF);
		QCOMPARE(value.toSizeF(), QSizeF());
		value = QSizeF(72.3, 3.2);
		QVERIFY(pp.szpf.fromVariant(value));
		QCOMPARE(pp.szpf.value(), QSizeF(72.3, 3.2));
		value = "sss";
		QVERIFY(!pp.szpf.fromVariant(value));
		QCOMPARE(pp.szpf.value(), QSizeF(72.3, 3.2));

		QVERIFY(pp.szpfc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QSizeF);
		QCOMPARE(value.toSizeF(), QSizeF(33.0, 21.9));
		value = QSizeF(6.3, 4.5);
		QVERIFY(pp.szpfc.fromVariant(value));
		QCOMPARE(pp.szpfc.value(), QSizeF(6.3, 4.5));
		value = "sss";
		QVERIFY(!pp.szpfc.fromVariant(value));
		QCOMPARE(pp.szpfc.value(), QSizeF(6.3, 4.5));

		QVERIFY(pp.ep.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Int);
		QCOMPARE(value.toInt(), (int) COLOR::BLUE);
		value = (int) COLOR::RED;
		QVERIFY(pp.ep.fromVariant(value));
		QCOMPARE(pp.ep.value(), (int) COLOR::RED);
		value = QSize(6, 4);
		QVERIFY(!pp.ep.fromVariant(value));
		QCOMPARE(pp.ep.value(), (int) COLOR::RED);

		QVERIFY(pp.epc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Int);
		QCOMPARE(value.toInt(), (int) COLOR::RED);
		value = (int) COLOR::YELLOW;
		QVERIFY(pp.epc.fromVariant(value));
		QCOMPARE(pp.epc.value(), (int) COLOR::YELLOW);
		value = QSize(6, 4);
		QVERIFY(!pp.epc.fromVariant(value));
		QCOMPARE(pp.epc.value(), (int) COLOR::YELLOW);

		QVERIFY(pp.efp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Int);
		QCOMPARE(value.toInt(), (int) (MASK::ONE | MASK::FOUR));
		value = (int) MASK::TWO;
		QVERIFY(pp.efp.fromVariant(value));
		QCOMPARE(pp.efp.value(), (int) MASK::TWO);
		value = QSize(6, 4);
		QVERIFY(!pp.efp.fromVariant(value));
		QCOMPARE(pp.efp.value(), (int) MASK::TWO);

		QVERIFY(pp.efpc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::Int);
		QCOMPARE(value.toInt(), (int) (MASK::ONE | MASK::FOUR));
		value = (int) MASK::TWO;
		QVERIFY(pp.efpc.fromVariant(value));
		QCOMPARE(pp.efpc.value(), (int) MASK::TWO);
		value = QSize(6, 4);
		QVERIFY(!pp.efpc.fromVariant(value));
		QCOMPARE(pp.efpc.value(), (int) MASK::TWO);

		QVERIFY(pp.cp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QColor);
		QCOMPARE(value.value<QColor>(), QColor(Qt::blue));
		value = QColor(200, 23, 44);
		QVERIFY(pp.cp.fromVariant(value));
		QCOMPARE(pp.cp.value(), QColor(200, 23, 44));
		value = QSize(6, 4);
		QVERIFY(!pp.cp.fromVariant(value));
		QCOMPARE(pp.cp.value(), QColor(200, 23, 44));

		QVERIFY(pp.cpc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QColor);
		QCOMPARE(value.value<QColor>(), QColor(Qt::red));
		value = QColor(0, 3, 3);
		QVERIFY(pp.cpc.fromVariant(value));
		QCOMPARE(pp.cpc.value(), QColor(0, 3, 3));
		value = QSize(6, 4);
		QVERIFY(!pp.cpc.fromVariant(value));
		QCOMPARE(pp.cpc.value(), QColor(0, 3, 3));

		QVERIFY(pp.fnp.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QFont);
		QCOMPARE(value.value<QFont>(), QFont("Courier", 10));
		value = QFont("Arial", 14);
		QVERIFY(pp.fnp.fromVariant(value));
		QCOMPARE(pp.fnp.value(), QFont("Arial", 14));
		value = QSize(6, 4);
		QVERIFY(!pp.fnp.fromVariant(value));
		QCOMPARE(pp.fnp.value(), QFont("Arial", 14));

		QVERIFY(pp.fnpc.toVariant(value));
		QCOMPARE(value.userType(), (int) QMetaType::QFont);
		QCOMPARE(value.value<QFont>(), QFont("Arial", 19));
		value = QFont("Courier", 9);
		QVERIFY(pp.fnpc.fromVariant(value));
		QCOMPARE(pp.fnpc.value(), QFont("Courier", 9));
		value = QSize(6, 4);
		QVERIFY(!pp.fnpc.fromVariant(value));
		QCOMPARE(pp.fnpc.value(), QFont("Courier", 9));
	}
}

void TestProperty::stringConversions()
{
	{
		QtnPropertyBool p(this);
		QString text;
		QVERIFY(p.toStr(text));
		QCOMPARE(text, QString("False"));

		QVERIFY(p.fromStr("true"));
		QCOMPARE(p.value(), true);

		QVERIFY(p.toStr(text));
		QCOMPARE(text, QString("True"));

		QVERIFY(!p.fromStr("Hello"));
		QCOMPARE(p.value(), true);
	}

	{
		QtnPropertySetAllPropertyTypes pp(this);
		QString str;
		//        bool ok = false;

		QVERIFY(pp.bp.toStr(str));
		QCOMPARE(str, QString("False"));
		QVERIFY(pp.bp.fromStr(QString("tRue")));
		QCOMPARE(pp.bp.value(), true);
		QVERIFY(pp.bp.toStr(str));
		QCOMPARE(str, QString("True"));
		QVERIFY(!pp.bp.fromStr("aaa"));
		QCOMPARE(pp.bp.value(), true);

		QVERIFY(pp.bpc.toStr(str));
		QCOMPARE(str, QString("True"));
		QVERIFY(pp.bpc.fromStr(QString("falSe")));
		QCOMPARE(pp.bpc.value(), false);
		QVERIFY(pp.bpc.toStr(str));
		QCOMPARE(str, QString("False"));
		QVERIFY(!pp.bpc.fromStr("bbb"));
		QCOMPARE(pp.bpc.value(), false);

		QVERIFY(pp.ip.toStr(str));
		QCOMPARE(str, QString("0"));
		QVERIFY(pp.ip.fromStr(QString("-23")));
		QCOMPARE(pp.ip.value(), -23);
		QVERIFY(pp.ip.toStr(str));
		QCOMPARE(str, QString("-23"));
		QVERIFY(!pp.ip.fromStr("93jms"));
		QCOMPARE(pp.ip.value(), -23);
		pp.ip.setMinValue(-2);
		QCOMPARE(pp.ip.value(), -2);
		QVERIFY(!pp.ip.fromStr("-32"));
		QCOMPARE(pp.ip.value(), -2);

		QVERIFY(pp.ipc.toStr(str));
		QCOMPARE(str, QString("12"));
		QVERIFY(pp.ipc.fromStr(QString("45")));
		QCOMPARE(pp.ipc.value(), 45);
		QVERIFY(pp.ipc.toStr(str));
		QCOMPARE(str, QString("45"));
		QVERIFY(!pp.ipc.fromStr("23.22"));
		QCOMPARE(pp.ipc.value(), 45);

		QVERIFY(pp.up.toStr(str));
		QCOMPARE(str, QString("0"));
		QVERIFY(pp.up.fromStr(QString("7829")));
		QCOMPARE(pp.up.value(), 7829u);
		QVERIFY(pp.up.toStr(str));
		QCOMPARE(str, QString("7829"));
		QVERIFY(!pp.up.fromStr("92ms"));
		QCOMPARE(pp.up.value(), 7829u);

		QVERIFY(pp.upc.toStr(str));
		QCOMPARE(str, QString("9"));
		QVERIFY(pp.upc.fromStr(QString("11")));
		QCOMPARE(pp.upc.value(), 11u);
		QVERIFY(pp.upc.toStr(str));
		QCOMPARE(str, QString("11"));
		QVERIFY(!pp.upc.fromStr("-233"));
		QCOMPARE(pp.upc.value(), 11u);
		pp.upc.setMaxValue(100);
		QCOMPARE(pp.upc.value(), 11u);
		QVERIFY(!pp.upc.fromStr("110"));
		QCOMPARE(pp.upc.value(), 11u);

		QVERIFY(pp.fp.toStr(str));
		QCOMPARE(str, QString("0"));
		QVERIFY(pp.fp.fromStr(QString("-23.2")));
		QCOMPARE(pp.fp.value(), -23.2f);
		QVERIFY(pp.fp.toStr(str));
		QCOMPARE(str, QString("-23.2"));
		QVERIFY(!pp.fp.fromStr("32.ws"));
		QCOMPARE(pp.fp.value(), -23.2f);

		QVERIFY(pp.fpc.toStr(str));
		QCOMPARE(str, QString("0.2"));
		QVERIFY(pp.fpc.fromStr(QString("0.002")));
		QCOMPARE(pp.fpc.value(), 0.002f);
		QVERIFY(pp.fpc.toStr(str));
		QCOMPARE(str, QString("0.002"));
		QVERIFY(!pp.fpc.fromStr("-s233"));
		QCOMPARE(pp.fpc.value(), 0.002f);

		QVERIFY(pp.dp.toStr(str));
		QCOMPARE(str, QString("0"));
		QVERIFY(pp.dp.fromStr(QString("9403.234")));
		QCOMPARE(pp.dp.value(), 9403.234);
		QVERIFY(pp.dp.toStr(str));
		QCOMPARE(str, QString("9403.234"));
		QVERIFY(!pp.dp.fromStr("94d03.234s"));
		QCOMPARE(pp.dp.value(), 9403.234);

		QVERIFY(pp.dpc.toStr(str));
		QCOMPARE(str, QString("32.4"));
		QVERIFY(pp.dpc.fromStr(QString("-92.34")));
		QCOMPARE(pp.dpc.value(), -92.34);
		QVERIFY(pp.dpc.toStr(str));
		QCOMPARE(str, QString("-92.34"));
		QVERIFY(!pp.dpc.fromStr("weee"));
		QCOMPARE(pp.dpc.value(), -92.34);

		QVERIFY(pp.sp.toStr(str));
		QCOMPARE(str, QString(""));
		QVERIFY(pp.sp.fromStr("Hello"));
		QCOMPARE(pp.sp.value(), QString("Hello"));
		QVERIFY(pp.sp.toStr(str));
		QCOMPARE(str, QString("Hello"));
		QVERIFY(pp.sp.fromStr("  \"Hello world \"  "));
		QCOMPARE(pp.sp.value(), QString("\"Hello world \""));

		QVERIFY(pp.spc.toStr(str));
		QCOMPARE(str, QString("name"));
		QVERIFY(pp.spc.fromStr(QString("Name")));
		QCOMPARE(pp.spc.value(), QString("Name"));
		QVERIFY(pp.spc.toStr(str));
		QCOMPARE(str, QString("Name"));

		QVERIFY(pp.rp.toStr(str));
		QCOMPARE(str, QString("QRect(0, 0, 0, 0)"));
		QVERIFY(pp.rp.fromStr(QString("QRect (-2, 3, 43, 45 )   ")));
		QCOMPARE(pp.rp.value(), QRect(-2, 3, 43, 45));
		QVERIFY(pp.rp.toStr(str));
		QCOMPARE(str, QString("QRect(-2, 3, 43, 45)"));
		QVERIFY(!pp.rp.fromStr("ddlwk,s"));
		QCOMPARE(pp.rp.value(), QRect(-2, 3, 43, 45));

		QVERIFY(pp.rpc.toStr(str));
		QCOMPARE(str, QString("QRect(10, 10, 10, 10)"));
		QVERIFY(pp.rpc.fromStr(QString("QRect(0, 0, 10, 10)")));
		QCOMPARE(pp.rpc.value(), QRect(0, 0, 10, 10));
		QVERIFY(pp.rpc.toStr(str));
		QCOMPARE(str, QString("QRect(0, 0, 10, 10)"));
		QVERIFY(!pp.rpc.fromStr("weee"));
		QCOMPARE(pp.rpc.value(), QRect(0, 0, 10, 10));

		QVERIFY(pp.rpf.toStr(str));
		QCOMPARE(str, QString("QRectF(0, 0, 0, 0)"));
		QVERIFY(pp.rpf.fromStr(QString("QRectF (-2.3, 3.2, 43.5, 45.6 )   ")));
		QCOMPARE(pp.rpf.value(), QRectF(-2.3, 3.2, 43.5, 45.6));
		QVERIFY(pp.rpf.toStr(str));
		QCOMPARE(str, QString("QRectF(-2.3, 3.2, 43.5, 45.6)"));
		QVERIFY(!pp.rpf.fromStr("ddlwk,s"));
		QCOMPARE(pp.rpf.value(), QRectF(-2.3, 3.2, 43.5, 45.6));

		QVERIFY(pp.rpfc.toStr(str));
		QCOMPARE(str, QString("QRectF(10.1, 10.2, 10.3, 10.4)"));
		QVERIFY(pp.rpfc.fromStr(QString("QRectF(0.3, 0.0, 10.1, 10.1)")));
		QCOMPARE(pp.rpfc.value(), QRectF(0.3, 0.0, 10.1, 10.10));
		QVERIFY(pp.rpfc.toStr(str));
		QCOMPARE(str, QString("QRectF(0.3, 0, 10.1, 10.1)"));
		QVERIFY(!pp.rpfc.fromStr("weee"));
		QCOMPARE(pp.rpfc.value(), QRectF(0.3, 0.0, 10.1, 10.1));

		QVERIFY(pp.pp.toStr(str));
		QCOMPARE(str, QString("QPoint(0, 0)"));
		QVERIFY(pp.pp.fromStr(QString("QPoint (-2, 3 )   ")));
		QCOMPARE(pp.pp.value(), QPoint(-2, 3));
		QVERIFY(pp.pp.toStr(str));
		QCOMPARE(str, QString("QPoint(-2, 3)"));
		QVERIFY(!pp.pp.fromStr("ddlwk,s"));
		QCOMPARE(pp.pp.value(), QPoint(-2, 3));

		QVERIFY(pp.ppc.toStr(str));
		QCOMPARE(str, QString("QPoint(9, 2)"));
		QVERIFY(pp.ppc.fromStr(QString("QPoint(-3, 20)")));
		QCOMPARE(pp.ppc.value(), QPoint(-3, 20));
		QVERIFY(pp.ppc.toStr(str));
		QCOMPARE(str, QString("QPoint(-3, 20)"));
		QVERIFY(!pp.ppc.fromStr("weee"));
		QCOMPARE(pp.ppc.value(), QPoint(-3, 20));

		QVERIFY(pp.ppf.toStr(str));
		QCOMPARE(str, QString("QPointF(0, 0)"));
		QVERIFY(pp.ppf.fromStr(QString("QPointF (-2.2, 3.3 )   ")));
		QCOMPARE(pp.ppf.value(), QPointF(-2.2, 3.3));
		QVERIFY(pp.ppf.toStr(str));
		QCOMPARE(str, QString("QPointF(-2.2, 3.3)"));
		QVERIFY(!pp.ppf.fromStr("ddlwk,s"));
		QCOMPARE(pp.ppf.value(), QPointF(-2.2, 3.3));

		QVERIFY(pp.ppfc.toStr(str));
		QCOMPARE(str, QString("QPointF(9.9, 2.2)"));
		QVERIFY(pp.ppfc.fromStr(QString("QPointF(-3.3, 20.20)")));
		QCOMPARE(pp.ppfc.value(), QPointF(-3.3, 20.20));
		QVERIFY(pp.ppfc.toStr(str));
		QCOMPARE(str, QString("QPointF(-3.3, 20.2)"));
		QVERIFY(!pp.ppfc.fromStr("weee"));
		QCOMPARE(pp.ppfc.value(), QPointF(-3.3, 20.20));

		QVERIFY(pp.szp.toStr(str));
		QCOMPARE(str, QString("QSize(-1, -1)"));
		QVERIFY(pp.szp.fromStr(QString("QSize (-2, 3 )   ")));
		QCOMPARE(pp.szp.value(), QSize(-2, 3));
		QVERIFY(pp.szp.toStr(str));
		QCOMPARE(str, QString("QSize(-2, 3)"));
		QVERIFY(!pp.szp.fromStr("ddlwk,s"));
		QCOMPARE(pp.szp.value(), QSize(-2, 3));

		QVERIFY(pp.szpc.toStr(str));
		QCOMPARE(str, QString("QSize(33, 21)"));
		QVERIFY(pp.szpc.fromStr(QString("QSize(-3, 20)")));
		QCOMPARE(pp.szpc.value(), QSize(-3, 20));
		QVERIFY(pp.szpc.toStr(str));
		QCOMPARE(str, QString("QSize(-3, 20)"));
		QVERIFY(!pp.szpc.fromStr("weee"));
		QCOMPARE(pp.szpc.value(), QSize(-3, 20));

		QVERIFY(pp.szpf.toStr(str));
		QCOMPARE(str, QString("QSizeF(-1, -1)"));
		QVERIFY(pp.szpf.fromStr(QString("QSizeF (-2.3, 3.4 )   ")));
		QCOMPARE(pp.szpf.value(), QSizeF(-2.3, 3.4));
		QVERIFY(pp.szpf.toStr(str));
		QCOMPARE(str, QString("QSizeF(-2.3, 3.4)"));
		QVERIFY(!pp.szpf.fromStr("ddlwk,s"));
		QCOMPARE(pp.szpf.value(), QSizeF(-2.3, 3.4));

		QVERIFY(pp.szpfc.toStr(str));
		QCOMPARE(str, QString("QSizeF(33, 21.9)"));
		QVERIFY(pp.szpfc.fromStr(QString("QSizeF(-3.1, 20.8)")));
		QCOMPARE(pp.szpfc.value(), QSizeF(-3.1, 20.8));
		QVERIFY(pp.szpfc.toStr(str));
		QCOMPARE(str, QString("QSizeF(-3.1, 20.8)"));
		QVERIFY(!pp.szpfc.fromStr("weee"));
		QCOMPARE(pp.szpfc.value(), QSizeF(-3.1, 20.8));

		QVERIFY(pp.ep.toStr(str));
		QCOMPARE(str, QString("BLUE"));
		QVERIFY(pp.ep.fromStr(QString(" Yellow   \t")));
		QCOMPARE(pp.ep.value(), (QtnEnumValueType) COLOR::YELLOW);
		QVERIFY(pp.ep.toStr(str));
		QCOMPARE(str, QString("YELLOW"));
		QVERIFY(!pp.ep.fromStr("ddlwk,s"));
		QCOMPARE(pp.ep.value(), (QtnEnumValueType) COLOR::YELLOW);

		QVERIFY(pp.epc.toStr(str));
		QCOMPARE(str, QString("RED"));
		QVERIFY(pp.epc.fromStr(QString("BlUe")));
		QCOMPARE(pp.epc.value(), (QtnEnumValueType) COLOR::BLUE);
		QVERIFY(pp.epc.toStr(str));
		QCOMPARE(str, QString("BLUE"));
		QVERIFY(!pp.epc.fromStr("COLOUR::Red"));
		QCOMPARE(pp.epc.value(), (QtnEnumValueType) COLOR::BLUE);

		QVERIFY(pp.efp.toStr(str));
		QCOMPARE(str, QString("ONE|FOUR"));
		QVERIFY(pp.efp.fromStr(QString(" 0   \t")));
		QCOMPARE(pp.efp.value(), 0);
		QVERIFY(pp.efp.toStr(str));
		QCOMPARE(str, QString("0"));
		QVERIFY(pp.efp.fromStr("Two"));
		QCOMPARE(pp.efp.value(), (QtnEnumFlagsValueType) MASK::TWO);
		QVERIFY(pp.efp.fromStr("Two | Four"));
		QCOMPARE(
			pp.efp.value(), (QtnEnumFlagsValueType)(MASK::TWO | MASK::FOUR));
		QVERIFY(!pp.efp.fromStr("Two&Four"));
		QCOMPARE(
			pp.efp.value(), (QtnEnumFlagsValueType)(MASK::TWO | MASK::FOUR));

		QVERIFY(pp.efpc.toStr(str));
		QCOMPARE(str, QString("ONE|FOUR"));
		QVERIFY(pp.efpc.fromStr(QString("Two")));
		QCOMPARE(pp.efpc.value(), (QtnEnumFlagsValueType) MASK::TWO);
		QVERIFY(pp.efpc.toStr(str));
		QCOMPARE(str, QString("TWO"));
		QVERIFY(!pp.efpc.fromStr("weee"));
		QCOMPARE(pp.efpc.value(), (QtnEnumFlagsValueType) MASK::TWO);

		QVERIFY(pp.cp.toStr(str));
		QCOMPARE(str, QString("#0000ff"));
		QVERIFY(pp.cp.fromStr(QString(" Red   \t")));
		QCOMPARE(pp.cp.value(), QColor(Qt::red));
		QVERIFY(pp.cp.toStr(str));
		QCOMPARE(str, QString("#ff0000"));
		QVERIFY(!pp.cp.fromStr("ddlwk,s"));
		QCOMPARE(pp.cp.value(), QColor(Qt::red));

		QVERIFY(pp.cpc.toStr(str));
		QCOMPARE(str, QString("#ff0000"));
		QVERIFY(pp.cpc.fromStr(QString("transparent")));
		QCOMPARE(pp.cpc.value(), QColor(Qt::transparent));
		QVERIFY(pp.cpc.toStr(str));
		QCOMPARE(str, QString("#00000000"));
		QVERIFY(!pp.cpc.fromStr("COLOUR::Red"));
		QCOMPARE(pp.cpc.value(), QColor(Qt::transparent));

		QVERIFY(pp.fnp.toStr(str));
		QCOMPARE(str, QString("Courier,10,-1,5,50,0,0,0,0,0"));
		QVERIFY(pp.fnp.fromStr(QString(" Arial,18,-1,5,50,0,0,0,0,0   \t")));
		QCOMPARE(pp.fnp.value(), QFont("Arial", 18));
		QVERIFY(pp.fnp.toStr(str));
		QCOMPARE(str, QString("Arial,18,-1,5,50,0,0,0,0,0"));

		QVERIFY(pp.fnpc.toStr(str));
		QCOMPARE(str, QString("Arial,19,-1,5,50,0,0,0,0,0"));
		QVERIFY(pp.fnpc.fromStr(QString("Mono,23")));
		QFont font;
		font.setFamily("Mono");
		font.setPointSize(23);
		QCOMPARE(pp.fnpc.value(), font);
		QVERIFY(pp.fnpc.toStr(str));
		QCOMPARE(str, QString("Mono,23,-1,5,50,0,0,0,0,0"));
	}

	{
		QtnPropertySetTest3 p(this);
		QCOMPARE(p.iis.a.value(), true);
		QVERIFY(p.fromStr("iis.a = false"));
		QCOMPARE(p.iis.a.value(), false);

		QCOMPARE(p.u.value(), true);
		QCOMPARE(p.yy.s.value(), QString(""));
		QCOMPARE(p.s.a.value(), false);
		QVERIFY(p.fromStr("u = False\r\nyy.s = \"new value\"\n  s.a=true"));
		QCOMPARE(p.u.value(), false);
		QCOMPARE(p.yy.s.value(), QString("new value"));
		QCOMPARE(p.s.a.value(), true);
	}

	{
		QtnPropertySet ps(this);

		QtnPropertyBool pb(&ps);
		pb.setName("AAAA");
		pb.setDisplayName("AA AA");
		pb.setValue(false);

		QtnPropertyInt pi(&ps);
		pi.setName("BBBB");
		pi.setDisplayName("BB BB");
		pi.setValue(12);

		{
			QString res;
			QVERIFY(ps.toStr(res));
			QVERIFY(ps.fromStr(res));
		}

		pb.addState(QtnPropertyStateInvisible);

		{
			QString res;
			QVERIFY(ps.toStr(res));
			QVERIFY(ps.fromStr(res));
		}
	}

	{
		QtnPropertySetTest12 ps(this);

		QString res;
		QVERIFY(ps.toStr(res));
		QVERIFY(ps.fromStr(res));
	}
}

void TestProperty::qObjectProperty()
{
	{
		QObject obj;
		obj.setObjectName("Item1");

		QtnProperty *p = qtnCreateQObjectProperty(&obj, "objectName");
		QVERIFY(p);
		QtnPropertyQStringBase *ps = qobject_cast<QtnPropertyQStringBase *>(p);
		QVERIFY(ps);
		QCOMPARE(ps->value(), QString("Item1"));
		QCOMPARE(ps->state(), QtnPropertyStateNone);
		ps->setValue("NewItemName");
		QCOMPARE(obj.objectName(), QString("NewItemName"));
		QCOMPARE(ps->value(), QString("NewItemName"));
	}
}

void TestProperty::qObjectPropertySet()
{
	{
		QObject obj;
		obj.setObjectName("name");

		QtnPropertySet *p = qtnCreateQObjectPropertySet(&obj);
		QVERIFY(p);
		QCOMPARE(p->name(), QString("name"));
		const QList<QtnPropertyBase *> &subPropertySets = p->childProperties();
		QCOMPARE(subPropertySets.size(), 1);

		QtnPropertySet *p1 = subPropertySets[0]->asPropertySet();
		QVERIFY(p1);
		QCOMPARE(p1->name(), QString("QObject"));
		QCOMPARE(p1->childProperties().size(), 1);
	}

	{
		QCoreApplication *app = QCoreApplication::instance();
		QVERIFY(app);

		QtnPropertySet *p = qtnCreateQObjectPropertySet(app);
		QVERIFY(p);
	}
}

void TestProperty::checkPropertyStateIsNonSimple(
	QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue, int typeId)
{
	QCOMPARE(typeId, qMetaTypeId<QtnPropertyState>());
	QCOMPARE(reason, QtnPropertyChangeReasonStateLocal);
	auto state = qtnCastPropertyValue<QtnPropertyState>(newValue);
	QVERIFY(state);
	QCOMPARE(*state, QtnPropertyStateNonSimple | QtnPropertyStateResettable);
}
