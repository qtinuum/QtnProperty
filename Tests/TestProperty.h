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
    void propertyPen();
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
    void qObjectProperty();
    void qObjectPropertySet();

public Q_SLOTS:

    void checkPropertyStateIsNonSimple(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue);
};

#endif // TEST_PROPERTY_H
