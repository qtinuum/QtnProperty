/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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

#include "PropertyDelegateEnum.h"
#include "Core/PropertyEnum.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorHandler.h"

#include <QComboBox>
#include <QLineEdit>

class QtnPropertyEnumComboBoxHandler
	: public QtnPropertyEditorHandler<QtnPropertyEnumBase, QComboBox>
{
public:
	QtnPropertyEnumComboBoxHandler(QtnPropertyEnumBase &property, QComboBox &editor);

protected:
	virtual void updateEditor() override;

private:
	void onCurrentIndexChanged(int index);

	unsigned updating;
};


static bool regEnumDelegate = QtnPropertyDelegateFactory::staticInstance()
								.registerDelegateDefault(&QtnPropertyEnumBase::staticMetaObject
								, &qtnCreateDelegate<QtnPropertyDelegateEnum, QtnPropertyEnumBase>
								, "ComboBox");

QWidget* QtnPropertyDelegateEnum::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
	const QtnEnumInfo* info = owner().enumInfo();

	if (!info)
		return 0;

	if (owner().isEditableByUser())
	{
		QComboBox* combo = new QComboBox(parent);
		info->forEachEnumValue([combo](const QtnEnumValueInfo &value)->bool {
			combo->addItem(value.name(), QVariant(value.value()));
			return true;
		});

		combo->setGeometry(rect);

		new QtnPropertyEnumComboBoxHandler(owner(), *combo);

		if (inplaceInfo)
			combo->showPopup();

		return combo;
	}
	else
	{
		const QtnEnumValueInfo* valueInfo = info->findByValue(owner());
		if (!valueInfo)
			return nullptr;

		return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
	}

}

bool QtnPropertyDelegateEnum::propertyValueToStr(QString& strValue) const
{
	QtnEnumValueType value = owner().value();
	const QtnEnumInfo* info = owner().enumInfo();
	const QtnEnumValueInfo* valueInfo = info ? info->findByValue(value) : 0;

	if (!valueInfo)
		return false;

	strValue = valueInfo->name();
	return true;
}

QtnPropertyEnumComboBoxHandler::QtnPropertyEnumComboBoxHandler(QtnPropertyEnumBase &property, QComboBox &editor)
	: QtnPropertyEditorHandlerType(property, editor)
	, updating(0)
{
	updateEditor();

	if (!property.isEditableByUser())
		editor.setDisabled(true);

	QObject::connect(  &editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
					   , this, &QtnPropertyEnumComboBoxHandler::onCurrentIndexChanged);
}

void QtnPropertyEnumComboBoxHandler::updateEditor()
{
	updating++;

	if (property().valueIsHidden())
		editor().setCurrentIndex(-1);
	else
	{
		int index = editor().findData((int)property());
		if (index >= 0)
			editor().setCurrentIndex(index);
	}

	updating--;
}

void QtnPropertyEnumComboBoxHandler::onCurrentIndexChanged(int index)
{
	if (updating)
		return;

	if (index >= 0)
	{
		QVariant data = editor().itemData(index);
		if (data.canConvert<int>())
			property().edit(data.toInt());
	}
}
