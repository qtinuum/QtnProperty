#include "PropertyConnector.h"

#include "PropertyBase.h"
#include "Property.h"
#include "QObjectPropertySet.h"
#include "IQtnPropertyStateProvider.h"

QtnPropertyConnector::QtnPropertyConnector(QtnPropertyBase *parent)
	: QObject(parent)
	, object(nullptr)
{
}

void QtnPropertyConnector::connectProperty(QObject *object, const QMetaProperty &metaProperty)
{
	this->object = object;
	this->metaProperty = metaProperty;
	auto metaObject = this->metaObject();
	auto slot = metaObject->method(metaObject->indexOfSlot("onValueChanged()"));
	QObject::connect(object, metaProperty.notifySignal(), this, slot);

	auto stateProvider = dynamic_cast<IQtnPropertyStateProvider *>(object);
	if (nullptr != stateProvider)
	{
		auto srcMetaObject = object->metaObject();
		auto signal = srcMetaObject->method(srcMetaObject->indexOfSignal("modifiedSetChanged()"));
		Q_ASSERT(signal.isValid());

		slot = metaObject->method(metaObject->indexOfSlot("onModifiedSetChanged()"));
		QObject::connect(object, signal, this, slot);
	}
}

bool QtnPropertyConnector::isResettablePropertyValue() const
{
	return metaProperty.isResettable();
}

void QtnPropertyConnector::resetPropertyValue(bool edit)
{
	if (nullptr != object && metaProperty.isResettable())
	{
		auto property = dynamic_cast<QtnPropertyBase *>(parent());
		Q_ASSERT(nullptr != property);

		if (!property->valueIsDefault())
		{
			QtnPropertyChangeReason reasons = QtnPropertyChangeReasonNewValue | QtnPropertyChangeReasonResetValue;
			if (edit)
				reasons |= QtnPropertyChangeReasonEditValue;

			emit property->propertyWillChange(reasons, nullptr, 0);
			metaProperty.reset(object);
			emit property->propertyDidChange(reasons);
		}
	}
}

void QtnPropertyConnector::onValueChanged()
{
	auto property = dynamic_cast<QtnPropertyBase *>(parent());
	if (nullptr != property)
	{
		property->postUpdateEvent(QtnPropertyChangeReasonNewValue);
	}
}

void QtnPropertyConnector::onModifiedSetChanged()
{
	auto property = dynamic_cast<QtnPropertyBase *>(parent());
	if (nullptr != property)
	{
		QtnPropertyState state;

		auto stateProvider = dynamic_cast<IQtnPropertyStateProvider *>(object);
		if (nullptr != stateProvider)
			state = stateProvider->getPropertyState(metaProperty);

		if (property->isCollapsed())
			state |= QtnPropertyStateCollapsed;
		else
			state &= ~QtnPropertyStateCollapsed;

		state |= qtnPropertyStateToAdd(metaProperty);

		property->setState(state);
	}
}
