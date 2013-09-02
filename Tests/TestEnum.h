#ifndef TEST_ENUM_H
#define TEST_ENUM_H
/*
#include "Property.h"
#include "Enum.h"
#include "PropertyCore.h"
#include "PEG/test.peg.h"
#include <QtTest/QtTest>
*/
#include <QObject>

class TestEnum: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestEnum() {}

private Q_SLOTS:

    void enumValue();
    void forEachEnumValue();
    void find();
    void state();
};

#endif // TEST_ENUM_H
