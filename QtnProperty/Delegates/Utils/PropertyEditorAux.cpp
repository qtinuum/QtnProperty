/*******************************************************************************
Copyright (c) 2012-2019 Alex Zhondin <lexxmark.dev@gmail.com>
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

#include "PropertyEditorAux.h"
#include "QtnProperty/Delegates/PropertyDelegate.h"
#include "PropertyDelegateMisc.h"
#include "QtnProperty/MultiProperty.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLocale>
#include <QCoreApplication>
#include <QCompleter>

QtnLineEditBttn::QtnLineEditBttn(
	QWidget *parent, const QString &bttnText, QLineEdit *lineEdit)
	: QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	if (!lineEdit)
		lineEdit = new QLineEdit(this);
	else
		lineEdit->setParent(this);
	this->lineEdit = lineEdit;
	layout->addWidget(lineEdit);

	toolButton = new QToolButton(this);
	toolButton->setText(bttnText);
	toolButton->setFocusPolicy(Qt::StrongFocus);
	layout->addWidget(toolButton);

	setFocusProxy(lineEdit);
	setAutoFillBackground(true);
}

QtnComboBoxBttn::QtnComboBoxBttn(QWidget *parent, const QString &bttnText)
	: QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	comboBox = new QComboBox(this);
	layout->addWidget(comboBox);

	toolButton = new QToolButton(this);
	toolButton->setText(bttnText);
	toolButton->setFocusPolicy(Qt::StrongFocus);
	layout->addWidget(toolButton);

	setFocusProxy(comboBox);
	setAutoFillBackground(true);
}

void QtnLineEditBttn::setTextForProperty(
	QtnPropertyBase *property, const QString &text)
{
	if (property->isMultiValue())
	{
		lineEdit->clear();
		lineEdit->setPlaceholderText(
			QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		lineEdit->setText(text);
		lineEdit->setPlaceholderText(QString());
	}
}

bool qtnAcceptForLineEdit(QKeyEvent *keyEvent)
{
	if (keyEvent->type() != QEvent::KeyPress)
		return false;

	// any printable key press is acceptable
	QString text = keyEvent->text();
	return (text.size() == 1 && text[0].isPrint());
}

void qtnInitLineEdit(QLineEdit *lineEdit, QtnInplaceInfo *inplaceInfo)
{
	if (!lineEdit || !inplaceInfo)
		return;

	if (!lineEdit->isReadOnly() &&
		(inplaceInfo->activationEvent->type() == QEvent::KeyPress))
	{
		QKeyEvent *keyEvent =
			static_cast<QKeyEvent *>(inplaceInfo->activationEvent);

		if (qtnAcceptForLineEdit(keyEvent))
		{
			lineEdit->setText(keyEvent->text());
			auto completer = lineEdit->completer();
			if (completer)
				completer->setCompletionPrefix(lineEdit->text());

			return;
		}
	} else
	{
		lineEdit->selectAll();
	}
}

bool qtnAcceptForNumEdit(QKeyEvent *keyEvent, QtnNumType type)
{
	if (keyEvent->type() != QEvent::KeyPress)
		return false;

	// any numeric key press is acceptable
	QString text = keyEvent->text();

	if (text.size() == 1)
	{
		QChar c = text.at(0);

		QLocale locale;

		switch (type)
		{
			case NUM_FLOAT:
				if (c == QLatin1Char('.') || c == locale.decimalPoint())
				{
					return true;
				}
				// fall through
			case NUM_SIGNED_INT:
				if (c == QLatin1Char('-') || c == QLatin1Char('+') ||
					c == locale.negativeSign() || c == locale.positiveSign())
				{
					return true;
				}
				// fall through
			case NUM_UNSIGNED_INT:
				if (c.isDigit())
				{
					return true;
				}
				break;
		}
	}

	return false;
}

void qtnInitNumEdit(
	QWidget *numEdit, QtnInplaceInfo *inplaceInfo, QtnNumType type)
{
	if (nullptr != inplaceInfo &&
		inplaceInfo->activationEvent->type() == QEvent::KeyPress)
	{
		auto keyEvent = static_cast<QKeyEvent *>(inplaceInfo->activationEvent);

		if (qtnAcceptForNumEdit(keyEvent, type))
		{
			keyEvent = new QKeyEvent(keyEvent->type(), keyEvent->key(),
				keyEvent->modifiers(), keyEvent->text());

			QCoreApplication::postEvent(numEdit, keyEvent);
		}
	}
}

QtnPropertyComboBox::QtnPropertyComboBox(
	QtnPropertyDelegate *delegate, QWidget *parent)
	: QComboBox(parent)
	, m_delegate(delegate)
{
}

void QtnPropertyComboBox::paintEvent(QPaintEvent *event)
{
	auto rect = event->rect();
	QComboBox::paintEvent(event);

	QPainter painter(this);
	if (stateProperty()->isMultiValue())
	{
		if (isEnabled())
		{
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
			auto color =
				palette().color(QPalette::Active, QPalette::PlaceholderText);
#else
			auto color = palette().color(QPalette::Disabled, QPalette::Text);
#endif
			painter.setPen(color);
		}
		qtnDrawValueText(QtnMultiProperty::getMultiValuePlaceholder(), painter,
			rect, style());
	} else if (currentIndex() >= 0)
	{
		customPaint(painter, rect);
	}
}

void QtnPropertyComboBox::customPaint(QPainter &, const QRect &)
{
	// do nothing
}
