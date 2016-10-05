#include "MultiProperty.h"

#include "Config.h"
#include "PropertyConnector.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/Core/PropertyDelegateBool.h"

#include <QStyleOption>
#include <QKeyEvent>

QtnMultiProperty::QtnMultiProperty(const QMetaObject *propertyMetaObject, QObject *parent)
	: QtnProperty(parent)
	, propertyMetaObject(propertyMetaObject)
	, calculateMultipleValues(true)
	, multipleValues(false)
{

}

QtnMultiProperty::~QtnMultiProperty()
{
	for (auto property : properties)
	{
		QObject::disconnect(property, &QtnProperty::propertyValueAccept,
							this, &QtnMultiProperty::onPropertyValueAccept);
		QObject::disconnect(property, &QtnPropertyBase::propertyWillChange,
							this, &QtnMultiProperty::onPropertyWillChange);
		QObject::disconnect(property, &QtnPropertyBase::propertyDidChange,
							this, &QtnMultiProperty::onPropertyDidChange);
	}
}

void QtnMultiProperty::addProperty(QtnProperty *property, bool own)
{
	Q_ASSERT(nullptr != property);
	Q_ASSERT(nullptr != propertyMetaObject->cast(property));

	if (own)
		property->setParent(this);

	if (properties.end() != std::find(properties.begin(), properties.end(), property))
		return;

	properties.push_back(property);

	QObject::connect(property, &QtnProperty::propertyValueAccept,
					 this, &QtnMultiProperty::onPropertyValueAccept);
	QObject::connect(property, &QtnPropertyBase::propertyWillChange,
					 this, &QtnMultiProperty::onPropertyWillChange);
	QObject::connect(property, &QtnPropertyBase::propertyDidChange,
					 this, &QtnMultiProperty::onPropertyDidChange);
}

void QtnMultiProperty::resetValues()
{
	for (auto property : properties)
	{
		auto connector = getPropertyConnector(property);

		if (nullptr != connector)
			connector->resetPropertyValue();
	}

	refreshValues();
	emit propertyDidChange(this, this, QtnPropertyChangeReasonNewValue);
}

bool QtnMultiProperty::hasResettableValues() const
{
	for (auto property : properties)
	{
		auto connector = getPropertyConnector(property);

		if (nullptr != connector && connector->isResettablePropertyValue())
			return true;
	}

	return false;
}

void QtnMultiProperty::Register()
{
	qRegisterMetaType<QtnMultiVariant>();

	QtnPropertyDelegateFactory::staticInstance()
		.registerDelegateDefault(&QtnMultiProperty::staticMetaObject
		, &qtnCreateDelegate<QtnMultiPropertyDelegate, QtnMultiProperty>
		, "MultiProperty");
}

QString QtnMultiProperty::getMultiValuePlaceholder()
{
	return tr("(Multiple Values)");
}

bool QtnMultiProperty::hasMultipleValues() const
{
	if (calculateMultipleValues)
	{
		QString dummy;
		toStrImpl(dummy);
	}

	return multipleValues;
}

void QtnMultiProperty::onPropertyValueAccept(QtnPropertyValuePtr valueToAccept, bool *accept)
{
	emit propertyValueAccept(valueToAccept, accept);
}

void QtnMultiProperty::onPropertyWillChange(QtnPropertyChangeReason reason,
											QtnPropertyValuePtr newValue)
{
	emit propertyWillChange(reason, newValue);
}

void QtnMultiProperty::onPropertyDidChange(QtnPropertyChangeReason reason)
{
	if (0 != (reason & QtnPropertyChangeReasonValue))
		refreshValues();

	emit propertyDidChange(reason);
}

bool QtnMultiProperty::loadImpl(QDataStream &stream)
{
	for (auto property : properties)
	{
		QByteArray propertyData;
		stream >> propertyData;
		QDataStream propertyStream(&propertyData, QIODevice::ReadOnly);
		if (!property->load(propertyStream))
			return false;
	}

	return true;
}

bool QtnMultiProperty::saveImpl(QDataStream &stream) const
{
	for (auto property : properties)
	{
		QByteArray propertyData;
		QDataStream propertyStream(&propertyData, QIODevice::WriteOnly);
		if (!property->save(propertyStream))
			return false;

		stream << propertyData;
	}

	return true;
}

bool QtnMultiProperty::fromStrImpl(const QString &str, bool edit)
{
	for (auto property : properties)
	{
		if (!property->fromStr(str, edit))
			return false;
	}

	return true;
}

bool QtnMultiProperty::toStrImpl(QString &str) const
{
	if (calculateMultipleValues)
	{
		auto thiz = const_cast<QtnMultiProperty *>(this);
		thiz->calculateMultipleValues = false;

		size_t sameCount = 0;
		QString temp;
		for (auto property : properties)
		{
			if (!property->toStr(str))
				str.clear();

			if (sameCount == 0)
				temp = str;

			if (sameCount == 0 || str == temp)
				sameCount++;
		}

		thiz->multipleValues = (sameCount != properties.size());
	} else if (!multipleValues)
	{
		if (properties.empty() || !properties.at(0)->toStr(str))
			str.clear();
	}

	if (multipleValues)
		str.clear();

	return true;
}

bool QtnMultiProperty::fromVariantImpl(const QVariant &var, bool edit)
{
	bool ok = true;
	if (var.type() == QVariant::UserType && var.userType() == qMetaTypeId<QtnMultiVariant>())
	{
		auto values = var.value<QtnMultiVariant>().values;
		auto count = properties.size();
		auto varCount = static_cast<size_t>(values.count());
		if (count != varCount)
			ok = false;

		if (count > varCount)
			count = varCount;

		for (size_t i = 0; i < count; i++)
		{
			QtnProperty *property = properties.at(i);
			if (!property->fromVariant(values.at(i), edit))
				ok = false;
		}
	} else
	{
		for (auto property : properties)
		{
			if (!property->fromVariant(var, edit))
				ok = false;
		}
	}

	return ok;
}

bool QtnMultiProperty::toVariantImpl(QVariant &var) const
{
	QtnMultiVariant multiVariant;

	for (auto property : properties)
	{
		if (!property->toVariant(var))
			var.clear();

		multiVariant.values.push_back(var);
	}

	var.setValue(multiVariant);

	return true;
}

QtnPropertyConnector *QtnMultiProperty::getPropertyConnector(QtnProperty *property)
{
	Q_ASSERT(nullptr != property);

	return property->findChild<QtnPropertyConnector *>(
								 QString(), Qt::FindDirectChildrenOnly);
}

QtnMultiPropertyDelegate::QtnMultiPropertyDelegate(QtnMultiProperty &owner)
	: Inherited(owner)
{
	int subPropertyCount = 0;
	for (auto property : owner.properties)
	{
		auto delegate = QtnPropertyDelegateFactory::staticInstance()
						.createDelegate(*property);
		superDelegates.push_back(DelegatePtr(delegate));
		subPropertyCount = delegate->subPropertyCount();
	}

	for (int i = 0; i < subPropertyCount; i++)
	{
		auto delegate = superDelegates.at(0).get();
		auto subProperty = delegate->subProperty(i);

		auto subMultiProperty = new QtnMultiProperty(subProperty->metaObject());
		subMultiProperty->setName(subProperty->name());
		subMultiProperty->setState(subProperty->stateLocal());
		subMultiProperty->setDescription(subProperty->description());
		subMultiProperty->setId(subProperty->id());

		for (auto &delegate : superDelegates)
		{
			subMultiProperty->addProperty(
				dynamic_cast<QtnProperty *>(delegate->subProperty(i)), false);
		}

		addSubProperty(subMultiProperty);
	}
}

QtnMultiPropertyDelegate::~QtnMultiPropertyDelegate()
{
	m_subProperties.clear();
}

void QtnMultiPropertyDelegate::onPropertyEdited(PropertyToEdit *data)
{
	Q_ASSERT(nullptr != data);
	auto owner = data->owner;
	Q_ASSERT(nullptr != owner);
	auto editedProperty = data->property;
	Q_ASSERT(nullptr != editedProperty);

	auto value = editedProperty->valueAsVariant();
	for (auto property : owner->properties)
	{
		if (property != editedProperty)
		{
			property->blockSignals(true);
			property->setValueAsVariant(value);
			property->blockSignals(false);
		}
	}

	owner->refreshValues();

	emit owner->propertyDidChange(owner, owner, QtnPropertyChangeReasonNewValue);
}

void QtnMultiPropertyDelegate::onEditedPropertyDestroyed(PropertyToEdit *data)
{
	Q_ASSERT(nullptr != data);
	data->property = nullptr;
	data->connections.clear();
}

void QtnMultiPropertyDelegate::onEditorDestroyed(PropertyToEdit *data)
{
	Q_ASSERT(nullptr != data);
	auto propertyToEdit = data->property;
	if (nullptr != propertyToEdit)
	{
		for (auto &connection : data->connections)
			QObject::disconnect(connection);
	}

	delete data;
}


bool QtnMultiPropertyDelegate::propertyValueToStr(QString &strValue) const
{
	if (owner().hasMultipleValues())
	{
		strValue = QtnMultiProperty::getMultiValuePlaceholder();
		return true;
	}

	if (!superDelegates.empty())
		return superDelegates.at(0)->propertyValueToStr(strValue);

	strValue.clear();
	return false;
}

void QtnMultiPropertyDelegate::applyAttributesImpl(const QtnPropertyDelegateAttributes &attributes)
{
	for (auto &delegate : superDelegates)
	{
		delegate->applyAttributes(attributes);
	}
}

void QtnMultiPropertyDelegate::drawValueImpl(QStylePainter &painter, const QRect &rect,
											 const QStyle::State &state, bool *needTooltip) const
{
	if (owner().hasMultipleValues())
	{
		Q_ASSERT(!superDelegates.empty());

		auto boolDelegate = dynamic_cast<QtnPropertyDelegateBoolCheck *>(superDelegates.at(0).get());
		if (nullptr != boolDelegate)
		{
			QStyleOptionButton opt;
			opt.rect = rect;
			opt.state = state | QStyle::State_NoChange;

			painter.drawControl(QStyle::CE_CheckBox, opt);
			return;
		}

		QPen oldPen = painter.pen();
		painter.setPen(Qt::darkGray);

		Inherited::drawValueImpl(painter, rect, state, needTooltip);
		painter.setPen(oldPen);
	} else
	if (!superDelegates.empty())
	{
		superDelegates.at(0)->drawValue(painter, rect, state, needTooltip);
	} else
	{
		Inherited::drawValueImpl(painter, rect, state, needTooltip);
	}
}

QString QtnMultiPropertyDelegate::toolTipImpl() const
{
	if (owner().hasMultipleValues())
		return Inherited::toolTipImpl();

	if (!superDelegates.empty())
		return superDelegates.at(0)->toolTip();

	return QString();
}

bool QtnMultiPropertyDelegate::acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const
{
	if (!superDelegates.empty())
		return superDelegates.at(0)->acceptKeyPressedForInplaceEdit(keyEvent);

	return Inherited::acceptKeyPressedForInplaceEditImpl(keyEvent);
}

QWidget *QtnMultiPropertyDelegate::createValueEditorImpl(QWidget *parent,
														 const QRect &rect,
														 QtnInplaceInfo *inplaceInfo)
{
	if (!superDelegates.empty())
	{
		auto superDelegate = superDelegates.at(0).get();

		PropertyToEdit *data = nullptr;
		auto propertyToEdit = owner().properties.at(0);
		if (owner().hasMultipleValues())
			propertyToEdit->addState(QtnPropertyStateHiddenValue);

		bool isEditable = owner().isEditableByUser();

		if (isEditable)
		{
			data = new PropertyToEdit;
			data->owner = &owner();
			data->property = propertyToEdit;

			data->connections.push_back(QObject::connect(propertyToEdit, &QtnProperty::propertyEdited,
							 std::bind(&QtnMultiPropertyDelegate::onPropertyEdited, data)));
			data->connections.push_back(QObject::connect(propertyToEdit, &QObject::destroyed,
							 std::bind(&QtnMultiPropertyDelegate::onEditedPropertyDestroyed, data)));
		}

		auto editor = superDelegate->createValueEditor(parent, rect, inplaceInfo);

		if (isEditable)
		{
			if (nullptr == editor)
			{
				onEditorDestroyed(data);
			} else
			{
				data->connections.push_back(QObject::connect(editor, &QObject::destroyed,
								 std::bind(&QtnMultiPropertyDelegate::onEditorDestroyed, data)));
			}
		}

		return editor;
	}

	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

void QtnMultiProperty::refreshValues()
{
	calculateMultipleValues = true;

	properties.at(0)->switchState(QtnPropertyStateHiddenValue, hasMultipleValues());
}
