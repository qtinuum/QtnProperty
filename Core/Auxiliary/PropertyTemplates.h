/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
 */

#ifndef PROPERTYBASIS_H
#define PROPERTYBASIS_H

#include "../Property.h"
#include "PropertyMacro.h"
#include <limits>
#include <functional>

namespace Qtinuum
{

template<typename T, typename EqPred = std::equal_to<T> >
class SinglePropertyBase: public Property
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
        Q_EMIT propertyValueAccept(this, PropertyValuePtr(&newValue), &accept);
        if (!accept)
            return false;

        Q_EMIT propertyWillChange(this, this, PropertyChangeReasonNewValue, PropertyValuePtr(&newValue));
        setValueImpl(newValue);
        Q_EMIT propertyDidChange(this, this, PropertyChangeReasonNewValue);

        return true;
    }

    operator ValueType() const
    {
        return value();
    }
    SinglePropertyBase<ValueType>& operator=(ValueType newValue)
    {
        setValue(newValue);
        return *this;
    }
    SinglePropertyBase<ValueType>& operator=(const SinglePropertyBase<ValueType>& newValue)
    {
        setValue(newValue.value());
        return *this;
    }

protected:
    explicit SinglePropertyBase(QObject *parent)
        : Property(parent)
    {
    }

    virtual ValueType valueImpl() const = 0;
    virtual void setValueImpl(ValueType newValue) = 0;
    virtual bool isValueAcceptedImpl(ValueType valueToAccept) { return true; }
    virtual bool isValueEqualImpl(ValueType valueToCompare) { return EqPred()(valueToCompare, value()); }

    // serialization implementation
    bool loadImpl(QDataStream& stream) override
    {
        if (!Property::loadImpl(stream))
            return false;

        ValueType newValue = ValueType();
        stream >> newValue;

        if (stream.status() != QDataStream::Ok)
            return false;

        Q_EMIT propertyWillChange(this, this, PropertyChangeReasonLoadedValue, PropertyValuePtr(&newValue));
        setValueImpl(newValue);
        Q_EMIT propertyDidChange(this, this, PropertyChangeReasonLoadedValue);

        return stream.status() == QDataStream::Ok;
    }

    bool saveImpl(QDataStream& stream) const override
    {
        if (!Property::saveImpl(stream))
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
    SinglePropertyBase(const SinglePropertyBase&) Q_DECL_EQ_DELETE;
};

template<typename SinglePropertyType>
class SinglePropertyValue: public SinglePropertyType
{
public:
    typedef typename SinglePropertyType::ValueType ValueType;
    typedef typename SinglePropertyType::ValueTypeStore ValueTypeStore;

protected:
    explicit SinglePropertyValue(QObject *parent)
        : SinglePropertyType(parent),
          m_value(ValueTypeStore())
    {
    }

    ValueType valueImpl() const override { return m_value; }
    void setValueImpl(ValueType newValue) override { m_value = newValue; }

private:
    ValueTypeStore m_value;

    Q_DISABLE_COPY(SinglePropertyValue)
};

template<typename SinglePropertyType>
class SinglePropertyCallback: public SinglePropertyType
{
public:
    typedef typename SinglePropertyType::ValueType ValueType;
    typedef typename SinglePropertyType::ValueTypeStore ValueTypeStore;

    typedef std::function<ValueType()> CallbackValueGet;
    typedef std::function<void(ValueType)> CallbackValueSet;
    typedef std::function<bool(ValueType)> CallbackValueAccepted;
    typedef std::function<bool(ValueType)> CallbackValueEqual;

    void setCallbackValueGet(const CallbackValueGet& callback) { m_callbackValueGet = callback; }
    void setCallbackValueSet(const CallbackValueSet& callback) { m_callbackValueSet = callback; }
    void setCallbackValueAccepted(const CallbackValueAccepted& callback) { m_callbackValueAccepted = callback; }
    void setCallbackValueEqual(const CallbackValueEqual& callback) { m_callbackValueEqual = callback; }

protected:
    explicit SinglePropertyCallback(QObject *parent)
        : SinglePropertyType(parent)
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
            return SinglePropertyType::isValueAcceptedImpl(valueToAccept);
    }
    bool isValueEqualImpl(ValueType valueToCompare) override
    {
        if (m_callbackValueEqual)
            return m_callbackValueEqual(valueToCompare);
        else
            return SinglePropertyType::isValueEqualImpl(valueToCompare);
    }

private:
    Q_DISABLE_COPY(SinglePropertyCallback)

    CallbackValueGet m_callbackValueGet;
    CallbackValueSet m_callbackValueSet;
    CallbackValueAccepted m_callbackValueAccepted;
    CallbackValueEqual m_callbackValueEqual;
};

template<typename SinglePropertyType>
class NumericPropertyBase: public SinglePropertyType
{
public:
    typedef typename SinglePropertyType::ValueType ValueType;

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
    explicit NumericPropertyBase(QObject *parent)
        : SinglePropertyType(parent),
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

    Q_DISABLE_COPY(NumericPropertyBase)
};

template <typename SinglePropertyType>
class NumericPropertyValue: public NumericPropertyBase<SinglePropertyType>
{
  public:
    typedef typename NumericPropertyBase<SinglePropertyType>::ValueType ValueType;

protected:
    explicit NumericPropertyValue(QObject *parent)
        : NumericPropertyBase<SinglePropertyType>(parent),
          m_value(ValueType(0))
    {
    }

    ValueType valueImpl() const override { return m_value; }
    void setValueImpl(ValueType newValue) override { m_value = newValue; }

private:
    ValueType m_value;

    Q_DISABLE_COPY(NumericPropertyValue)
};

} // end namespace Qtinuum

#endif // PROPERTYBASIS_H
