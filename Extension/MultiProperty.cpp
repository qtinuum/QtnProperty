#include "MultiProperty.h"

#include "QtnProperty/Widget/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Widget/Delegates/Core/PropertyDelegateBool.h"

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
						 this, &QtnMultiProperty::onPropertyValueChanged);
	}
}

void QtnMultiProperty::addProperty(QtnProperty *property, bool own)
{
	Q_ASSERT(nullptr != property);
	Q_ASSERT(nullptr != propertyMetaObject->cast(property));
	Q_ASSERT(properties.end() == std::find(properties.begin(), properties.end(), property));

	if (own)
		property->setParent(this);

	properties.push_back(property);

	QObject::connect(property, &QtnProperty::propertyValueAccept,
					 this, &QtnMultiProperty::onPropertyValueAccept);
	QObject::connect(property, &QtnPropertyBase::propertyWillChange,
					 this, &QtnMultiProperty::onPropertyWillChange);
	QObject::connect(property, &QtnPropertyBase::propertyDidChange,
					 this, &QtnMultiProperty::onPropertyValueChanged);
}

void QtnMultiProperty::Register()
{
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

void QtnMultiProperty::onPropertyValueAccept(const QtnProperty *property,
											 QtnPropertyValuePtr valueToAccept,
											 bool *accept)
{
	emit propertyValueAccept(property, valueToAccept, accept);
}

void QtnMultiProperty::onPropertyWillChange(const QtnPropertyBase *changedProperty,
											const QtnPropertyBase *firedProperty,
											QtnPropertyChangeReason reason,
											QtnPropertyValuePtr newValue)
{
	emit propertyWillChange(changedProperty, firedProperty, reason, newValue);
}

void QtnMultiProperty::onPropertyValueChanged(const QtnPropertyBase *changedProperty,
											  const QtnPropertyBase *firedProperty,
											  QtnPropertyChangeReason reason)
{
	calculateMultipleValues = true;
	emit propertyDidChange(changedProperty, firedProperty, reason);
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

bool QtnMultiProperty::fromStrImpl(const QString &str)
{
	for (auto property : properties)
	{
		if (!property->fromStr(str))
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

		if (multipleValues)
			str.clear();
	}

	return true;
}

bool QtnMultiProperty::fromVariantImpl(const QVariant &var)
{
	for (auto property : properties)
	{
		if (!property->fromVariant(var))
			return false;
	}

	return true;
}

bool QtnMultiProperty::toVariantImpl(QVariant &var) const
{
	if (calculateMultipleValues)
	{
		auto thiz = const_cast<QtnMultiProperty *>(this);
		thiz->calculateMultipleValues = false;

		size_t sameCount = 0;
		QVariant temp;
		for (auto property : properties)
		{
			if (!property->toVariant(var))
				var.clear();

			if (sameCount == 0)
				temp = var;

			if (sameCount == 0 || var == temp)
				sameCount++;
		}

		thiz->multipleValues = (sameCount != properties.size());

		if (multipleValues)
			var.clear();
	}

	return true;
}

QtnMultiPropertyDelegate::QtnMultiPropertyDelegate(QtnMultiProperty &owner)
	: Inherited(owner)
{
	int subPropertyCount = 0;
	for (auto property : owner.properties)
	{
		auto delegate = QtnPropertyDelegateFactory::staticInstance().createDelegate(*(property.get()));
		superDelegates.push_back(delegate);
		subPropertyCount = delegate->subPropertyCount();
	}

	for (int i = 0; i < subPropertyCount; i++)
	{
		auto delegate = superDelegates.at(0);
		auto subProperty = delegate->subProperty(i);

		auto subMultiProperty = new QtnMultiProperty(subProperty->metaObject());
		subMultiProperty->setName(subProperty->name());
		subMultiProperty->setState(subProperty->state());
		subMultiProperty->setDescription(subProperty->description());
		subMultiProperty->setId(subProperty->id());

		for (auto &delegate : superDelegates)
		{
			subMultiProperty->addProperty(delegate->subProperty(i), false);
		}

		addSubProperty(subMultiProperty);
	}
}

QtnMultiPropertyDelegate::~QtnMultiPropertyDelegate()
{
	delete superDelegate;
}

bool QtnMultiPropertyDelegate::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		auto keyEvent = static_cast<QKeyEvent *>(event);

		if (keyEvent->key() == Qt::Key_Escape)
		{
			owner().setValueAsVariant(backupValue);
			return true;
		}
	}

	return QObject::eventFilter(object, event);
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
		if (!superDelegates.empty())
		{
			auto boolDelegate = dynamic_cast<QtnPropertyDelegateBoolCheck *>(superDelegates.at(0).get());
			if (nullptr != boolDelegate)
			{
				QStyleOptionButton opt;
				opt.rect = rect;
				opt.state = state | QStyle::State_NoChange;

				painter.drawControl(QStyle::CE_CheckBox, opt);
				return;
			}
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

QWidget *QtnMultiPropertyDelegate::createValueEditorImpl(QWidget *parent, const QRect &rect,
														 QtnInplaceInfo *inplaceInfo)
{
	bool multiValue = owner().hasMultipleValues();
	if (multiValue)
	{
		backupValue = owner().valueAsVariant();
		owner().blockSignals(true);
		owner().setValueAsVariant(QVariant());
		owner().blockSignals(false);
	}

	if (!superDelegates.empty())
	{
		auto result = superDelegates.at(0)->createValueEditor(parent, rect, inplaceInfo);

		if (multiValue)
		{
			result->installEventFilter(this);
		}

		return result;
	}

	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}
