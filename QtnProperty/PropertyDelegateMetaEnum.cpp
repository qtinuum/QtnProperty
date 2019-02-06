#include "PropertyDelegateMetaEnum.h"

#include "Delegates/PropertyEditorHandler.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Property.h"

#include <QCoreApplication>
#include <QComboBox>
#include <QLineEdit>

class QtnPropertyDelegateMetaEnum::EditorHandler
	: public QtnPropertyEditorHandlerBase
{
	QtnPropertyDelegateMetaEnum *mOwner;

public:
	EditorHandler(QtnPropertyDelegateMetaEnum *owner, QComboBox &editor);

	QComboBox &comboBox() const;

protected:
	virtual void updateEditor() override;
	void updateValue(int value);

private:
	void onCurrentIndexChanged(int index);

	unsigned updating;
};

QtnPropertyDelegateMetaEnum::QtnPropertyDelegateMetaEnum(
	const QMetaEnum &metaEnum, QtnProperty *property)
	: QtnPropertyDelegate(property)
	, mMetaEnum(metaEnum)
	, mShouldTranslate(false)
{
}

void QtnPropertyDelegateMetaEnum::Register(
	QMetaEnum metaEnum, QtnPropertyDelegateFactory *factory)
{
	if (!factory)
		factory = &QtnPropertyDelegateFactory::staticInstance();

	factory->registerDelegate(&QtnProperty::staticMetaObject,
		[metaEnum](QtnProperty &owner) -> QtnPropertyDelegate * {
			return new QtnPropertyDelegateMetaEnum(metaEnum, &owner);
		},
		delegateName(metaEnum));
}

QtnPropertyDelegateInfo QtnPropertyDelegateMetaEnum::delegateInfo(
	const QMetaEnum &metaEnum)
{
	QtnPropertyDelegateInfo result;
	result.name = delegateName(metaEnum);
	return result;
}

QByteArray QtnPropertyDelegateMetaEnum::delegateName(const QMetaEnum &metaEnum)
{
	const char *cscope = metaEnum.scope();
	const char *cname = metaEnum.name();
	auto scope = QByteArray::fromRawData(cscope, qstrlen(cscope));
	auto name = QByteArray::fromRawData(cname, qstrlen(cname));
	return scope + "." + name;
}

int QtnPropertyDelegateMetaEnum::currentValue() const
{
	QVariant v;
	ownerProperty->toVariant(v);
	return int(v.toLongLong());
}

bool QtnPropertyDelegateMetaEnum::propertyValueToStr(QString &strValue) const
{
	strValue = valueToStr(currentValue());
	return !strValue.isNull();
}

QString QtnPropertyDelegateMetaEnum::valueToStr(int value) const
{
	auto key = mMetaEnum.valueToKey(value);
	if (!key)
		return QString();

	return keyToStr(key);
}

QString QtnPropertyDelegateMetaEnum::keyToStr(const char *key) const
{
	return mShouldTranslate
		? QCoreApplication::translate(mMetaEnum.scope(), key)
		: QString(QLatin1String(key));
}

QByteArray QtnPropertyDelegateMetaEnum::translateAttribute()
{
	returnQByteArrayLiteral("translate");
}

QWidget *QtnPropertyDelegateMetaEnum::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	if (ownerProperty->isEditableByUser())
	{
		QComboBox *combo = new QComboBox(parent);
		for (int i = 0, count = mMetaEnum.keyCount(); i < count; i++)
		{
			combo->addItem(
				keyToStr(mMetaEnum.key(i)), QVariant(mMetaEnum.value(i)));
		}

		combo->setGeometry(rect);

		new EditorHandler(this, *combo);

		if (inplaceInfo)
			combo->showPopup();

		return combo;
	}

	return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

void QtnPropertyDelegateMetaEnum::applyAttributesImpl(
	const QtnPropertyDelegateAttributes &attributes)
{
	if (!qtnGetAttribute(attributes, translateAttribute(), mShouldTranslate))
		mShouldTranslate = false;
}

QtnPropertyDelegateMetaEnum::EditorHandler::EditorHandler(
	QtnPropertyDelegateMetaEnum *owner, QComboBox &editor)
	: QtnPropertyEditorHandlerBase(*owner->ownerProperty, editor)
	, mOwner(owner)
	, updating(0)
{
	updateEditor();

	QObject::connect(&editor,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &EditorHandler::onCurrentIndexChanged);
}

QComboBox &QtnPropertyDelegateMetaEnum::EditorHandler::comboBox() const
{
	return *static_cast<QComboBox *>(m_editor);
}

void QtnPropertyDelegateMetaEnum::EditorHandler::updateEditor()
{
	++updating;

	if (m_property->valueIsHidden())
		comboBox().setCurrentIndex(-1);
	else
	{
		int index = comboBox().findData(mOwner->currentValue());

		if (index >= 0)
			comboBox().setCurrentIndex(index);
	}

	--updating;
}

void QtnPropertyDelegateMetaEnum::EditorHandler::updateValue(int value)
{
	if (updating > 0)
		return;

	if (m_property)
		m_property->fromVariant(value, true);
}

void QtnPropertyDelegateMetaEnum::EditorHandler::onCurrentIndexChanged(
	int index)
{
	if (index < 0)
		return;

	QVariant data = comboBox().itemData(index);

	if (data.canConvert<int>())
		updateValue(data.toInt());
}
