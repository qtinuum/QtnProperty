#ifndef TEST_PROPERTY_H
#define TEST_PROPERTY_H

#include "Property.h"
#include <QObject>

class TestProperty: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestProperty() {}

private Q_SLOTS:

    void name();
    void description();
    void id();
    void state();
    void stateChange();
    void propertyDelegate();
    void propertyDelegateCallback();
    void propertyBool();
    void propertyInt();
    void propertyString();
    void propertyRect();
    void propertyEnum();
    void propertyEnumFlags();
    void propertySerializationState();
    void propertySerializationChildren();
    void propertySerializationValue();

public Q_SLOTS:

    void checkPropertyStateIsNonSimple(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason, Qtinuum::PropertyValuePtr newValue);
};

#endif // TEST_PROPERTY_H
