/*******************************************************************************
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "MultiProperty.h"

#include "Config.h"
#include "Property.h"
#include "PropertyConnector.h"
#include "PropertySet.h"
#include "QObjectPropertySet.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/Core/PropertyDelegateBool.h"

#include <QStyleOption>
#include <QKeyEvent>
#include <QLineEdit>

QtnMultiProperty::QtnMultiProperty(
	const QMetaObject *propertyMetaObject, QObject *parent)
	: QtnProperty(parent)
	, mPropertyMetaObject(propertyMetaObject)
	, mutablePropertyIndex(-1)
	, calculateMultipleValues(true)
	, multipleValues(false)
{
}

QtnMultiProperty::~QtnMultiProperty()
{
	for (auto property : properties)
	{
		QObject::disconnect(property, &QtnProperty::propertyValueAccept, this,
			&QtnMultiProperty::onPropertyValueAccept);
		QObject::disconnect(property, &QtnPropertyBase::propertyWillChange,
			this, &QtnMultiProperty::onPropertyWillChange);
		QObject::disconnect(property, &QtnPropertyBase::propertyDidChange, this,
			&QtnMultiProperty::onPropertyDidChange);
	}
}

const QMetaObject *QtnMultiProperty::propertyMetaObject() const
{
	return mPropertyMetaObject;
}

void QtnMultiProperty::addProperty(QtnProperty *property, bool own)
{
	Q_ASSERT(nullptr != property);
	Q_ASSERT(nullptr != mPropertyMetaObject->cast(property));

	if (own)
		property->setParent(this);

	if (properties.end() ==
		std::find(properties.begin(), properties.end(), property))
	{
		properties.push_back(property);
		updateStateFrom(property);
	} else
	{
		updateStateFrom(property);
		return;
	}

	auto delegate = property->delegate();

	if (delegate)
	{
		QtnPropertyDelegateInfo copy;
		copy.attributes = delegate->attributes;
		setDelegate(copy);
	}

	QObject::connect(property, &QtnProperty::propertyValueAccept, this,
		&QtnMultiProperty::onPropertyValueAccept);
	QObject::connect(property, &QtnPropertyBase::propertyWillChange, this,
		&QtnMultiProperty::onPropertyWillChange);
	QObject::connect(property, &QtnPropertyBase::propertyDidChange, this,
		&QtnMultiProperty::onPropertyDidChange);
}

void QtnMultiProperty::resetValues(bool edit)
{
	if (!valueIsDefault() && isEditableByUser())
	{
		QtnPropertyChangeReason reasons =
			QtnPropertyChangeReasonNewValue | QtnPropertyChangeReasonResetValue;

		if (edit)
			reasons |= QtnPropertyChangeReasonEditValue;

		emit propertyWillChange(reasons, nullptr, 0);

		for (auto property : properties)
		{
			auto connector = property->getConnector();

			if (nullptr != connector)
				connector->resetPropertyValue(false);
		}

		emit propertyDidChange(reasons);

		updateMultipleState(true);
	}
}

bool QtnMultiProperty::hasResettableValues() const
{
	for (auto property : properties)
	{
		auto connector = property->getConnector();

		if (nullptr != connector && connector->isResettablePropertyValue())
			return true;
	}

	return false;
}

bool QtnMultiProperty::Register()
{
	qRegisterMetaType<QtnMultiVariant>();

	return QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
		&QtnMultiProperty::staticMetaObject,
		&qtnCreateDelegate<QtnMultiPropertyDelegate, QtnMultiProperty>,
		QByteArrayLiteral("MultiProperty"));
}

QString QtnMultiProperty::getMultiValuePlaceholder()
{
	return tr("(Multiple Values)");
}

QMetaProperty QtnMultiProperty::getMetaProperty() const
{
	Q_ASSERT(!properties.empty());

	auto connector =
		properties.at(mutablePropertyIndex < 0 ? 0 : mutablePropertyIndex)
			->getConnector();
	Q_ASSERT(nullptr != connector);

	return connector->getMetaProperty();
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

void QtnMultiProperty::onPropertyValueAccept(
	QtnPropertyValuePtr valueToAccept, bool *accept)
{
	emit propertyValueAccept(valueToAccept, accept);
}

void QtnMultiProperty::onPropertyWillChange(
	QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue, int typeId)
{
	emit propertyWillChange(reason, newValue, typeId);
}

void QtnMultiProperty::onPropertyDidChange(QtnPropertyChangeReason reason)
{
	if (0 !=
		(reason &
			(QtnPropertyChangeReasonState | QtnPropertyChangeReasonValue)))
	{
		auto property = qobject_cast<QtnProperty *>(sender());
		Q_ASSERT(nullptr != property);
		updateStateFrom(property);
		updateMultipleState(true);
	}

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

	if (var.type() == QVariant::UserType &&
		var.userType() == qMetaTypeId<QtnMultiVariant>())
	{
		auto values = var.value<QtnMultiVariant>().values;
		auto count = properties.size();
		auto varCount = size_t(values.count());

		if (count != varCount)
			ok = false;

		if (count > varCount)
			count = varCount;

		for (size_t i = 0; i < count; i++)
		{
			QtnProperty *property = properties.at(i);

			if (!property->fromVariant(values.at(int(i)), edit))
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

void QtnMultiProperty::updateStateFrom(QtnProperty *source)
{
	if (stateLocal() == QtnPropertyStateNone)
		setState(source->stateLocal());

	if (source->isEditableByUser())
	{
		removeState(QtnPropertyStateImmutable | QtnPropertyStateInvisible);

		if (mutablePropertyIndex >= 0)
			return;
	}

	updateMutablePropertyIndex();
}

void QtnMultiProperty::updateMultipleState(bool force)
{
	if (force)
		calculateMultipleValues = true;

	bool multipleValues = hasMultipleValues();
	auto state = stateLocal() & ~QtnPropertyStateModifiedValue;

	if (multipleValues)
		state |= QtnPropertyStateHiddenValue;
	else
		state &= ~QtnPropertyStateHiddenValue;

	for (auto property : properties)
	{
		if (!property->valueIsDefault())
		{
			state |= QtnPropertyStateModifiedValue;
			break;
		}
	}

	setState(state);
	properties.at((mutablePropertyIndex < 0) ? 0 : mutablePropertyIndex)
		->switchState(QtnPropertyStateHiddenValue, multipleValues);
}

void QtnMultiProperty::updateMutablePropertyIndex()
{
	if (mutablePropertyIndex < 0 ||
		!properties.at(mutablePropertyIndex)->isEditableByUser())
	{
		mutablePropertyIndex = 0;

		for (auto property : properties)
		{
			if (property->isEditableByUser())
				return;

			mutablePropertyIndex++;
		}

		mutablePropertyIndex = -1;
	}
}

QtnMultiPropertyDelegate::QtnMultiPropertyDelegate(QtnMultiProperty &owner)
	: Inherited(owner)
{
	owner.updateMultipleState(true);
}

void QtnMultiPropertyDelegate::init()
{
	Q_ASSERT(superDelegates.empty());

	auto &properties = owner().properties;
	superDelegates.reserve(properties.size());
	for (auto property : properties)
	{
		auto delegate = factory()->createDelegate(*property);
		superDelegates.emplace_back(delegate);

		for (int i = 0, count = delegate->subPropertyCount(); i < count; ++i)
		{
			auto property = delegate->subProperty(i);
			auto it = std::find_if(m_subProperties.begin(),
				m_subProperties.end(),
				[property](const QScopedPointer<QtnPropertyBase> &a) -> bool {
					return property->propertyMetaObject() ==
						a->propertyMetaObject() &&
						property->name() == a->name();
				});

			auto subSet = property->asPropertySet();
			if (subSet)
			{
				QtnPropertySet *multiSet;

				if (it == m_subProperties.end())
				{
					multiSet = new QtnPropertySet(
						subSet->childrenOrder(), subSet->compareFunc());
					multiSet->setName(subSet->name());
					multiSet->setDescription(subSet->description());
					multiSet->setId(subSet->id());
					multiSet->setState(subSet->stateLocal());

					addSubProperty(multiSet);
				} else
				{
					multiSet = it->data()->asPropertySet();
				}

				qtnPropertiesToMultiSet(multiSet, subSet);
			} else
			{
				QtnMultiProperty *multiProperty;

				if (it == m_subProperties.end())
				{
					multiProperty =
						new QtnMultiProperty(property->metaObject());
					multiProperty->setName(property->name());
					multiProperty->setDescription(property->description());
					multiProperty->setId(property->id());

					addSubProperty(multiProperty);
				} else
				{
					multiProperty =
						qobject_cast<QtnMultiProperty *>(it->data());
				}

				multiProperty->addProperty(property->asProperty(), false);
			}
		}
	}
}

QtnMultiPropertyDelegate::~QtnMultiPropertyDelegate()
{
	m_subProperties.clear();
}

void QtnMultiPropertyDelegate::onEditedPropertyWillChange(PropertyToEdit *data,
	QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue, int typeId)
{
	Q_ASSERT(nullptr != data);
	auto owner = data->owner;
	Q_ASSERT(nullptr != owner);

	emit owner->propertyWillChange(reason, newValue, typeId);
}

void QtnMultiPropertyDelegate::onEditedPropertyDidChange(
	PropertyToEdit *data, QtnPropertyChangeReason reason)
{
	Q_ASSERT(nullptr != data);
	auto owner = data->owner;
	Q_ASSERT(nullptr != owner);

	if (0 != (reason & QtnPropertyChangeReasonEditValue))
	{
		auto editedProperty = data->property;
		Q_ASSERT(nullptr != editedProperty);

		auto value = editedProperty->valueAsVariant();

		for (auto property : owner->properties)
		{
			if (property != editedProperty && property->isEditableByUser())
			{
				property->blockSignals(true);
				property->setValueAsVariant(value);
				property->blockSignals(false);
			}
		}
	}

	owner->updateMultipleState(true);

	emit owner->propertyDidChange(reason);
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
		{
			QObject::disconnect(connection);
		}
	}

	delete data;
}

bool QtnMultiPropertyDelegate::propertyValueToStr(QString &strValue) const
{
	if (owner().valueIsHidden())
	{
		strValue = QtnMultiProperty::getMultiValuePlaceholder();
		return true;
	}

	if (!superDelegates.empty())
		return superDelegates.at(0)->propertyValueToStr(strValue);

	strValue.clear();
	return false;
}

void QtnMultiPropertyDelegate::applyAttributesImpl(
	const QtnPropertyDelegateAttributes &attributes)
{
	for (auto &delegate : superDelegates)
	{
		delegate->applyAttributes(attributes);
	}
}

void QtnMultiPropertyDelegate::drawValueImpl(QStylePainter &painter,
	const QRect &rect, const QStyle::State &state, bool *needTooltip) const
{
	if (owner().valueIsHidden())
	{
		Q_ASSERT(!superDelegates.empty());

		auto boolDelegate = dynamic_cast<QtnPropertyDelegateBoolCheck *>(
			superDelegates.at(0).get());

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
	} else if (!superDelegates.empty())
	{
		superDelegates.at(0)->drawValue(painter, rect, state, needTooltip);
	} else
	{
		Inherited::drawValueImpl(painter, rect, state, needTooltip);
	}
}

QString QtnMultiPropertyDelegate::toolTipImpl() const
{
	if (owner().valueIsHidden())
		return Inherited::toolTipImpl();

	if (!superDelegates.empty())
		return superDelegates.at(0)->toolTip();

	return QString();
}

bool QtnMultiPropertyDelegate::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (!superDelegates.empty())
		return superDelegates.at(0)->acceptKeyPressedForInplaceEdit(keyEvent);

	return Inherited::acceptKeyPressedForInplaceEditImpl(keyEvent);
}

QWidget *QtnMultiPropertyDelegate::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	if (!superDelegates.empty())
	{
		PropertyToEdit *data = nullptr;

		bool editable = owner().isEditableByUser();

		int superIndex = 0;

		auto propertyToEdit = owner().properties.at(superIndex);

		if (editable)
		{
			superIndex = owner().mutablePropertyIndex;
			data = new PropertyToEdit;
			data->owner = &owner();
			data->property = propertyToEdit;

			using namespace std::placeholders;

			data->connections.push_back(QObject::connect(propertyToEdit,
				&QtnProperty::propertyWillChange,
				std::bind(&QtnMultiPropertyDelegate::onEditedPropertyWillChange,
					data, _1, _2, _3)));
			data->connections.push_back(QObject::connect(propertyToEdit,
				&QtnProperty::propertyDidChange,
				std::bind(&QtnMultiPropertyDelegate::onEditedPropertyDidChange,
					data, _1)));
			data->connections.push_back(QObject::connect(propertyToEdit,
				&QObject::destroyed,
				std::bind(&QtnMultiPropertyDelegate::onEditedPropertyDestroyed,
					data)));
		}

		auto superDelegate = superDelegates.at(superIndex).get();

		auto delegate = propertyToEdit->delegate();

		if (delegate)
		{
			superDelegate->applyAttributes(delegate->attributes);
		}

		auto editor =
			superDelegate->createValueEditor(parent, rect, inplaceInfo);

		if (editable)
		{
			if (nullptr == editor)
			{
				onEditorDestroyed(data);
			} else
			{
				data->connections.push_back(
					QObject::connect(editor, &QObject::destroyed,
						std::bind(&QtnMultiPropertyDelegate::onEditorDestroyed,
							data)));
			}
		}

		return editor;
	}

	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}
