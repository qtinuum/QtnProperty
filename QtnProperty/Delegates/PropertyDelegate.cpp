/*******************************************************************************
Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyDelegate.h"
#include "MultiProperty.h"
#include "PropertyEditorAux.h"

#include <QLineEdit>
#include <QKeyEvent>

QString qtnElidedText(const QPainter &painter, const QString &text,
	const QRect &rect, bool *elided)
{
	QString newText =
		painter.fontMetrics().elidedText(text, Qt::ElideRight, rect.width());

	if (elided)
		*elided = (newText != text);

	return newText;
}

QtnPropertyDelegate::~QtnPropertyDelegate() {}

void QtnPropertyDelegate::init()
{
	// do nothing
}

QWidget *QtnPropertyDelegate::createValueEditor(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QWidget *valueEditor = createValueEditorImpl(parent, rect, inplaceInfo);

	if (!valueEditor)
		return valueEditor;

	valueEditor->setObjectName("QtnPropertyValueEditor");
	return valueEditor;
}

bool QtnPropertyDelegate::propertyValueToStr(QString &strValue) const
{
	Q_UNUSED(strValue);
	return false;
}

QtnPropertyDelegate::QtnPropertyDelegate(QtnProperty *ownerProperty)
	: ownerProperty(ownerProperty)
{
}

int QtnPropertyDelegate::subPropertyCountImpl() const
{
	return 0;
}

QtnPropertyBase *QtnPropertyDelegate::subPropertyImpl(int index)
{
	Q_UNUSED(index);
	return nullptr;
}

void QtnPropertyDelegate::applyAttributesImpl(
	const QtnPropertyDelegateAttributes &attributes)
{
	Q_UNUSED(attributes);
}

void QtnPropertyDelegate::drawValueImpl(QStylePainter &painter,
	const QRect &rect, const QStyle::State &state, bool *needTooltip) const
{
	QString strValue;

	if (propertyValueToStr(strValue))
	{
		drawValueText(strValue, painter, rect, state, needTooltip);
	}
}

QString QtnPropertyDelegate::toolTipImpl() const
{
	QString strValue;
	propertyValueToStr(strValue);
	return strValue;
}

bool QtnPropertyDelegate::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	int key = keyEvent->key();
	return (key == Qt::Key_Space || key == Qt::Key_Return);
}

void QtnPropertyDelegate::drawValueText(const QString &text,
	QStylePainter &painter, const QRect &rect, const QStyle::State &state,
	bool *needTooltip)
{
	Q_UNUSED(state);

	if (text.isEmpty())
		return;

	painter.drawText(rect, Qt::AlignLeading | Qt::AlignVCenter,
		qtnElidedText(painter, text, rect, needTooltip));
}

QLineEdit *QtnPropertyDelegate::createValueEditorLineEdit(QWidget *parent,
	const QRect &rect, bool readOnly, QtnInplaceInfo *inplaceInfo)
{
	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setGeometry(rect);
	lineEdit->setReadOnly(readOnly);

	QString strValue;

	if (ownerProperty->valueIsHidden())
	{
		lineEdit->setPlaceholderText(
			QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		propertyValueToStr(strValue);
	}

	lineEdit->setText(strValue);

	qtnInitLineEdit(lineEdit, inplaceInfo);

	return lineEdit;
}

QtnInplaceInfo::QtnInplaceInfo()
	: activationEvent(0)
{
}
