/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyDelegateIntList.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Core/PropertyInt.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include <QComboBox>
#include <QLineEdit>

void regIntListDelegates()
{
	QtnPropertyDelegateFactory::staticInstance().registerDelegate(
		&QtnPropertyIntBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateIntList, QtnPropertyIntBase>,
		"IntList");
}

class QtnPropertyIntComboBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyIntBase, QComboBox>
{
public:
	QtnPropertyIntComboBoxHandler(
		QtnPropertyDelegateIntList *delegate, QComboBox &editor)
		: QtnPropertyEditorHandlerVT(delegate, editor)
	{
		updateEditor();

		QObject::connect(&editor,
			static_cast<void (QComboBox::*)(int)>(
				&QComboBox::currentIndexChanged),
			this, &QtnPropertyIntComboBoxHandler::onCurrentIndexChanged);
	}

private:
	void updateEditor() override
	{
		updating++;

		editor().setEnabled(stateProperty()->isEditableByUser());
		if (stateProperty()->isMultiValue())
			editor().setCurrentIndex(-1);
		else
		{
			int index = editor().findData((int) property());
			editor().setCurrentIndex(index);
		}
		updating--;
	}

	void onCurrentIndexChanged(int index)
	{
		if (index >= 0)
		{
			QVariant data = editor().itemData(index);
			if (data.canConvert<int>())
				onValueChanged(data.toInt());
		}
	}
};

QtnPropertyDelegateIntList::QtnPropertyDelegateIntList(
	QtnPropertyIntBase &owner)
	: QtnPropertyDelegateInt(owner)
{
}

QWidget *QtnPropertyDelegateIntList::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QComboBox *comboBox = new QComboBox(parent);

	auto delegate = owner().delegateInfo();
	if (delegate)
	{
		QList<int> values;
		delegate->loadAttribute("values", values);
		for (auto value : values)
		{
			comboBox->addItem(QString::number(value), value);
		}
	}

	comboBox->setGeometry(rect);

	// connect widget and property
	new QtnPropertyIntComboBoxHandler(this, *comboBox);

	if (inplaceInfo)
		comboBox->showPopup();

	return comboBox;
}
