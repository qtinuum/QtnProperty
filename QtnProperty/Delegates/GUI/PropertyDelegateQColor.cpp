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

#include "PropertyDelegateQColor.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"
#include "QtnProperty/PropertyDelegateAttrs.h"
#include "QtnProperty/Utils/InplaceEditing.h"

#include <QColorDialog>

QByteArray qtnShapeAttr()
{
	return QByteArrayLiteral("shape");
}

QByteArray qtnRgbSubItemsAttr()
{
	return QByteArrayLiteral("rgbSubItems");
}

QByteArray qtnSolidDelegateName()
{
	return QByteArrayLiteral("Solid");
}

QByteArray qtnSelectColorDelegateName()
{
	return QByteArrayLiteral("SelectColor");
}

class QtnPropertyQColorLineEditBttnHandler
	: public QtnPropertyEditorBttnHandler<QtnPropertyQColorBase,
		  QtnLineEditBttn>
{
public:
	QtnPropertyQColorLineEditBttnHandler(
		QtnPropertyDelegate *delegate, QtnLineEditBttn &editor);

protected:
	virtual void onToolButtonClick() override;
	virtual void updateEditor() override;

private:
	void onToolButtonClicked(bool);
	void onEditingFinished();
};

QtnPropertyDelegateQColor::QtnPropertyDelegateQColor(
	QtnPropertyQColorBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQColorBase>(owner)
	, m_shape(QtnColorDelegateShapeSquare)
{
}

void QtnPropertyDelegateQColor::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQColorBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQColor, QtnPropertyQColorBase>,
		qtnSelectColorDelegateName());
}

void QtnPropertyDelegateQColor::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnShapeAttr(), m_shape);

	if (info.getAttribute(qtnRgbSubItemsAttr(), false))
	{
		addSubProperty(owner().createRedProperty());
		addSubProperty(owner().createGreenProperty());
		addSubProperty(owner().createBlueProperty());
	}
}

void QtnPropertyDelegateQColor::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	if (stateProperty()->isMultiValue())
	{
		QtnPropertyDelegateTypedEx::drawValueImpl(painter, rect);
		return;
	}

	QColor value = owner().value();

	QRect textRect = rect;

	if (m_shape != QtnColorDelegateShapeNone)
	{
		QRect colorRect = rect;
		colorRect.setWidth(colorRect.height());
		colorRect.adjust(2, 2, -2, -2);

		if (m_shape == QtnColorDelegateShapeSquare)
		{
			painter.fillRect(colorRect,
				painter.style()->standardPalette().color(
					stateProperty()->isEditableByUser() ? QPalette::Active
														: QPalette::Disabled,
					QPalette::Text));
			colorRect.adjust(1, 1, -1, -1);
			painter.fillRect(colorRect, value);
		} else if (m_shape == QtnColorDelegateShapeCircle)
		{
			auto oldBrush = painter.brush();
			bool oldAntiAliasing =
				painter.testRenderHint(QPainter::Antialiasing);
			painter.setRenderHint(QPainter::Antialiasing);

			painter.setBrush(value);
			painter.drawEllipse(colorRect);

			painter.setRenderHint(QPainter::Antialiasing, oldAntiAliasing);
			painter.setBrush(oldBrush);
		}

		textRect.setLeft(colorRect.right() + 3);
	}

	if (textRect.isValid())
	{
		QtnPropertyDelegateTyped<QtnPropertyQColorBase>::drawValueImpl(
			painter, textRect);
	}
}

QWidget *QtnPropertyDelegateQColor::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QtnLineEditBttn *editor = new QtnLineEditBttn(parent);
	editor->setGeometry(rect);

	new QtnPropertyQColorLineEditBttnHandler(this, *editor);

	if (inplaceInfo)
	{
		editor->lineEdit->selectAll();
	}

	return editor;
}

bool QtnPropertyDelegateQColor::propertyValueToStrImpl(QString &strValue) const
{
	return owner().toStr(strValue);
}

QtnPropertyDelegateQColorSolid::QtnPropertyDelegateQColorSolid(
	QtnPropertyQColorBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyQColorBase>(owner)
{
}

void QtnPropertyDelegateQColorSolid::Register(
	QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegate(&QtnPropertyQColorBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQColorSolid,
			QtnPropertyQColorBase>,
		qtnSolidDelegateName());
}

bool QtnPropertyDelegateQColorSolid::createSubItemValueImpl(
	QtnDrawContext &context, QtnSubItem &subItemValue)
{
	if (!QtnPropertyDelegateTyped<
			QtnPropertyQColorBase>::createSubItemValueImpl(context,
			subItemValue))
		return false;

	// correct left value rect
	subItemValue.rect.setLeft(context.splitPos);
	return true;
}

void QtnPropertyDelegateQColorSolid::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	if (stateProperty()->isMultiValue())
	{
		QtnPropertyDelegateTyped::drawValueImpl(painter, rect);
		return;
	}

	auto boxRect = rect;
	boxRect.adjust(2, 2, -2, -2);
	painter.fillRect(boxRect, owner());
}

QWidget *QtnPropertyDelegateQColorSolid::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	Q_UNUSED(rect);
	Q_UNUSED(inplaceInfo);

	QColorDialog dlg(owner(), parent);
	if (dlg.exec() == QDialog::Accepted)
	{
		owner().setValue(dlg.currentColor(), editReason());
	}

	return nullptr;
}

QtnPropertyQColorLineEditBttnHandler::QtnPropertyQColorLineEditBttnHandler(
	QtnPropertyDelegate *delegate, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(delegate, editor)
{
	if (!stateProperty()->isEditableByUser())
	{
		editor.lineEdit->setReadOnly(true);
		editor.toolButton->setEnabled(false);
	}

	QtnPropertyQColorLineEditBttnHandler::updateEditor();
	editor.lineEdit->installEventFilter(this);
	QObject::connect(editor.toolButton, &QToolButton::clicked, this,
		&QtnPropertyQColorLineEditBttnHandler::onToolButtonClicked);
	QObject::connect(editor.lineEdit, &QLineEdit::editingFinished, this,
		&QtnPropertyQColorLineEditBttnHandler::onEditingFinished);
}

void QtnPropertyQColorLineEditBttnHandler::onToolButtonClick()
{
	onToolButtonClicked(false);
}

void QtnPropertyQColorLineEditBttnHandler::updateEditor()
{
	QString str;
	property().toStr(str);
	editor().setTextForProperty(stateProperty(), str);
	editor().lineEdit->selectAll();
}

void QtnPropertyQColorLineEditBttnHandler::onToolButtonClicked(bool)
{
	auto property = &this->property();
	volatile bool destroyed = false;
	auto connection = QObject::connect(property, &QObject::destroyed,
		[&destroyed]() mutable { destroyed = true; });
	reverted = true;
	auto dialog = new QColorDialog(property->value(), editorBase());
	auto dialogContainer = connectDialog(dialog);

	if (dialog->exec() == QDialog::Accepted && !destroyed)
	{
		property->setValue(dialog->currentColor(), delegate()->editReason());
	}

	if (!destroyed)
		QObject::disconnect(connection);

	Q_UNUSED(dialogContainer);
}

void QtnPropertyQColorLineEditBttnHandler::onEditingFinished()
{
	if (canApply())
	{
		property().fromStr(editor().lineEdit->text(), delegate()->editReason());
	}

	applyReset();
}
