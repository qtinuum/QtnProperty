
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

#include "PropertyDelegateQBrush.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QComboBox>
#include <QStyledItemDelegate>
#include <QPaintEvent>

QByteArray qtnShowAllAttr()
{
	return QByteArrayLiteral("showAll");
}

void QtnPropertyDelegateQBrushStyle::Register(
	QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(
		&QtnPropertyQBrushStyleBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQBrushStyle,
			QtnPropertyQBrushStyleBase>,
		qtnComboBoxDelegate());
}

static void drawBrushStyle(
	QStyle *style, QPainter &painter, QRect rect, Qt::BrushStyle brushStyle)
{
	switch (brushStyle)
	{
		case Qt::NoBrush:
		case Qt::LinearGradientPattern:
		case Qt::RadialGradientPattern:
		case Qt::ConicalGradientPattern:
		case Qt::TexturePattern:
		{
			QString str;
			QtnPropertyQBrushStyle::translateBrushStyle(brushStyle, str);
			qtnDrawValueText(str, painter, rect, style);
			break;
		}

		default:
		{
			rect.adjust(2, 2, -2, -2);
			auto brush = painter.brush();
			brush.setStyle(brushStyle);
			auto oldBrush = painter.brush();
			painter.setBrush(brush);
			painter.drawRect(rect);
			painter.setBrush(oldBrush);
		}
	}
}

class QtnPropertyBrushStyleItemDelegate : public QStyledItemDelegate
{
	QComboBox *m_owner;

public:
	QtnPropertyBrushStyleItemDelegate(QComboBox *owner);
	void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;
};

class QtnPropertyBrushStyleComboBox : public QtnPropertyComboBox
{
public:
	explicit QtnPropertyBrushStyleComboBox(
		QtnPropertyDelegate *delegate, QWidget *parent = Q_NULLPTR);

protected:
	virtual void customPaint(QPainter &painter, const QRect &rect) override;
};

class QtnPropertyBrushStyleComboBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyQBrushStyleBase, QComboBox>
{
public:
	QtnPropertyBrushStyleComboBoxHandler(
		QtnPropertyDelegate *delegate, QComboBox &editor);

private:
	void updateEditor() override;

	void onCurrentIndexChanged(int index);
};

QtnPropertyDelegateQBrushStyle::QtnPropertyDelegateQBrushStyle(
	QtnPropertyQBrushStyleBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyQBrushStyleBase>(owner)
	, m_showAll(false)
{
}

void QtnPropertyDelegateQBrushStyle::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnShowAllAttr(), m_showAll);
}

void QtnPropertyDelegateQBrushStyle::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	if (stateProperty()->isMultiValue())
	{
		QtnPropertyDelegateTyped::drawValueImpl(painter, rect);
	} else
	{
		auto value = owner().value();
		drawBrushStyle(painter.style(), painter, rect, value);
	}
}

QWidget *QtnPropertyDelegateQBrushStyle::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	if (stateProperty()->isEditableByUser())
	{
		QComboBox *combo = new QtnPropertyBrushStyleComboBox(this, parent);
		combo->setLineEdit(nullptr);
		combo->setItemDelegate(new QtnPropertyBrushStyleItemDelegate(combo));
		if (m_showAll)
		{
			for (auto bs = Qt::NoBrush; bs <= Qt::ConicalGradientPattern;
				 bs = Qt::BrushStyle(bs + 1))
			{
				combo->addItem(QString(), QVariant::fromValue(bs));
			}
			combo->addItem(QString(), QVariant::fromValue(Qt::TexturePattern));
		} else
		{
			combo->addItem(QString(), QVariant::fromValue(Qt::NoBrush));
			combo->addItem(QString(), QVariant::fromValue(Qt::SolidPattern));
			combo->addItem(QString(), QVariant::fromValue(Qt::HorPattern));
			combo->addItem(QString(), QVariant::fromValue(Qt::VerPattern));
			combo->addItem(QString(), QVariant::fromValue(Qt::CrossPattern));
			combo->addItem(QString(), QVariant::fromValue(Qt::BDiagPattern));
			combo->addItem(QString(), QVariant::fromValue(Qt::FDiagPattern));
			combo->addItem(
				QString(), QVariant::fromValue(Qt::DiagCrossPattern));
		}

		combo->setGeometry(rect);

		new QtnPropertyBrushStyleComboBoxHandler(this, *combo);

		if (inplaceInfo && stateProperty()->isEditableByUser())
			combo->showPopup();

		return combo;
	}

	return nullptr;
}

bool QtnPropertyDelegateQBrushStyle::propertyValueToStrImpl(
	QString &strValue) const
{
	return QtnPropertyQBrushStyle::translateBrushStyle(
		owner().value(), strValue);
}

QtnPropertyBrushStyleComboBoxHandler::QtnPropertyBrushStyleComboBoxHandler(
	QtnPropertyDelegate *delegate, QComboBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
{
	updateEditor();

	QObject::connect(&editor,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &QtnPropertyBrushStyleComboBoxHandler::onCurrentIndexChanged);
}

void QtnPropertyBrushStyleComboBoxHandler::updateEditor()
{
	updating++;

	if (stateProperty()->isMultiValue())
		editor().setCurrentIndex(-1);
	else
	{
		int index = editor().findData(QVariant::fromValue(property().value()));
		editor().setCurrentIndex(index);
	}

	updating--;
}

void QtnPropertyBrushStyleComboBoxHandler::onCurrentIndexChanged(int index)
{
	if (index >= 0)
	{
		QVariant data = editor().itemData(index);

		if (data.canConvert<Qt::BrushStyle>())
			onValueChanged(data.value<Qt::BrushStyle>());
	}
}

QtnPropertyBrushStyleComboBox::QtnPropertyBrushStyleComboBox(
	QtnPropertyDelegate *delegate, QWidget *parent)
	: QtnPropertyComboBox(delegate, parent)
{
}

void QtnPropertyBrushStyleComboBox::customPaint(
	QPainter &painter, const QRect &rect)
{
	auto brushStyle = currentData().value<Qt::BrushStyle>();
	drawBrushStyle(style(), painter, rect, brushStyle);
}

QtnPropertyBrushStyleItemDelegate::QtnPropertyBrushStyleItemDelegate(
	QComboBox *owner)
	: m_owner(owner)
{
}

void QtnPropertyBrushStyleItemDelegate::paint(QPainter *painter,
	const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	auto brushStyle = index.data(Qt::UserRole).value<Qt::BrushStyle>();
	drawBrushStyle(m_owner->style(), *painter, option.rect, brushStyle);
}
