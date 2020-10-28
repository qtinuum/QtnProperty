/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
		QtnPropertyDelegateInt *delegate, QSpinBox &editor);

protected:
	virtual void updateEditor() override;

private:
	QtnPropertyDelegateInt *m_delegate;
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

int QtnPropertyDelegateInt::minValue() const
{
	return m_min.isValid() ? m_min.toInt() : owner().minValue();
}

int QtnPropertyDelegateInt::maxValue() const
{
	return m_max.isValid() ? m_max.toInt() : owner().maxValue();
}

int QtnPropertyDelegateInt::currentValue() const
{
	return std::min(maxValue(), std::max(minValue(), owner().value()));
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
	m_min = info.attributes.value(qtnMinAttr());
	m_max = info.attributes.value(qtnMaxAttr());
	fixMinMaxVariant<int>(m_min, m_max);
}

bool QtnPropertyDelegateInt::propertyValueToStrImpl(QString &strValue) const
{
	strValue = QLocale().toString(currentValue());
	return true;
}

QtnPropertyIntSpinBoxHandler::QtnPropertyIntSpinBoxHandler(
	QtnPropertyDelegateInt *delegate, QSpinBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
	, m_delegate(delegate)
{
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

	editor().setReadOnly(!stateProperty()->isEditableByUser());
	editor().setSingleStep(property().stepValue());
	editor().setRange(m_delegate->minValue(), m_delegate->maxValue());

	if (stateProperty()->isMultiValue())
	{
		editor().setValue(editor().minimum());
		editor().setSpecialValueText(
			QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		editor().setValue(m_delegate->currentValue());
		editor().setSpecialValueText(QString());
	}

	editor().selectAll();

	updating--;
}
