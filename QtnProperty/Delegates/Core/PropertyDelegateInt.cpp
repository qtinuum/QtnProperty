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

#include "PropertyDelegateInt.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/Delegates/Utils/PropertyDelegateSliderBox.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/MultiProperty.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QCoreApplication>
#include <QSpinBox>
#include <QLocale>

QByteArray qtnSpinBoxDelegate()
{
	return QByteArrayLiteral("SpinBox");
}

QByteArray qtnSliderBoxDelegate()
{
	return QByteArrayLiteral("SliderBox");
}

QByteArray qtnSuffixAttr()
{
	return QByteArrayLiteral("suffix");
}

QByteArray qtnMultiplierAttr()
{
	return QByteArrayLiteral("multiplier");
}

QByteArray qtnMinAttr()
{
	return QByteArrayLiteral("min");
}

QByteArray qtnMaxAttr()
{
	return QByteArrayLiteral("max");
}

void qtnInitPercentSpinBoxDelegate(QtnPropertyDelegateInfo &delegate)
{
	delegate.name = qtnSpinBoxDelegate();
	delegate.attributes[qtnSuffixAttr()] = QLocale().percent();
}

void qtnInitDegreeSpinBoxDelegate(QtnPropertyDelegateInfo &delegate)
{
	delegate.name = qtnSpinBoxDelegate();
	delegate.attributes[qtnSuffixAttr()] = QString::fromUtf8("º");
}

class QtnPropertyIntSpinBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyIntBase, QSpinBox>
{
public:
	QtnPropertyIntSpinBoxHandler(
		QtnPropertyDelegate *delegate, QSpinBox &editor);

protected:
	virtual void updateEditor() override;
};

QtnPropertyDelegateInt::QtnPropertyDelegateInt(QtnPropertyIntBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyIntBase>(owner)
{
}

void QtnPropertyDelegateInt::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyIntBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateInt, QtnPropertyIntBase>,
		qtnSpinBoxDelegate());

	factory.registerDelegate(&QtnPropertyIntBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateSlideBoxTyped<QtnPropertyIntBase>,
			QtnPropertyIntBase>,
		qtnSliderBoxDelegate());
}

QWidget *QtnPropertyDelegateInt::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto spinBox = new QSpinBox(parent);
	spinBox->setSuffix(m_suffix);
	spinBox->setGeometry(rect);

	new QtnPropertyIntSpinBoxHandler(this, *spinBox);

	spinBox->selectAll();

	if (stateProperty()->isEditableByUser())
		qtnInitNumEdit(spinBox, inplaceInfo, NUM_SIGNED_INT);

	return spinBox;
}

bool QtnPropertyDelegateInt::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	if (QtnPropertyDelegateTyped<
			QtnPropertyIntBase>::acceptKeyPressedForInplaceEditImpl(keyEvent))
	{
		return true;
	}

	return qtnAcceptForNumEdit(keyEvent, NUM_SIGNED_INT);
}

void QtnPropertyDelegateInt::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnSuffixAttr(), m_suffix);
}

bool QtnPropertyDelegateInt::propertyValueToStrImpl(QString &strValue) const
{
	strValue = QLocale().toString(owner().value());
	return true;
}

QtnPropertyIntSpinBoxHandler::QtnPropertyIntSpinBoxHandler(
	QtnPropertyDelegate *delegate, QSpinBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
{
	if (!stateProperty()->isEditableByUser())
		editor.setReadOnly(true);

	auto &p = property();
	editor.setRange(p.minValue(), p.maxValue());
	editor.setSingleStep(p.stepValue());

	updateEditor();

	editor.setKeyboardTracking(false);
	editor.installEventFilter(this);
	QObject::connect(&editor,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
		&QtnPropertyIntSpinBoxHandler::onValueChanged);
}

void QtnPropertyIntSpinBoxHandler::updateEditor()
{
	updating++;

	if (stateProperty()->isMultiValue())
	{
		editor().setValue(editor().minimum());
		editor().setSpecialValueText(
			QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		editor().setValue(property().value());
		editor().setSpecialValueText(QString());
	}

	editor().selectAll();

	updating--;
}
