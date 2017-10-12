/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "QtnProperty/Property.h"
#include "PropertyMacro.h"
#include <limits>
#include <functional>

template <typename T, typename EqPred = std::equal_to<T> >
class QtnSinglePropertyBase
	: public QtnProperty
{
public:
	typedef T ValueType;
	using ValueTypeStore =
			typename std::remove_const<
				typename std::remove_reference<ValueType>::type>::type;

	inline ValueType value() const { return valueImpl(); }

	inline bool edit(ValueType newValue)
	{
		return setValue(newValue, true);
	}

	bool setValue(ValueType newValue, bool edit = false)
	{
		if (!valueIsHidden() && isValueEqualImpl(newValue))
			return true;

		if (!isValueAcceptedImpl(newValue))
			return false;

		bool accept = true;
		emit propertyValueAccept(QtnPropertyValuePtr(&newValue), &accept);

		if (!accept)
			return false;

		QtnPropertyChangeReason reason = QtnPropertyChangeReasonNewValue;
		if (edit)
			reason |= QtnPropertyChangeReasonEditValue;

		emit propertyWillChange(reason,
								QtnPropertyValuePtr(&newValue),
								qMetaTypeId<ValueTypeStore>());
		setValueImpl(newValue);
		emit propertyDidChange(reason);

		return true;
	}

	inline operator ValueType() const
	{
		return value();
	}

	inline QtnSinglePropertyBase<ValueType> &operator=(ValueType newValue)
	{
		setValue(newValue);
		return *this;
	}

	inline QtnSinglePropertyBase<ValueType> &operator=(
		const QtnSinglePropertyBase<ValueType> &newValue)
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
	virtual bool isValueAcceptedImpl(ValueType) { return true; }

	virtual bool isValueEqualImpl(ValueType valueToCompare)
	{
		return EqPred() (valueToCompare, value());
	}

	// serialization implementation
	virtual bool loadImpl(QDataStream &stream) override
	{
		if (!QtnProperty::loadImpl(stream))
			return false;

		auto newValue = ValueTypeStore();
		stream >> newValue;

		if (stream.status() != QDataStream::Ok)
			return false;

		emit propertyWillChange(QtnPropertyChangeReasonLoadedValue,
								QtnPropertyValuePtr(&newValue),
								qMetaTypeId<ValueTypeStore>());
		setValueImpl(newValue);
		emit propertyDidChange(QtnPropertyChangeReasonLoadedValue);

		return stream.status() == QDataStream::Ok;
	}

	virtual bool saveImpl(QDataStream &stream) const override
	{
		if (!QtnProperty::saveImpl(stream))
			return false;

		stream << value();

		return stream.status() == QDataStream::Ok;
	}

	// variant conversion implementation
	virtual bool fromVariantImpl(const QVariant &var, bool edit) override
	{
		if (var.canConvert<ValueTypeStore>())
			return setValue(var.value<ValueTypeStore>(), edit);
		else
			return false;
	}

	virtual bool toVariantImpl(QVariant &var) const override
	{
		var.setValue<ValueTypeStore>(value());
		return var.isValid();
	}

private:
	QtnSinglePropertyBase(const QtnSinglePropertyBase &) Q_DECL_EQ_DELETE;
};

template <typename QtnSinglePropertyType>
class QtnSinglePropertyValue : public QtnSinglePropertyType
{
public:
	typedef typename QtnSinglePropertyType::ValueType ValueType;
	typedef typename QtnSinglePropertyType::ValueTypeStore ValueTypeStore;

protected:
	explicit QtnSinglePropertyValue(QObject *parent)
		: QtnSinglePropertyType(parent)
		, m_value(ValueTypeStore())
	{
	}

	ValueType valueImpl() const override { return m_value; }

	void setValueImpl(ValueType newValue) override { m_value = newValue; }

private:
	ValueTypeStore m_value;

	Q_DISABLE_COPY(QtnSinglePropertyValue)
};

template <typename QtnSinglePropertyType>
class QtnSinglePropertyCallback : public QtnSinglePropertyType
{
public:
	typedef typename QtnSinglePropertyType::ValueType ValueType;
	typedef typename QtnSinglePropertyType::ValueTypeStore ValueTypeStore;

	typedef std::function<ValueType()> CallbackValueGet;
	typedef std::function<void (ValueType)> CallbackValueSet;
	typedef std::function<bool (ValueType)> CallbackValueAccepted;
	typedef std::function<bool (ValueType)> CallbackValueEqual;

	inline void setCallbackValueGet(const CallbackValueGet &callback)
	{
		m_callbackValueGet = callback;
	}

	inline void setCallbackValueSet(const CallbackValueSet &callback)
	{
		m_callbackValueSet = callback;
	}

	inline void setCallbackValueAccepted(const CallbackValueAccepted &callback)
	{
		m_callbackValueAccepted = callback;
	}

	inline void setCallbackValueEqual(const CallbackValueEqual &callback)
	{
		m_callbackValueEqual = callback;
	}

protected:
	explicit QtnSinglePropertyCallback(QObject *parent)
		: QtnSinglePropertyType(parent)
	{
	}

	virtual ValueType valueImpl() const override
	{
		//Q_ASSERT(m_callbackValueGet);
		return m_callbackValueGet();
	}

	virtual void setValueImpl(ValueType newValue) override
	{
		Q_ASSERT(m_callbackValueSet);
		if (m_callbackValueSet)
			m_callbackValueSet(newValue);
	}

	virtual bool isValueAcceptedImpl(ValueType valueToAccept) override
	{
		if (m_callbackValueAccepted)
			return m_callbackValueAccepted(valueToAccept);

		return QtnSinglePropertyType::isValueAcceptedImpl(valueToAccept);
	}

	virtual bool isValueEqualImpl(ValueType valueToCompare) override
	{
		if (m_callbackValueEqual)
			return m_callbackValueEqual(valueToCompare);

		return QtnSinglePropertyType::isValueEqualImpl(valueToCompare);
	}

private:
	Q_DISABLE_COPY(QtnSinglePropertyCallback)

	CallbackValueGet m_callbackValueGet;
	CallbackValueSet m_callbackValueSet;
	CallbackValueAccepted m_callbackValueAccepted;
	CallbackValueEqual m_callbackValueEqual;
};

template <typename QtnSinglePropertyType>
class QtnNumericPropertyBase : public QtnSinglePropertyType
{
public:
	typedef typename QtnSinglePropertyType::ValueType ValueType;

	inline ValueType defaultValue() const
	{
		return m_defaultValue;
	}

	inline void setDefaultValue(ValueType defaultValue)
	{
		m_defaultValue = defaultValue;
	}

	inline ValueType minValue() const
	{
		return m_minValue;
	}

	inline void setMinValue(ValueType minValue)
	{
		m_minValue = minValue;
		m_maxValue = std::max(m_maxValue, m_minValue);
		correctValue();
	}

	inline ValueType maxValue() const
	{
		return m_maxValue;
	}

	inline void setMaxValue(ValueType maxValue)
	{
		m_maxValue = maxValue;
		m_minValue = std::min(m_minValue, m_maxValue);
		correctValue();
	}

	inline ValueType stepValue() const
	{
		return m_stepValue;
	}

	inline void setStepValue(ValueType stepValue)
	{
		m_stepValue = stepValue;
	}

	inline void incrementValue(int steps = 1)
	{
		ValueType newValue = this->value() + (stepValue() * (ValueType) steps);
		this->setValue(newValue);
	}

protected:
	explicit QtnNumericPropertyBase(QObject *parent)
		: QtnSinglePropertyType(parent)
		, m_defaultValue(ValueType(0))
		, m_minValue(std::numeric_limits<ValueType>::lowest())
		, m_maxValue(std::numeric_limits<ValueType>::max())
		, m_stepValue(ValueType(1))
	{
	}

	virtual bool isValueAcceptedImpl(ValueType valueToAccept) override
	{
		if (valueToAccept < m_minValue)
			return false;

		if (valueToAccept > m_maxValue)
			return false;

		return true;
	}

	inline void correctValue()
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
class QtnNumericPropertyValue
	: public QtnNumericPropertyBase<QtnSinglePropertyType>
{
public:
	using ValueType =
			typename QtnNumericPropertyBase<QtnSinglePropertyType>::ValueType;

protected:
	explicit QtnNumericPropertyValue(QObject *parent)
		: QtnNumericPropertyBase<QtnSinglePropertyType>(parent)
		, m_value(ValueType(0))
	{
	}

	inline ValueType valueImpl() const override
	{
		return m_value;
	}

	inline void setValueImpl(ValueType newValue) override
	{
		m_value = newValue;
	}

private:
	ValueType m_value;

	Q_DISABLE_COPY(QtnNumericPropertyValue)
};

#endif	// PROPERTYBASIS_H
