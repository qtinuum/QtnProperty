/*******************************************************************************
Copyright 2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyDelegateMetaEnum.h"

#include "Delegates/Utils/PropertyEditorHandler.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Property.h"
#include "PropertyDelegateAttrs.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"

#include <QCoreApplication>
#include <QComboBox>
#include <QLineEdit>

class QtnPropertyDelegateMetaEnum::EditorHandler
	: public QtnPropertyEditorHandlerBase
{
	QtnPropertyDelegateMetaEnum *mOwner;

public:
	EditorHandler(QtnPropertyDelegateMetaEnum *delegate, QComboBox &editor);

	QComboBox &comboBox() const;

protected:
	virtual void updateEditor() override;
	void updateValue(int value);

private:
	void onCurrentIndexChanged(int index);

	unsigned updating;
};

QtnPropertyDelegateMetaEnum::QtnPropertyDelegateMetaEnum(
	const QMetaEnum &metaEnum, QtnPropertyBase *property, bool translate)
	: QtnPropertyDelegateWithValueEditor(*property)
	, mMetaEnum(metaEnum)
	, mShouldTranslate(translate)
{
}

void QtnPropertyDelegateMetaEnum::Register(
	QMetaEnum metaEnum, QtnPropertyDelegateFactory *factory, bool translate)
{
	if (!factory)
		factory = &QtnPropertyDelegateFactory::staticInstance();

	factory->registerDelegate(&QtnProperty::staticMetaObject,
		[metaEnum, translate](QtnPropertyBase &owner) -> QtnPropertyDelegate * {
			return new QtnPropertyDelegateMetaEnum(metaEnum, &owner, translate);
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
	propertyImmutable()->toVariant(v);
	return int(v.toLongLong());
}

bool QtnPropertyDelegateMetaEnum::propertyValueToStrImpl(
	QString &strValue) const
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
		? QCoreApplication::translate(mMetaEnum.scope(), key, mMetaEnum.name())
		: QString(QLatin1String(key));
}

QByteArray qtnTranslateAttribute()
{
	return QByteArrayLiteral("translate");
}

QWidget *QtnPropertyDelegateMetaEnum::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QComboBox *combo = new QtnPropertyComboBox(this, parent);
	for (int i = 0, count = mMetaEnum.keyCount(); i < count; i++)
	{
		combo->addItem(
			keyToStr(mMetaEnum.key(i)), QVariant(mMetaEnum.value(i)));
	}

	combo->setGeometry(rect);

	new EditorHandler(this, *combo);

	if (inplaceInfo && stateProperty()->isEditableByUser())
		combo->showPopup();

	return combo;
}

void QtnPropertyDelegateMetaEnum::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnTranslateAttribute(), mShouldTranslate);
}

QtnPropertyDelegateMetaEnum::EditorHandler::EditorHandler(
	QtnPropertyDelegateMetaEnum *delegate, QComboBox &editor)
	: QtnPropertyEditorHandlerBase(delegate, editor)
	, mOwner(delegate)
	, updating(0)
{
	updateEditor();

	QObject::connect(&editor,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &EditorHandler::onCurrentIndexChanged);
}

QComboBox &QtnPropertyDelegateMetaEnum::EditorHandler::comboBox() const
{
	return *static_cast<QComboBox *>(editorBase());
}

void QtnPropertyDelegateMetaEnum::EditorHandler::updateEditor()
{
	++updating;

	if (stateProperty()->isMultiValue())
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

	if (propertyBase())
		propertyBase()->fromVariant(value, delegate()->editReason());
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
