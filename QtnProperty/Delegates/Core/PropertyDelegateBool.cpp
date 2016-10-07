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

#include "PropertyDelegateBool.h"

#include "Core/PropertyBool.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorHandler.h"

#include <QStyleOption>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

class QtnPropertyBoolComboBoxHandler: public QtnPropertyEditorHandler<QtnPropertyBoolBase, QComboBox>
{
public:
	QtnPropertyBoolComboBoxHandler(QtnPropertyBoolBase &property, QComboBox &editor);

protected:
	virtual void updateEditor() override;

private:
	void onCurrentIndexChanged(int index);

	unsigned updating;
};

static bool regBoolDelegate = QtnPropertyDelegateFactory::staticInstance()
								.registerDelegateDefault(&QtnPropertyBoolBase::staticMetaObject
								, &qtnCreateDelegate<QtnPropertyDelegateBoolCheck, QtnPropertyBoolBase>
								, "CheckBox");

static bool regBoolDelegateCombobox = QtnPropertyDelegateFactory::staticInstance()
								.registerDelegate(&QtnPropertyBoolBase::staticMetaObject
								, &qtnCreateDelegate<QtnPropertyDelegateBoolCombobox, QtnPropertyBoolBase>
								, "ComboBox");

void QtnPropertyDelegateBoolCheck::drawValueImpl(QStylePainter &painter,
												 const QRect &rect,
												 const QStyle::State &state,
												 bool *needTooltip) const
{
	QStyleOptionButton opt;
	opt.rect = rect;
	opt.state = state;

	bool value = owner().value();
	if (value)
		opt.state |= QStyle::State_On;

	painter.drawControl(QStyle::CE_CheckBox, opt);
}

QWidget* QtnPropertyDelegateBoolCheck::createValueEditorImpl(QWidget *,
															 const QRect &,
															 QtnInplaceInfo *)
{
	if (owner().isEditableByUser())
		owner().edit(!owner().value() || owner().valueIsHidden());
	return nullptr;
}

QtnPropertyDelegateBoolCombobox::QtnPropertyDelegateBoolCombobox(QtnPropertyBoolBase& owner)
	: QtnPropertyDelegateTyped<QtnPropertyBoolBase>(owner)
{
	m_labels[0] = QtnPropertyBool::getBoolText(false, false);
	m_labels[1] = QtnPropertyBool::getBoolText(true, false);
}

void QtnPropertyDelegateBoolCombobox::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
	qtnGetAttribute(attributes, "labelFalse", m_labels[0]);
	qtnGetAttribute(attributes, "labelTrue", m_labels[1]);
}

QWidget* QtnPropertyDelegateBoolCombobox::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
	if (owner().isEditableByUser())
	{
		QComboBox *comboBox = new QComboBox(parent);
		comboBox->addItem(m_labels[1], true);
		comboBox->addItem(m_labels[0], false);

		comboBox->setGeometry(rect);

		// connect widget and property
		new QtnPropertyBoolComboBoxHandler(owner(), *comboBox);

		if (inplaceInfo)
			comboBox->showPopup();

		return comboBox;
	}
	else
	{
		QLineEdit *lineEdit = new QLineEdit(parent);
		lineEdit->setReadOnly(true);
		lineEdit->setText(m_labels[bool(owner()) ? 1 : 0]);

		lineEdit->setGeometry(rect);

		return lineEdit;
	}
}

bool QtnPropertyDelegateBoolCombobox::propertyValueToStr(QString& strValue) const
{
	strValue = m_labels[bool(owner()) ? 1 : 0];
	return true;
}

QtnPropertyBoolComboBoxHandler::QtnPropertyBoolComboBoxHandler(
		QtnPropertyBoolBase &property, QComboBox &editor)
	: QtnPropertyEditorHandlerType(property, editor)
	, updating(0)
{
	updateEditor();

	if (!property.isEditableByUser())
		editor.setDisabled(true);

	QObject::connect(  &editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
					   , this, &QtnPropertyBoolComboBoxHandler::onCurrentIndexChanged);
}

void QtnPropertyBoolComboBoxHandler::updateEditor()
{
	updating++;

	if (property().valueIsHidden())
		editor().setCurrentIndex(-1);
	else
	{
		int index = editor().findData((bool)property());
		if (index >= 0)
			editor().setCurrentIndex(index);
	}

	updating--;
}

void QtnPropertyBoolComboBoxHandler::onCurrentIndexChanged(int index)
{
	if (updating > 0)
		return;

	if (index >= 0)
	{
		QVariant data = editor().itemData(index);
		if (data.canConvert<bool>())
			property().edit(data.toBool());
	}
}
