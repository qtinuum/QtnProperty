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
    void propertySet();
    void serializationState();
    void serializationChildren();
    void serializationValue();
    void createNew();
    void createCopy();
    void copyValues();
    void propertyAssignment();
    void propertyScripting();
    void variantConversions();
    void stringConversions();

public Q_SLOTS:

    void checkPropertyStateIsNonSimple(const Qtinuum::PropertyBase* changedProperty, const Qtinuum::PropertyBase* firedProperty, Qtinuum::PropertyChangeReason reason, Qtinuum::PropertyValuePtr newValue);
};

#endif // TEST_PROPERTY_H
