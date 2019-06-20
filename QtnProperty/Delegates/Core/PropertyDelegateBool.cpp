/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com
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

#include "PropertyDelegateBool.h"

#include "QtnProperty/Core/PropertyBool.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QStyleOption>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QMouseEvent>

QByteArray qtnLabelFalseAttr()
{
	return QByteArrayLiteral("labelFalse");
}

QByteArray qtnLabelTrueAttr()
{
	return QByteArrayLiteral("labelTrue");
}

class QtnPropertyBoolComboBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyBoolBase, QComboBox>
{
public:
	QtnPropertyBoolComboBoxHandler(
		QtnPropertyDelegate *delegate, QComboBox &editor);

protected:
	virtual void updateEditor() override;

private:
	void onCurrentIndexChanged(int index);
};

QtnPropertyDelegateBoolCheck::QtnPropertyDelegateBoolCheck(
	QtnPropertyBoolBase &owner)
	: QtnPropertyDelegateTyped(owner)
{
}

void QtnPropertyDelegateBoolCheck::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyBoolBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateBoolCheck, QtnPropertyBoolBase>,
		qtnCheckBoxDelegate());
}

bool QtnPropertyDelegateBoolCheck::createSubItemValueImpl(
	QtnDrawContext &context, QtnSubItem &subItemValue)
{
	subItemValue.trackState();
	subItemValue.rect.setWidth(
		context.style()->pixelMetric(QStyle::PM_IndicatorWidth));

	subItemValue.drawHandler = [this](QtnDrawContext &context,
								   const QtnSubItem &item) {
		QStyleOptionButton opt;
		opt.rect = item.rect;
		opt.state = state(context.isActive, item);

		if (stateProperty()->isMultiValue())
		{
			opt.state |= QStyle::State_NoChange;
		} else
		{
			bool value = owner().value();
			opt.state |= (value ? QStyle::State_On : QStyle::State_Off);
		}

		context.painter->drawControl(QStyle::CE_CheckBox, opt);
	};

	subItemValue.eventHandler = [this](QtnEventContext &context,
									const QtnSubItem &,
									QtnPropertyToEdit *toEdit) {
		bool toggleValue = false;
		switch (context.eventType())
		{
			case QEvent::MouseButtonRelease:
				toggleValue = true;
				break;

			case QEvent::KeyPress:
			{
				int key = context.eventAs<QKeyEvent>()->key();
				toggleValue = (key == Qt::Key_Space) || (key == Qt::Key_Return);
				break;
			}

			default:
				return false;
		}

		if (toggleValue)
		{
			toEdit->setup(property(), [this]() -> QWidget * {
				QtnPropertyDelegateBoolCheck *thiz = this;
				auto &p = thiz->owner();
				p.setValue(!p.value(), editReason());
				return nullptr;
			});
		}

		return true;
	};

	return true;
}

QtnPropertyDelegateBoolCombobox::QtnPropertyDelegateBoolCombobox(
	QtnPropertyBoolBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyBoolBase>(owner)
{
	m_labels[0] = QtnPropertyBool::getBoolText(false, false);
	m_labels[1] = QtnPropertyBool::getBoolText(true, false);
}

void QtnPropertyDelegateBoolCombobox::Register(
	QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegate(&QtnPropertyBoolBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateBoolCombobox,
			QtnPropertyBoolBase>,
		qtnComboBoxDelegate());
}

void QtnPropertyDelegateBoolCombobox::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnLabelFalseAttr(), m_labels[0]);
	info.loadAttribute(qtnLabelTrueAttr(), m_labels[1]);
}

QWidget *QtnPropertyDelegateBoolCombobox::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QComboBox *comboBox = new QComboBox(parent);
	comboBox->addItem(m_labels[0], false);
	comboBox->addItem(m_labels[1], true);

	comboBox->setGeometry(rect);

	// connect widget and property
	new QtnPropertyBoolComboBoxHandler(this, *comboBox);

	if (inplaceInfo && stateProperty()->isEditableByUser())
		comboBox->showPopup();

	return comboBox;
}

bool QtnPropertyDelegateBoolCombobox::propertyValueToStrImpl(
	QString &strValue) const
{
	strValue = m_labels[owner().value() ? 1 : 0];
	return true;
}

QtnPropertyBoolComboBoxHandler::QtnPropertyBoolComboBoxHandler(
	QtnPropertyDelegate *delegate, QComboBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
{
	updateEditor();

	QObject::connect(&editor,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &QtnPropertyBoolComboBoxHandler::onCurrentIndexChanged);
}

void QtnPropertyBoolComboBoxHandler::updateEditor()
{
	updating++;
	editor().setEnabled(stateProperty()->isEditableByUser());

	if (stateProperty()->isMultiValue())
		editor().setCurrentIndex(-1);
	else
	{
		editor().setCurrentIndex(property().value() ? 1 : 0);
	}

	updating--;
}

void QtnPropertyBoolComboBoxHandler::onCurrentIndexChanged(int index)
{
	if (index >= 0)
	{
		auto data = editor().itemData(index);

		if (data.canConvert<bool>())
		{
			onValueChanged(data.toBool());
		}
	}
}
