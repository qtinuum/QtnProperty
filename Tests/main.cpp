#include "TestProperty.h"
#include "TestGeneratedProperty.h"
#include "TestEnum.h"
#include <QtTest/QtTest>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    int result = 0;

    QList<const QMetaObject *> tests;

    // register tests
    tests.append(&TestProperty::staticMetaObject);
    tests.append(&TestGeneratedProperty::staticMetaObject);
    tests.append(&TestEnum::staticMetaObject);

    // run tests
    foreach (const QMetaObject *testMetaObject, tests)
    {
        QScopedPointer<QObject> test(testMetaObject->newInstance());
        Q_ASSERT(test);

        if (test)
        {
            result |= QTest::qExec(test.data(), argc, argv);
        }
    }

    return result;
}

