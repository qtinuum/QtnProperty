#ifndef TEST_GENERATED_PROPERTY_H
#define TEST_GENERATED_PROPERTY_H

#include <QObject>

static bool ret_true()
{
    return true;
}

class TestGeneratedProperty: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestGeneratedProperty() {}

private Q_SLOTS:

    void test1();
    void test2();
    void testAllPropertyTypes();
};

#endif // TEST_GENERATED_PROPERTY_H
