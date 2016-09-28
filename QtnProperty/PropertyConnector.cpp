#include "PropertyConnector.h"

#include "PropertyBase.h"
#include "Property.h"
#include "QObjectPropertySet.h"
#include "IQtnPropertyStateProvider.h"

QtnPropertyConnector::QtnPropertyConnector(QtnProperty *parent)
	: QObject(parent)
	, object(nullptr)
{
}

void QtnPropertyConnector::connectProperty(QObject *object, const QMetaProperty &metaProperty)
{
	this->object = object;
	this->metaProperty = metaProperty;
	auto meta_object = metaObject();
	auto slot = meta_object->method(meta_object->indexOfSlot("onValueChanged()"));
	QObject::connect(object, metaProperty.notifySignal(), this, slot);
}

bool QtnPropertyConnector::isResettablePropertyValue() const
{
	return metaProperty.isResettable();
}

void QtnPropertyConnector::resetPropertyValue()
{
	if (nullptr != object && metaProperty.isResettable())
		metaProperty.reset(object);
}

void QtnPropertyConnector::onValueChanged()
{
	auto property = dynamic_cast<QtnPropertyBase *>(parent());
	if (nullptr != property)
	{
		auto stateProvider = dynamic_cast<IQtnPropertyStateProvider *>(object);
		if (nullptr != stateProvider)
		{
			auto state = stateProvider->getPropertyState(metaProperty);
			bool collapsed = property->isCollapsed();
			if (collapsed)
				state |= QtnPropertyStateCollapsed;
			else
				state &= ~QtnPropertyStateCollapsed;
			property->setState(state);

			qtnUpdatePropertyState(property, metaProperty);
		}
		property->postUpdateEvent(QtnPropertyChangeReasonNewValue);
	}
}
