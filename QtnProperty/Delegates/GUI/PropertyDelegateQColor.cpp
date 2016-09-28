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

#include "PropertyDelegateQColor.h"
#include "GUI/PropertyQColor.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorHandler.h"
#include "Delegates/PropertyEditorAux.h"

#include <QColorDialog>

class QtnPropertyQColorLineEditBttnHandler
	: public QtnPropertyEditorBttnHandler<QtnPropertyQColorBase, QtnLineEditBttn>
{
public:
	QtnPropertyQColorLineEditBttnHandler(QtnPropertyQColorBase &property,
										 QtnLineEditBttn &editor);

protected:
	virtual void onToolButtonClick() override;
	virtual void updateEditor() override;

private:
	void onToolButtonClicked(bool);
	void onEditingFinished();
};

static bool regQColorDelegate = QtnPropertyDelegateFactory::staticInstance()
								.registerDelegateDefault(&QtnPropertyQColorBase::staticMetaObject
								, &qtnCreateDelegate<QtnPropertyDelegateQColor, QtnPropertyQColorBase>
								, "LineEditBttn");

QtnPropertyDelegateQColor::QtnPropertyDelegateQColor(QtnPropertyQColorBase& owner)
	: QtnPropertyDelegateTyped<QtnPropertyQColorBase>(owner),
	  m_shape(QtnColorDelegateShapeSquare)
{
}

void QtnPropertyDelegateQColor::applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes)
{
	qtnGetAttribute(attributes, "shape", m_shape);
}

void QtnPropertyDelegateQColor::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
	QColor value = owner().value();

	QRect textRect = rect;

	if (m_shape != QtnColorDelegateShapeNone)
	{
		QRect colorRect = rect;
		colorRect.setRight(colorRect.left() + colorRect.height());
		colorRect.adjust(2, 2, -2, -2);

		if (m_shape == QtnColorDelegateShapeSquare)
		{
			painter.fillRect(colorRect, Qt::black);
			colorRect.adjust(1, 1, -1, -1);
			painter.fillRect(colorRect, value);
		}
		else if (m_shape == QtnColorDelegateShapeCircle)
		{
			bool oldAntiAliasing = painter.testRenderHint(QPainter::Antialiasing);
			painter.setRenderHint(QPainter::Antialiasing);

			QPainterPath path;
			path.addEllipse(colorRect);
			painter.fillPath(path, value);
			painter.drawEllipse(colorRect);

			painter.setRenderHint(QPainter::Antialiasing, oldAntiAliasing);
		}

		textRect.setLeft(colorRect.right() + 3);
	}

	if (textRect.isValid())
	{
		QtnPropertyDelegateTyped<QtnPropertyQColorBase>::drawValueImpl(painter, textRect, state, needTooltip);
	}
}

QWidget* QtnPropertyDelegateQColor::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
	QtnLineEditBttn* editor = new QtnLineEditBttn(parent);
	editor->setGeometry(rect);

	new QtnPropertyQColorLineEditBttnHandler(owner(), *editor);

	if (inplaceInfo)
	{
		editor->lineEdit->selectAll();
	}

	return editor;
}

bool QtnPropertyDelegateQColor::propertyValueToStr(QString& strValue) const
{
	strValue = owner().value().name();
	return true;
}

QtnPropertyQColorLineEditBttnHandler::QtnPropertyQColorLineEditBttnHandler(QtnPropertyQColorBase &property, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(property, editor)
{
	if (!property.isEditableByUser())
	{
		editor.lineEdit->setReadOnly(true);
		editor.toolButton->setEnabled(false);
	}

	updateEditor();
	editor.lineEdit->installEventFilter(this);
	QObject::connect(editor.toolButton, &QToolButton::clicked,
					 this, &QtnPropertyQColorLineEditBttnHandler::onToolButtonClicked);
	QObject::connect(editor.lineEdit, &QLineEdit::editingFinished,
					 this, &QtnPropertyQColorLineEditBttnHandler::onEditingFinished);
}

void QtnPropertyQColorLineEditBttnHandler::onToolButtonClick()
{
	onToolButtonClicked(false);
}

void QtnPropertyQColorLineEditBttnHandler::updateEditor()
{
	editor().setTextForProperty(&property(), property().value().name());
}

void QtnPropertyQColorLineEditBttnHandler::onToolButtonClicked(bool)
{
	auto property = &this->property();
	volatile bool destroyed = false;
	auto connection = QObject::connect(property, &QObject::destroyed, [&destroyed]() mutable
	{
		destroyed = true;
	});
	reverted = true;
	QColorDialog dlg(property->value(), &editor());
	if (dlg.exec() == QDialog::Accepted && !destroyed)
	{
		property->edit(dlg.currentColor());
	}

	if (!destroyed)
		QObject::disconnect(connection);
}

void QtnPropertyQColorLineEditBttnHandler::onEditingFinished()
{
	if (canApply())
		property().edit(QColor(editor().lineEdit->text()));

	applyReset();
}
