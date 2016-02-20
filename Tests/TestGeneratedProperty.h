#ifndef TEST_GENERATED_PROPERTY_H
#define TEST_GENERATED_PROPERTY_H

#include <QObject>

class TestGeneratedProperty: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestGeneratedProperty() {}

private Q_SLOTS:

    void test1();
    void test2();
    void testAllPropertyTypes();
    void testLoadSave();
    void testJson();
};

#endif // TEST_GENERATED_PROPERTY_H
