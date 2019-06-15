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

#include "PropertyDelegateEnum.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/Utils/PropertyEditorHandler.h"
#include "Delegates/Utils/PropertyEditorAux.h"

#include <QComboBox>
#include <QLineEdit>

class QtnPropertyEnumComboBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyEnumBase, QComboBox>
{
public:
	QtnPropertyEnumComboBoxHandler(
		QtnPropertyDelegate *delegate, QComboBox &editor);

protected:
	virtual void updateEditor() override;

private:
	void onCurrentIndexChanged(int index);
};

void QtnPropertyDelegateEnum::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyEnumBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateEnum, QtnPropertyEnumBase>,
		qtnComboBoxDelegate());
}

QtnPropertyDelegateEnum::QtnPropertyDelegateEnum(QtnPropertyEnumBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyEnumBase>(owner)
{
}

QWidget *QtnPropertyDelegateEnum::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	const QtnEnumInfo *info = owner().enumInfo();

	if (!info)
		return 0;

	QComboBox *combo = new QtnPropertyComboBox(this, parent);
	info->forEachEnumValue([combo](const QtnEnumValueInfo &value) -> bool {
		combo->addItem(value.displayName(), QVariant(value.value()));
		return true;
	});

	combo->setGeometry(rect);

	new QtnPropertyEnumComboBoxHandler(this, *combo);

	if (inplaceInfo && stateProperty()->isEditableByUser())
		combo->showPopup();

	return combo;
}

bool QtnPropertyDelegateEnum::propertyValueToStrImpl(QString &strValue) const
{
	const QtnEnumInfo *info = owner().enumInfo();
	const QtnEnumValueInfo *valueInfo =
		info ? info->findByValue(owner().value()) : 0;

	if (!valueInfo)
		return false;

	strValue = valueInfo->displayName();
	return true;
}

QtnPropertyEnumComboBoxHandler::QtnPropertyEnumComboBoxHandler(
	QtnPropertyDelegate *delegate, QComboBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
{
	updateEditor();

	QObject::connect(&editor,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &QtnPropertyEnumComboBoxHandler::onCurrentIndexChanged);
}

void QtnPropertyEnumComboBoxHandler::updateEditor()
{
	updating++;
	editor().setEnabled(stateProperty()->isEditableByUser());

	if (stateProperty()->isMultiValue())
		editor().setCurrentIndex(-1);
	else
	{
		int index = editor().findData(property().value());
		editor().setCurrentIndex(index);
	}

	updating--;
}

void QtnPropertyEnumComboBoxHandler::onCurrentIndexChanged(int index)
{
	if (index >= 0)
	{
		QVariant data = editor().itemData(index);

		if (data.canConvert<int>())
			onValueChanged(data.toInt());
	}
}
