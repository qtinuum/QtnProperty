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

#include "PropertyDelegateQPen.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"
#include "QtnProperty/PropertyDelegateAttrs.h"
#include "QtnProperty/Core/PropertyDouble.h"
#include "QtnProperty/Core/PropertyQSize.h"
#include "QtnProperty/Core/PropertyEnum.h"
#include "QtnProperty/GUI/PropertyQColor.h"
#include "QtnProperty/MultiProperty.h"

#include <QComboBox>
#include <QStyledItemDelegate>
#include <QPaintEvent>
#include <QAbstractItemView>

QByteArray qtnShowNoPenAttr()
{
	return QByteArrayLiteral("showNoPen");
}

QByteArray qtnEditColorAttr()
{
	return QByteArrayLiteral("editColor");
}

QByteArray qtnEditStyleAttr()
{
	return QByteArrayLiteral("editStyle");
}

QByteArray qtnEditCapStyleAttr()
{
	return QByteArrayLiteral("editCapStyle");
}

QByteArray qtnEditJoinStyleAttr()
{
	return QByteArrayLiteral("editJoinStyle");
}

QByteArray qtnEditWidthAttr()
{
	return QByteArrayLiteral("editWidth");
}

void QtnPropertyDelegateQPenStyle::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQPenStyleBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQPenStyle,
			QtnPropertyQPenStyleBase>,
		qtnComboBoxDelegate());
}

static void drawPenStyle(QPainter &painter, QRect rect, Qt::PenStyle penStyle)
{
	QPen pen = painter.pen();
	pen.setStyle(penStyle);
	painter.save();
	painter.setPen(pen);
	auto midY = rect.center().y();
	painter.drawLine(rect.left(), midY, rect.right(), midY);
	painter.restore();
}

class QtnPropertyPenStyleItemDelegate : public QStyledItemDelegate
{
public:
	void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;
};

class QtnPropertyPenStyleComboBox : public QtnPropertyComboBox
{
public:
	explicit QtnPropertyPenStyleComboBox(
		QtnPropertyDelegate *delegate, QWidget *parent = Q_NULLPTR);

protected:
	virtual void customPaint(QPainter &painter, const QRect &rect) override;
};

class QtnPropertyPenStyleComboBoxHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyQPenStyleBase, QComboBox>
{
public:
	QtnPropertyPenStyleComboBoxHandler(
		QtnPropertyDelegate *delegate, QComboBox &editor);

private:
	void updateEditor() override;

	void onCurrentIndexChanged(int index);
};

QtnPropertyDelegateQPenStyle::QtnPropertyDelegateQPenStyle(
	QtnPropertyQPenStyleBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyQPenStyleBase>(owner)
	, m_showNoPen(false)
{
}

void QtnPropertyDelegateQPenStyle::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	info.loadAttribute(qtnShowNoPenAttr(), m_showNoPen);
}

void QtnPropertyDelegateQPenStyle::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	if (stateProperty()->isMultiValue())
	{
		QtnPropertyDelegateTyped::drawValueImpl(painter, rect);
		return;
	}

	Qt::PenStyle value = owner().value();
    int btnMargin = painter.style()->pixelMetric(QStyle::PM_ButtonMargin);
    QMargins margins(btnMargin, 0, btnMargin, 0);
    drawPenStyle(painter, rect.marginsRemoved(margins), value);
}

QWidget *QtnPropertyDelegateQPenStyle::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	if (stateProperty()->isEditableByUser())
	{
		QComboBox *combo = new QtnPropertyPenStyleComboBox(this, parent);
		combo->setLineEdit(nullptr);
		combo->setItemDelegate(new QtnPropertyPenStyleItemDelegate());
		auto startStyle = m_showNoPen ? Qt::NoPen : Qt::SolidLine;
		for (auto ps = (int) startStyle; ps < Qt::CustomDashLine; ++ps)
			combo->addItem(QString(), QVariant::fromValue(Qt::PenStyle(ps)));

		combo->setGeometry(rect);

		new QtnPropertyPenStyleComboBoxHandler(this, *combo);

		if (inplaceInfo && stateProperty()->isEditableByUser())
			combo->showPopup();

		return combo;
	}

	return nullptr;
}

bool QtnPropertyDelegateQPenStyle::propertyValueToStrImpl(
	QString &strValue) const
{
	auto info =
		QtnPropertyQPenBase::penStyleEnum().findByValue(int(owner().value()));
	if (!info)
		return false;

	strValue = info->displayName();
	return true;
}

void QtnPropertyDelegateQPen::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQPenBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQPen, QtnPropertyQPenBase>,
		qtnLineEditDelegate());
}

class QtnPropertyQPenLineEditHandler
	: public QtnPropertyEditorHandler<QtnPropertyQPenBase, QLineEdit>
{
public:
	QtnPropertyQPenLineEditHandler(
		QtnPropertyDelegate *delegate, QLineEdit &editor);

private:
	void updateEditor() override;
};

QtnPropertyDelegateQPen::QtnPropertyDelegateQPen(QtnPropertyQPenBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQPenBase>(owner)
{
}

void QtnPropertyDelegateQPen::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	auto owner = &this->owner();

	{
		bool editColor = true;
		info.loadAttribute(qtnEditColorAttr(), editColor);

		if (editColor)
		{
			addSubProperty(
				qtnCreateFieldProperty<QtnPropertyQColorCallback>(owner,
					&QPen::color, &QPen::setColor, QtnPropertyQPen::colorKey(),
					QtnPropertyQPen::colorDisplayName(),
					QtnPropertyQPen::colorDescriptionFmt()));
		}
	}

	{
		bool editStyle = true;
		info.loadAttribute(qtnEditStyleAttr(), editStyle);

		if (editStyle)
		{
			addSubProperty(
				qtnCreateFieldProperty<QtnPropertyQPenStyleCallback>(owner,
					&QPen::style, &QPen::setStyle, QtnPropertyQPen::styleKey(),
					QtnPropertyQPen::styleDisplayName(),
					QtnPropertyQPen::styleDescriptionFmt()));
		}
	}

	{
		bool editWidth = true;
		info.loadAttribute(qtnEditWidthAttr(), editWidth);

		if (editWidth)
		{
			auto widthProperty =
				qtnCreateFieldProperty<QtnPropertyDoubleCallback>(owner,
					&QPen::widthF, &QPen::setWidthF,
					QtnPropertyQSize::widthKey(),
					QtnPropertyQSize::widthDisplayName(),
					QtnPropertyQSize::widthDescriptionFmt());
			addSubProperty(widthProperty);

			widthProperty->setMinValue(0.0);
		}
	}

	{
		bool editCapStyle = true;
		info.loadAttribute(qtnEditCapStyleAttr(), editCapStyle);

		if (editCapStyle)
		{
			static_assert(
				sizeof(Qt::PenCapStyle) == sizeof(int), "enum size mismatch");
			auto capStyleProperty =
				qtnCreateFieldProperty<QtnPropertyEnumCallback>(owner,
					reinterpret_cast<int (QPen::*)() const>(&QPen::capStyle),
					reinterpret_cast<void (QPen::*)(int)>(&QPen::setCapStyle),
					QtnPropertyQPen::capStyleKey(),
					QtnPropertyQPen::capStyleDisplayName(),
					QtnPropertyQPen::capStyleDescriptionFmt());
			capStyleProperty->setEnumInfo(&QtnPropertyQPen::penCapStyleEnum());
			addSubProperty(capStyleProperty);
		}
	}

	{
		bool editJoinStyle = true;
		info.loadAttribute(qtnEditJoinStyleAttr(), editJoinStyle);

		if (editJoinStyle)
		{
			static_assert(
				sizeof(Qt::PenJoinStyle) == sizeof(int), "enum size mismatch");
			auto joinStyleProperty =
				qtnCreateFieldProperty<QtnPropertyEnumCallback>(owner,
					reinterpret_cast<int (QPen::*)() const>(&QPen::joinStyle),
					reinterpret_cast<void (QPen::*)(int)>(&QPen::setJoinStyle),
					QtnPropertyQPen::joinStyleKey(),
					QtnPropertyQPen::joinStyleDisplayName(),
					QtnPropertyQPen::joinStyleDescriptionFmt());
			joinStyleProperty->setEnumInfo(
				&QtnPropertyQPen::penJoinStyleEnum());
			addSubProperty(joinStyleProperty);
		}
	}
}

void QtnPropertyDelegateQPen::drawValueImpl(
    QStylePainter &painter, const QRect &rect) const
{
    if (stateProperty()->isMultiValue())
    {
        QtnPropertyDelegateTyped::drawValueImpl(painter, rect);
        return;
    }

    QPen pen = owner().value();
    int btnMargin = painter.style()->pixelMetric(QStyle::PM_ButtonMargin);
    QMargins margins(btnMargin, 0, btnMargin, 0);

    painter.save();
    painter.setPen(pen);
    drawPenStyle(painter, rect.marginsRemoved(margins), pen.style());
    painter.restore();
}

QWidget *QtnPropertyDelegateQPen::createValueEditorImpl(
    QWidget *parent, const QRect &rect, QtnInplaceInfo *)
{
    return nullptr;
}

bool QtnPropertyDelegateQPen::propertyValueToStrImpl(QString &strValue) const
{
	strValue = QtnPropertyQPen::rootDisplayValue();
	return true;
}

void QtnPropertyPenStyleItemDelegate::paint(QPainter *painter,
	const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	auto penStyle = index.data(Qt::UserRole).value<Qt::PenStyle>();
    drawPenStyle(*painter, option.rect.adjusted(2, 0, -2, 0), penStyle);
}

QtnPropertyPenStyleComboBox::QtnPropertyPenStyleComboBox(
	QtnPropertyDelegate *delegate, QWidget *parent)
	: QtnPropertyComboBox(delegate, parent)
{
}

void QtnPropertyPenStyleComboBox::customPaint(
	QPainter &painter, const QRect &rect)
{
    auto itemView = view();
    auto style = itemView->style();
	auto penStyle = currentData().value<Qt::PenStyle>();
    int btnMargin = style->pixelMetric(QStyle::PM_ButtonMargin);
    int indMargin = style->pixelMetric(QStyle::PM_IndicatorWidth);
    QMargins margins(btnMargin, 0, btnMargin + indMargin, 0);
    drawPenStyle(painter, rect.marginsRemoved(margins), penStyle);
}

QtnPropertyPenStyleComboBoxHandler::QtnPropertyPenStyleComboBoxHandler(
	QtnPropertyDelegate *delegate, QComboBox &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
{
	updateEditor();

	QObject::connect(&editor,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &QtnPropertyPenStyleComboBoxHandler::onCurrentIndexChanged);
}

void QtnPropertyPenStyleComboBoxHandler::updateEditor()
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

void QtnPropertyPenStyleComboBoxHandler::onCurrentIndexChanged(int index)
{
	if (index >= 0)
	{
		QVariant data = editor().itemData(index);

		if (data.canConvert<Qt::PenStyle>())
			onValueChanged(data.value<Qt::PenStyle>());
	}
}

QtnPropertyQPenLineEditHandler::QtnPropertyQPenLineEditHandler(
	QtnPropertyDelegate *delegate, QLineEdit &editor)
	: QtnPropertyEditorHandlerType(delegate, editor)
{
	editor.setReadOnly(true);
	editor.setPlaceholderText(stateProperty()->isMultiValue()
			? QtnMultiProperty::getMultiValuePlaceholder()
			: QtnPropertyQPen::rootDisplayValue());
}

void QtnPropertyQPenLineEditHandler::updateEditor()
{
	// do nothing
}
