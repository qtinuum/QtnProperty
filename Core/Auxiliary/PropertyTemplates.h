/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef PROPERTYBASIS_H
#define PROPERTYBASIS_H

#include "../Property.h"
#include "PropertyMacro.h"
#include <limits>
#include <functional>

template<typename T, typename EqPred = std::equal_to<T> >
class QtnSinglePropertyBase: public QtnProperty
{
 public:
    typedef T ValueType;
    typedef typename std::remove_const<typename std::remove_reference<ValueType>::type>::type ValueTypeStore;

    ValueType value() const { return valueImpl(); }
    bool setValue(ValueType newValue)
    {
        if (isValueEqualImpl(newValue))
            return false;

        if (!isValueAcceptedImpl(newValue))
            return false;

        bool accept = true;
        Q_EMIT propertyValueAccept(this, QtnPropertyValuePtr(&newValue), &accept);
        if (!accept)
            return false;

        Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonNewValue, QtnPropertyValuePtr(&newValue));
        setValueImpl(newValue);
        Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonNewValue);

        return true;
    }

    operator ValueType() const
    {
        return value();
    }
    QtnSinglePropertyBase<ValueType>& operator=(ValueType newValue)
    {
        setValue(newValue);
        return *this;
    }
    QtnSinglePropertyBase<ValueType>& operator=(const QtnSinglePropertyBase<ValueType>& newValue)
    {
        setValue(newValue.value());
        return *this;
    }

protected:
    explicit QtnSinglePropertyBase(QObject *parent)
        : QtnProperty(parent)
    {
    }

    virtual ValueType valueImpl() const = 0;
    virtual void setValueImpl(ValueType newValue) = 0;
    virtual bool isValueAcceptedImpl(ValueType valueToAccept) { return true; }
    virtual bool isValueEqualImpl(ValueType valueToCompare) { return EqPred()(valueToCompare, value()); }

    // serialization implementation
    bool loadImpl(QDataStream& stream) override
    {
        if (!QtnProperty::loadImpl(stream))
            return false;

        ValueType newValue = ValueType();
        stream >> newValue;

        if (stream.status() != QDataStream::Ok)
            return false;

        Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonLoadedValue, QtnPropertyValuePtr(&newValue));
        setValueImpl(newValue);
        Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonLoadedValue);

        return stream.status() == QDataStream::Ok;
    }

    bool saveImpl(QDataStream& stream) const override
    {
        if (!QtnProperty::saveImpl(stream))
            return false;

        ValueType valueToSave = value();
        stream << valueToSave;

        return stream.status() == QDataStream::Ok;
    }

    // variant conversion implementation
    bool fromVariantImpl(const QVariant& var) override
    {
        if (var.canConvert<ValueType>())
            return setValue(var.value<ValueType>());
        else
            return false;
    }

    bool toVariantImpl(QVariant& var) const override
    {
        var.setValue<ValueType>(value());
        return var.isValid();
    }


private:
    QtnSinglePropertyBase(const QtnSinglePropertyBase&) Q_DECL_EQ_DELETE;
};

template<typename QtnSinglePropertyType>
class QtnSinglePropertyValue: public QtnSinglePropertyType
{
public:
    typedef typename QtnSinglePropertyType::ValueType ValueType;
    typedef typename QtnSinglePropertyType::ValueTypeStore ValueTypeStore;

protected:
    explicit QtnSinglePropertyValue(QObject *parent)
        : QtnSinglePropertyType(parent),
          m_value(ValueTypeStore())
    {
    }

    ValueType valueImpl() const override { return m_value; }
    void setValueImpl(ValueType newValue) override { m_value = newValue; }

private:
    ValueTypeStore m_value;

    Q_DISABLE_COPY(QtnSinglePropertyValue)
};

template<typename QtnSinglePropertyType>
class QtnSinglePropertyCallback: public QtnSinglePropertyType
{
public:
    typedef typename QtnSinglePropertyType::ValueType ValueType;
    typedef typename QtnSinglePropertyType::ValueTypeStore ValueTypeStore;

    typedef std::function<ValueType()> CallbackValueGet;
    typedef std::function<void(ValueType)> CallbackValueSet;
    typedef std::function<bool(ValueType)> CallbackValueAccepted;
    typedef std::function<bool(ValueType)> CallbackValueEqual;

    void setCallbackValueGet(const CallbackValueGet& callback) { m_callbackValueGet = callback; }
    void setCallbackValueSet(const CallbackValueSet& callback) { m_callbackValueSet = callback; }
    void setCallbackValueAccepted(const CallbackValueAccepted& callback) { m_callbackValueAccepted = callback; }
    void setCallbackValueEqual(const CallbackValueEqual& callback) { m_callbackValueEqual = callback; }

protected:
    explicit QtnSinglePropertyCallback(QObject *parent)
        : QtnSinglePropertyType(parent)
    {
    }

    ValueType valueImpl() const override
    {
        //Q_ASSERT(m_callbackValueGet);
        return m_callbackValueGet();
    }
    void setValueImpl(ValueType newValue) override
    {
        Q_ASSERT(m_callbackValueSet);
        if (m_callbackValueSet)
            m_callbackValueSet(newValue);
    }
    bool isValueAcceptedImpl(ValueType valueToAccept) override
    {
        if (m_callbackValueAccepted)
            return m_callbackValueAccepted(valueToAccept);
        else
            return QtnSinglePropertyType::isValueAcceptedImpl(valueToAccept);
    }
    bool isValueEqualImpl(ValueType valueToCompare) override
    {
        if (m_callbackValueEqual)
            return m_callbackValueEqual(valueToCompare);
        else
            return QtnSinglePropertyType::isValueEqualImpl(valueToCompare);
    }

private:
    Q_DISABLE_COPY(QtnSinglePropertyCallback)

    CallbackValueGet m_callbackValueGet;
    CallbackValueSet m_callbackValueSet;
    CallbackValueAccepted m_callbackValueAccepted;
    CallbackValueEqual m_callbackValueEqual;
};

template<typename QtnSinglePropertyType>
class QtnNumericPropertyBase: public QtnSinglePropertyType
{
public:
    typedef typename QtnSinglePropertyType::ValueType ValueType;

    ValueType defaultValue() const { return m_defaultValue; }
    void setDefaultValue(ValueType defaultValue) { m_defaultValue = defaultValue; }

    ValueType minValue() const { return m_minValue; }
    void setMinValue(ValueType minValue)
    {
        m_minValue = minValue;
        m_maxValue = std::max(m_maxValue, m_minValue);
        correctValue();
    }

    ValueType maxValue() const { return m_maxValue; }
    void setMaxValue(ValueType maxValue)
    {
        m_maxValue = maxValue;
        m_minValue = std::min(m_minValue, m_maxValue);
        correctValue();
    }

    ValueType stepValue() const { return m_stepValue; }
    void setStepValue(ValueType stepValue) { m_stepValue = stepValue; }

    void incrementValue(int steps = 1)
    {
        ValueType newValue = this->value() + (stepValue() * (ValueType)steps);
        this->setValue(newValue);
    }

protected:
    explicit QtnNumericPropertyBase(QObject *parent)
        : QtnSinglePropertyType(parent),
          m_defaultValue(ValueType(0)),
          m_minValue(std::numeric_limits<ValueType>::lowest()),
          m_maxValue(std::numeric_limits<ValueType>::max()),
          m_stepValue(ValueType(1))
    {
    }

    bool isValueAcceptedImpl(ValueType valueToAccept) override
    {
        if (valueToAccept < m_minValue)
            return false;

        if (valueToAccept > m_maxValue)
            return false;

        return true;
    }

    void correctValue()
    {
        ValueType oldValue = this->value();
        ValueType newValue = oldValue;

        if (newValue < m_minValue)
            newValue = m_minValue;

        if (newValue > m_maxValue)
            newValue = m_maxValue;

        if (newValue != oldValue)
            this->setValue(newValue);
    }

private:
    ValueType m_defaultValue;
    ValueType m_minValue;
    ValueType m_maxValue;
    ValueType m_stepValue;

    Q_DISABLE_COPY(QtnNumericPropertyBase)
};

template <typename QtnSinglePropertyType>
class QtnNumericPropertyValue: public QtnNumericPropertyBase<QtnSinglePropertyType>
{
  public:
    typedef typename QtnNumericPropertyBase<QtnSinglePropertyType>::ValueType ValueType;

protected:
    explicit QtnNumericPropertyValue(QObject *parent)
        : QtnNumericPropertyBase<QtnSinglePropertyType>(parent),
          m_value(ValueType(0))
    {
    }

    ValueType valueImpl() const override { return m_value; }
    void setValueImpl(ValueType newValue) override { m_value = newValue; }

private:
    ValueType m_value;

    Q_DISABLE_COPY(QtnNumericPropertyValue)
};

#endif // PROPERTYBASIS_H
