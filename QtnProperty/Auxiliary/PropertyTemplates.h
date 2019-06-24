/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#ifndef PROPERTY_TEMPLATES_H
#define PROPERTY_TEMPLATES_H

#include "QtnProperty/Property.h"
#include "PropertyMacro.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <limits>
#include <functional>

template <typename T, typename EqPred = std::equal_to<T>>
class QtnSinglePropertyBase : public QtnProperty
{
public:
	typedef T ValueType;
	using ValueTypeStore = typename std::decay<ValueType>::type;

	inline ValueType value() const
	{
		return valueImpl();
	}

	bool setValue(ValueType newValue,
		QtnPropertyChangeReason reason = QtnPropertyChangeReason())
	{
		if ((reason & QtnPropertyChangeReasonEditValue) && !isEditableByUser())
		{
			return false;
		}

		if (!isWritable() || !isValueAcceptedImpl(newValue))
			return false;

		if (!(reason & QtnPropertyChangeReasonEditMultiValue) &&
			isValueEqualImpl(newValue))
		{
			return true;
		}

		bool accept = true;
		emit propertyValueAccept(QtnPropertyValuePtr(&newValue), &accept);

		if (!accept)
			return false;

		if (!(reason & QtnPropertyChangeReasonValue))
		{
			reason |= QtnPropertyChangeReasonNewValue;
		}

		emit propertyWillChange(reason, QtnPropertyValuePtr(&newValue),
			qMetaTypeId<ValueTypeStore>());
		setValueImpl(newValue, reason);
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

	inline bool readDefaultValue(ValueTypeStore &to) const
	{
		return defaultValueImpl(to);
	}

protected:
	explicit QtnSinglePropertyBase(QObject *parent)
		: QtnProperty(parent)
	{
	}

	virtual void doReset(QtnPropertyChangeReason reason) override
	{
		Q_ASSERT(reason & QtnPropertyChangeReasonResetValue);
		ValueTypeStore defaultValue;
		if (defaultValueImpl(defaultValue))
		{
			setValue(defaultValue, reason);
		} else
		{
			QtnProperty::doReset(reason);
		}
	}

	virtual ValueType valueImpl() const = 0;
	virtual void setValueImpl(ValueType newValue, QtnPropertyChangeReason reason) = 0;
	virtual bool isValueAcceptedImpl(ValueType)
	{
		return true;
	}

	virtual bool defaultValueImpl(ValueTypeStore &to) const
	{
		Q_UNUSED(to);
		return false;
	}

	virtual bool isValueEqualImpl(ValueType valueToCompare)
	{
		return EqPred()(valueToCompare, value());
	}

	// serialization implementation
	virtual bool loadImpl(QDataStream &stream) override
	{
		if (!QtnProperty::loadImpl(stream))
			return false;

		ValueTypeStore newValue;
		stream >> newValue;

		if (stream.status() != QDataStream::Ok)
			return false;

		setValue(newValue, QtnPropertyChangeReasonLoadedValue);

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
	virtual bool fromVariantImpl(
		const QVariant &var, QtnPropertyChangeReason reason) override
	{
		if (var.canConvert<ValueTypeStore>())
			return setValue(var.value<ValueTypeStore>(), reason);

		return QtnProperty::fromVariantImpl(var, reason);
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

	ValueType valueImpl() const override
	{
		return m_value;
	}

	void setValueImpl(ValueType newValue, QtnPropertyChangeReason /*reason*/) override
	{
		m_value = newValue;
	}

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

	typedef std::function<ValueTypeStore()> CallbackValueGet;
	typedef std::function<void(ValueType, QtnPropertyChangeReason)> CallbackValueSet;
	typedef std::function<bool(ValueType)> CallbackValueAccepted;
	typedef std::function<bool(ValueType)> CallbackValueEqual;

	inline const CallbackValueGet &callbackValueDefault() const
	{
		return m_callbackValueDefault;
	}

	inline const CallbackValueGet &callbackValueGet() const
	{
		return m_callbackValueGet;
	}

	inline const CallbackValueSet &callbackValueSet() const
	{
		return m_callbackValueSet;
	}

	inline const CallbackValueAccepted &callbackValueAccepted() const
	{
		return m_callbackValueAccepted;
	}

	inline const CallbackValueEqual &callbackValueEqual() const
	{
		return m_callbackValueEqual;
	}

	inline void setCallbackValueDefault(const CallbackValueGet &callback)
	{
		m_callbackValueDefault = callback;
		this->switchState(QtnPropertyStateResettable, callback != nullptr);
	}

	inline void setCallbackValueGet(const CallbackValueGet &callback)
	{
		m_callbackValueGet = callback;
	}

	inline void setCallbackValueSet(const CallbackValueSet &callback)
	{
		m_callbackValueSet = callback;
		this->switchState(QtnPropertyStateImmutable, callback == nullptr);
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
		Q_ASSERT(m_callbackValueGet);
		return m_callbackValueGet();
	}

	virtual void setValueImpl(ValueType newValue, QtnPropertyChangeReason reason) override
	{
		Q_ASSERT(m_callbackValueSet);
		m_callbackValueSet(newValue, reason);
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

	virtual bool defaultValueImpl(ValueTypeStore &to) const override
	{
		if (m_callbackValueDefault)
		{
			to = m_callbackValueDefault();
			return true;
		}

		return false;
	}

private:
	Q_DISABLE_COPY(QtnSinglePropertyCallback)

	CallbackValueGet m_callbackValueDefault;
	CallbackValueGet m_callbackValueGet;
	CallbackValueSet m_callbackValueSet;
	CallbackValueAccepted m_callbackValueAccepted;
	CallbackValueEqual m_callbackValueEqual;
};

template <typename QtnSinglePropertyType>
class QtnNumericPropertyBase : public QtnSinglePropertyType
{
	template <class T, class Enable = void>
	struct interval_t
	{
	};

	template <class T>
	struct interval_t<T,
		typename std::enable_if<std::is_floating_point<T>::value>::type>
	{
		using type = double;
		using maxsigned = double;
	};

	template <class T>
	struct interval_t<T,
		typename std::enable_if<std::is_integral<T>::value>::type>
	{
		using type = typename std::make_unsigned<T>::type;
		using maxsigned = qint64;
	};

public:
	using ValueType = typename QtnSinglePropertyType::ValueType;
	using IntervalType = typename interval_t<ValueType>::type;
	using SignedMaxType = typename interval_t<ValueType>::maxsigned;
	using MaxIntervalType = typename interval_t<SignedMaxType>::type;

	inline ValueType value() const
	{
		return correctValue(QtnSinglePropertyType::value());
	}

	inline ValueType minValue() const
	{
		return m_minValue;
	}

	inline void setMinValue(ValueType minValue)
	{
		if (m_minValue == minValue)
			return;

		emit this->propertyWillChange(
			QtnPropertyChangeReasonNewValue, nullptr, 0);
		m_minValue = minValue;
		m_maxValue = std::max(m_minValue, m_maxValue);
		emit this->propertyDidChange(QtnPropertyChangeReasonNewValue);
	}

	inline ValueType maxValue() const
	{
		return m_maxValue;
	}

	inline void setMaxValue(ValueType maxValue)
	{
		if (maxValue == m_maxValue)
			return;

		emit this->propertyWillChange(
			QtnPropertyChangeReasonNewValue, nullptr, 0);
		m_maxValue = maxValue;
		m_minValue = std::min(m_minValue, m_maxValue);
		emit this->propertyDidChange(QtnPropertyChangeReasonNewValue);
	}

	inline ValueType correctValue(ValueType value) const
	{
		if (value < m_minValue)
			value = m_minValue;

		if (value > m_maxValue)
			value = m_maxValue;

		return value;
	}

	inline ValueType stepValue() const
	{
		return m_stepValue;
	}

	inline void setStepValue(ValueType stepValue)
	{
		if (stepValue == m_stepValue)
			return;

		emit this->propertyWillChange(
			QtnPropertyChangeReasonStateLocal, nullptr, 0);
		m_stepValue = stepValue;
		emit this->propertyDidChange(QtnPropertyChangeReasonStateLocal);
	}

	inline void incrementValue(
		QtnPropertyChangeReason reason = QtnPropertyChangeReasonNewValue,
		int steps = 1)
	{
		auto oldValue = this->value();
		ValueType newValue;
		auto step = SignedMaxType(m_stepValue) * steps;
		if (step < SignedMaxType(0) &&
			MaxIntervalType(-step) >= IntervalType(oldValue - m_minValue))
		{
			newValue = m_minValue;
		} else if (step > SignedMaxType(0) &&
			MaxIntervalType(step) >= IntervalType(m_maxValue - oldValue))
		{
			newValue = m_maxValue;
		} else
		{
			newValue = oldValue + step;
		}
		this->setValue(newValue, reason);
	}

protected:
	explicit QtnNumericPropertyBase(QObject *parent)
		: QtnSinglePropertyType(parent)
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

private:
	ValueType m_minValue;
	ValueType m_maxValue;
	ValueType m_stepValue;

	Q_DISABLE_COPY(QtnNumericPropertyBase)
};

template <typename T>
inline void qtnMakePercentProperty(T *dProp,
	typename T::ValueType (*AfterGet)(typename T::ValueType),
	const QByteArray &delegateName = QByteArray())
{
	using ValueType = typename T::ValueType;
	auto prevGet = dProp->callbackValueGet();
	if (prevGet)
	{
		dProp->setCallbackValueGet([prevGet, AfterGet]() -> ValueType {
			return AfterGet(prevGet() * 100.0);
		});
	}

	auto prevEqual = dProp->callbackValueEqual();
	if (prevEqual)
	{
		dProp->setCallbackValueEqual([prevEqual](ValueType value) -> bool {
			return prevEqual(value / 100.0);
		});
	}

	auto prevSet = dProp->callbackValueSet();
	if (prevSet)
	{
		dProp->setCallbackValueSet(
			[prevSet](ValueType value) { prevSet(value / 100.0); });
	}

	auto prevDefault = dProp->callbackValueDefault();
	if (prevDefault)
	{
		dProp->setCallbackValueDefault([prevDefault, AfterGet]() -> ValueType {
			return AfterGet(prevDefault() * 100.0);
		});
	}

	QtnPropertyDelegateInfo delegate;
	qtnInitPercentSpinBoxDelegate(delegate);
	if (!delegateName.isEmpty())
		delegate.name = delegateName;
	dProp->setDelegateInfo(delegate);
}

template <typename QtnSinglePropertyType>
class QtnNumericPropertyValue : public QtnSinglePropertyType
{
public:
	using ValueType = typename QtnSinglePropertyType::ValueType;

	inline ValueType defaultValue() const
	{
		return m_defaultValue;
	}

	inline void setDefaultValue(ValueType defaultValue)
	{
		m_defaultValue = defaultValue;
	}

protected:
	explicit QtnNumericPropertyValue(QObject *parent)
		: QtnSinglePropertyType(parent)
		, m_value(ValueType(0))
		, m_defaultValue(ValueType(0))
	{
		this->addState(QtnPropertyStateResettable);
	}

	virtual bool defaultValueImpl(ValueType &to) const override
	{
		to = this->correctValue(m_defaultValue);
		return true;
	}

	virtual ValueType valueImpl() const override
	{
		return m_value;
	}

	virtual void setValueImpl(ValueType newValue, QtnPropertyChangeReason /*reason*/) override
	{
		m_value = newValue;
	}

private:
	ValueType m_value;
	ValueType m_defaultValue;

	Q_DISABLE_COPY(QtnNumericPropertyValue)
};

template class QTN_IMPORT_EXPORT QtnSinglePropertyBase<bool>;
template class QTN_IMPORT_EXPORT QtnSinglePropertyBase<qint32>;
template class QTN_IMPORT_EXPORT QtnSinglePropertyBase<quint32>;
template class QTN_IMPORT_EXPORT QtnSinglePropertyBase<qint64>;
template class QTN_IMPORT_EXPORT QtnSinglePropertyBase<quint64>;
template class QTN_IMPORT_EXPORT QtnSinglePropertyBase<float>;
template class QTN_IMPORT_EXPORT QtnSinglePropertyBase<double>;
template class QTN_IMPORT_EXPORT QtnSinglePropertyBase<QString>;
template class QTN_IMPORT_EXPORT QtnSinglePropertyBase<QVariant>;

#endif // PROPERTY_TEMPLATES_H
