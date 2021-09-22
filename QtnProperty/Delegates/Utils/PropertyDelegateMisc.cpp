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

#include "PropertyDelegateMisc.h"
#include "QtnProperty/PropertyView.h"
#include "QtnProperty/Utils/InplaceEditing.h"
#include "QtnProperty/MultiProperty.h"

#include <QLineEdit>
#include <QKeyEvent>

static QIcon qtnResetIcon;

QIcon QtnPropertyDelegate::resetIcon()
{
	if (!qtnResetIcon.isNull())
		return qtnResetIcon;

	const char iconData16[] =
		"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/"
		"9hAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJ"
		"bWFnZVJlYWR5ccllPAAAAsJJREFUeNp0UktIVGEUPve/"
		"j7l37h2aCSQRzEU46jg5D3QWBUnaWGKE"
		"WTmjNYobF4EERYWYD8bMVUW7XEVGtWsduIloZWoINRkR0oCLWaRCNa/76vzXuRdb+F/O/"
		"V/nfN85"
		"/3eY2tpaOGgwhAFCyKJpmNcNw/iz/"
		"y6bzVozMeHgj+d4kGU5JSvyZ5Zj22mAfWcPYq1tw8ESNiYI"
		"whtJlH673W7T4/EAWp2iKO94gX/IMIywLx440zTtdAVknJLc0rjL5SIIAhzHAc/ze8AsS/"
		"c383/z"
		"XcViMYFHGQcA6wRRFJ9KkjQiu2VAJisA2RwmCkj3oXCo7kr/"
		"ZY+TgfXjuA5MeUSRFeB4DsKRMCQH"
		"EuBv8EPfhUtAWKwUiZIDSUgOJt5jyIYDQFFrampmy2rZcmxqaoSpmcm3eDd96kT7suJRTN"
		"9hH9y6"
		"fQui0ch0WySWLpfLsLW1VQFAqVRdbaVlqKoKvX29q23RWHepVLLSDgQDMJOe2a2uPtIfaY"
		"kuaaoG"
		"9rvZGbDFQlHA+"
		"i0An893p1Qsga7roBENHj959AnveluaQ1l6tl8BS8a19VVAiQAbBTRNgwdz8znq"
		"SFkoIAafDDYez+"
		"qaLiHZKGY6inPCAQgFw3q9v16lwRRkc3Nz1moVBDB0A5rqAwV6jg11V3SJC6Ik"
		"LqD/mANA2c51n12jM33QQr5wEVVZAAYOVWoVUNIJZL6HPULlhrEbY/n/"
		"AOJd8ckz8U5Hb5R0FJtp"
		"F9c/"
		"BZdQQtb72Bss7Y2e8z3Q0Xk67TwiTbO5Ibi0ur6yWFVVNfTq5WvAIMDep2UcpVnRQGpXrw"
		"1C"
		"ajj1DP0/5HK5vQ72+rxAGALffmwIuJ/KfMmMLz5/"
		"QVY+rgDVm2bU2tYKQ8MpI9AcmEeftP9YQ3n7"
		"13YFwOvdW1SYvn7PxHA7gRZHk9AKaEtoc/igy7ZCOzs7Vtw/"
		"AQYASsoRySkHkqIAAAAASUVORK5C"
		"YII=";

	const char iconData32[] =
		"iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAAAXNSR0IArs4c6QAAAARnQU"
		"1BAACx"
		"jwv8YQUAAAVlSURBVFhH1ZdrTJtVGMdP7y29F3qld4kwNgaGWwiYgYKySRZm0LmREca6aD"
		"IBRZcR"
		"xTDJzBLD0ImBbMmInxDj/GLcZJkf1EFU2KIfNGJiQBZmVkYYiUBbSluf5+W8tXcg4gd/"
		"y+Ht+zzn"
		"Pc//POc6shPYbLZ3tVqtjL5uCy59/"
		"ivUanUbCPgZhJRR05bZEQEqlUqoUCpsSqVyzJnlPJebmyuk"
		"rk3ZEQF8AZ+kpaUREMFVKpRvBoKBCbvdnk3dKdkRAaFQiHC5XCIWi4lCoSCQiXypTPqTw+"
		"HooFWS"
		"wqHPLWG1WnMhUAOPx6uBZyF8LRGLIKhSwQTncDiMmGAgSLw+"
		"L1ldWSXLq8s3fR7f8bm5uXu0mSi2"
		"JMBsNmcJBIJzIrHosEgoIgKhgIAIptdYwMe8RxIMBonf7yeeVQ/xeDx/"
		"raysvDQ7OztM3WE2FQBj"
		"eVgsEX8kgT8ggAnGBsYepwKzEQgEiM/rIyAAy/"
		"DMzEwjdTOknAM2p61TKpWOwLiKZXIZk2Y+n88E"
		"RzBAqoJgZkA7MRqN5O2es0bGGEHSLlgd1ga5VP6pTCYjQqEwHHTbgA67w05On3l9SavTHd"
		"Gn60ep"
		"hyGhABxzmMm/"
		"wEwWikSiqFTjENQdrCNFRYUkZ1cOYzv6QiOT5lgwC1VPVBHXi64fZSJZvUajuUtd"
		"YRIKKHis4Aak7ilMeWTPnY84SfsrbV6L1XyBGwxdS083fIf2fZX7YM4Fo9rCoWo5cZzU7n"
		"/6imfZ"
		"1woryENdUcQJcDqdVuj5H9I0KYfH/"
		"2dm796dS3re6ZnkCfjHtErtb9TMUF5RHoRHuC3oKel844zv"
		"0ZzsNq1Ke5maExI3sCaT6SSo53C4G2saC+50Hac7bunTDWWxwRG2HpY9eXvIexf75iB4+"
		"WbBkTgB"
		"PD73CLumsUFczzU11WuGDGM9zIUA44gB6+Fya3i+gZzt6f4qQ63Nh+"
		"B3qDslcQJWVlZtOOkwMLuZ"
		"2B2Oz+DAWaRV4oCJGnqru4sca2rsAaG1qerGEifA6/"
		"Xy8cmmFHuWt3fXFcaZhIv9798vKSmt06r1"
		"3cmylIxYATx2ybECMAvfj92+"
		"zRiTYDFZy3Tpumv0dVtECcCAMpk0nH62DA0NmWiVhCRa31slSgCm"
		"D9brOgZlM4BIJJLnmB/bwGAwaGFFrWeaM0NsMRgNc9QdJm4OWKyWPyMF4JDML8y/"
		"CtcuJa2yJWAn"
		"7YX9hAdP/"
		"E0UcgU58MwBNXWHiRPwZHX16Pr6ejj9HPgX8AdUKo1qgFbZFJ1OV+Nf9zfhNo5bN3Nc"
		"w6ZW8XjFD7RKmDgBRaVFfSWlJUxwZghgTuJGBEKOQlrPQ5WU9z24nNbK5fIRPMAwe2wmLR"
		"YL7Cc1"
		"vbRamDgBuNO5Tp74mgdnQOQwYINwrHbCuN6BIBW0ehiwGfR6/"
		"SWo8yX0VhO5meFSbnE13zUbzTcY"
		"YwRxZwHiXnJnXR25OjXw4UDUssSG/Gt+4lvzERim+3D1ugf+RWjFwOfx8/"
		"CmhEIx5ZHfHXq2nrS2"
		"t+2PPYqRhAKQhUX3y4ODl/o/Hh4JN4bg0KAQdo7geY/"
		"nBp6a2OvImxIGr6yqJF3dXR+YtKZ2xhhD"
		"UgHI/MP589e/uN7Z19uHPabWDbDxjR9QoJVIkQj6m5qbiMvV8kmGWtcI/"
		"oQ7ZEoBiPuhu3lmevry"
		"QP+gYHJiklpTk5OTTU61niL5BXt7cHum5oRsKgBZWFjIDfGCF6Z+"
		"nar99ptbZHx8nMxMz1DvBpmZ"
		"JlIMq6eiopwUlxRPQGJe02v0Y9SdlC0JYHmw9KCQhIKH4LOD8JqHV25Y78x/"
		"RmAofg9xyCiXE/w8"
		"Q2W4ufHFf4zb7ZYtLS1p6Ov/EUL+BmFpBSMtaelKAAAAAElFTkSuQmCC";

	{
		auto bytes = QByteArray::fromBase64(iconData16);
		QPixmap pixmap;
		pixmap.loadFromData(bytes);
		qtnResetIcon.addPixmap(pixmap);
	}

	{
		auto bytes = QByteArray::fromBase64(iconData32);
		QPixmap pixmap;
		pixmap.loadFromData(bytes);
		qtnResetIcon.addPixmap(pixmap);
	}

	return qtnResetIcon;
}

void QtnPropertyDelegateWithValues::createSubItemsImpl(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	addSubItemBackground(context, subItems);
	addSubItemSelection(context, subItems);
	addSubItemBranchNode(context, subItems);
	addSubItemLock(context, subItems);
	addSubItemName(context, subItems);
	addSubItemReset(context, subItems);
	addSubItemValues(context, subItems);
}

bool QtnPropertyDelegateWithValues::isSplittable() const
{
	return true;
}

QtnPropertyDelegateWithValues::QtnPropertyDelegateWithValues(
	QtnPropertyBase &owner)
	: QtnPropertyDelegate(owner)
{
}

void QtnPropertyDelegateWithValues::addSubItemBackground(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	QtnSubItem bgItem(context.rect);

	if (!bgItem.rect.isValid())
		return;

	bgItem.drawHandler = [](QtnDrawContext &context, const QtnSubItem &item) //
	{
		auto &painter = *context.painter;
		const auto &rect = item.rect;
		auto splitPos = context.splitPos;

		QPen oldPen = painter.pen();
		QPen linesPen(context.palette().color(QPalette::Button));
		painter.setPen(linesPen);

		// draw item borders
		painter.drawLine(rect.bottomLeft(), rect.bottomRight());
		painter.drawLine(splitPos, rect.top(), splitPos, rect.bottom());

		painter.setPen(oldPen);
	};

	subItems.append(bgItem);
}

void QtnPropertyDelegateWithValues::addSubItemSelection(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	QtnSubItem selItem(context.rect);

	if (!selItem.rect.isValid())
		return;

	selItem.drawHandler = [](QtnDrawContext &context, const QtnSubItem &item) //
	{
		// highlight background if active property
		if (context.isActive)
		{
			context.painter->fillRect(
				item.rect, context.palette().color(QPalette::Highlight));
		}
	};

	subItems.append(selItem);
}

void QtnPropertyDelegateWithValues::addSubItemName(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	QtnSubItem nameItem(context.rect.marginsRemoved(context.margins));
	nameItem.rect.setRight(context.splitPos);
	nameItem.setPropertyDescriptionAsTooltip(*propertyImmutable());

	if (!nameItem.rect.isValid())
		return;

	nameItem.drawHandler = [this](QtnDrawContext &context,
							   const QtnSubItem &item) {
		context.painter->save();

		context.painter->setPen(
			context.textColorFor(stateProperty()->isEditableByUser()));

		if (!stateProperty()->valueIsDefault())
		{
			auto font = context.painter->font();
			font.setBold(true);
			context.painter->setFont(font);
		}

		context.painter->drawText(item.rect,
			int(Qt::AlignLeading | Qt::AlignVCenter) | Qt::TextSingleLine,
			qtnElidedText(
				*context.painter, property()->displayName(), item.rect));

		context.painter->restore();
	};

	subItems.append(nameItem);
}

void QtnPropertyDelegateWithValues::addSubItemReset(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	if (!stateProperty()->isResettable() || stateProperty()->valueIsDefault())
	{
		return;
	}

	QtnSubItem resetItem(context.rect.marginsRemoved(context.margins));
	resetItem.rect.setLeft(resetItem.rect.right() - resetItem.rect.height());
	if (!resetItem.rect.isValid())
		return;

	resetItem.setTextAsTooltip(QtnPropertyView::tr("Reset to default value"));
	resetItem.trackState();

	resetItem.drawHandler = [this](QtnDrawContext &context,
								const QtnSubItem &item) {
		drawButton(context, item, resetIcon(),
			QtnPropertyView::tr("R", "Reset button text"));
	};

	resetItem.eventHandler = [this](QtnEventContext &context,
								 const QtnSubItem &,
								 QtnPropertyToEdit *toEdit) -> bool {
		bool doClick = false;
		switch (context.eventType())
		{
			case QtnSubItemEvent::ReleaseMouse:
				doClick = true;
				break;
		}

		if (doClick)
		{
			toEdit->setup(stateProperty(), [this]() -> QWidget * {
				stateProperty()->reset(editReason());
				return nullptr;
			});
			return true;
		}

		return false;
	};

	subItems.append(resetItem);
}

void QtnPropertyDelegateWithValues::addSubItemValues(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	auto rect = context.rect.marginsRemoved(context.margins);
	rect.setLeft(context.splitPos);

	if (stateProperty()->isResettable() && !stateProperty()->valueIsDefault())
	{
		rect.setRight(rect.right() - rect.height());
	}

	if (rect.isValid())
		createSubItemValuesImpl(context, rect, subItems);
}

QtnPropertyDelegateWithValue::QtnPropertyDelegateWithValue(
	QtnPropertyBase &owner)
	: QtnPropertyDelegateWithValues(owner)
{
}

void QtnPropertyDelegateWithValue::createSubItemValuesImpl(
	QtnDrawContext &context, const QRect &valueRect,
	QList<QtnSubItem> &subItems)
{
	QtnSubItem subItem(valueRect);
	if (createSubItemValueImpl(context, subItem))
		subItems.append(subItem);
}

bool QtnPropertyDelegateWithValueEditor::propertyValueToStr(
	QString &strValue) const
{
	return propertyValueToStrImpl(strValue);
}

QtnPropertyDelegateWithValueEditor::QtnPropertyDelegateWithValueEditor(
	QtnPropertyBase &owner)
	: QtnPropertyDelegateWithValue(owner)
{
}

bool QtnPropertyDelegateWithValueEditor::propertyValueToStrImpl(
	QString &strValue) const
{
	Q_UNUSED(strValue);
	return false;
}

bool QtnPropertyDelegateWithValueEditor::toolTipImpl(QString &strValue) const
{
	return propertyValueToStrImpl(strValue);
}

bool QtnPropertyDelegateWithValueEditor::createSubItemValueImpl(
	QtnDrawContext &, QtnSubItem &subItemValue)
{
	subItemValue.drawHandler = [this](QtnDrawContext &context,
								   const QtnSubItem &item) {
		// draw property value
		auto oldBrush = context.painter->brush();
		auto oldPen = context.painter->pen();
		auto isNormalText = stateProperty()->isEditableByUser() &&
			!stateProperty()->isMultiValue();
		auto cg = isNormalText ? context.colorGroup() : QPalette::Disabled;
		auto color = context.textColorFor(isNormalText);
		auto bgColor = context.isActive
			? context.palette().color(cg, QPalette::Highlight)
			: context.alternateColor();

		context.painter->setBrush(bgColor);
		context.painter->setPen(color);
		drawValueImpl(*context.painter, item.rect);
		context.painter->setBrush(oldBrush);
		context.painter->setPen(oldPen);
	};

	subItemValue.eventHandler = [this](QtnEventContext &context,
									const QtnSubItem &item,
									QtnPropertyToEdit *propertyToEdit) -> bool {
		bool doEdit = false;
		switch (context.eventType())
		{
			case QEvent::MouseButtonDblClick:
				doEdit = (context.widget->propertyViewStyle() &
					QtnPropertyViewStyleDblClickActivation);
				break;

			case QEvent::MouseButtonRelease:
				doEdit = !(context.widget->propertyViewStyle() &
					QtnPropertyViewStyleDblClickActivation);
				break;

			case QEvent::KeyPress:
				doEdit = acceptKeyPressedForInplaceEditImpl(
					context.eventAs<QKeyEvent>());
				break;
		}

		if (doEdit)
		{
			auto ctx = &context;
			auto it = &item;
			propertyToEdit->setup(property(), [this, ctx, it]() -> QWidget * {
				QtnInplaceInfo inplaceInfo;
				inplaceInfo.activationEvent = ctx->event;
				return createValueEditorImpl(
					ctx->widget->viewport(), it->rect, &inplaceInfo);
			});
			return true;
		}

		return false;
	};

	subItemValue.tooltipHandler = [this](QtnEventContext &,
									  const QtnSubItem &) -> QString {
		if (stateProperty()->isMultiValue())
		{
			return QtnMultiProperty::getMultiValuePlaceholder();
		}
		QString valueText;
		if (!toolTipImpl(valueText))
			return QString();
		return valueText;
	};

	return true;
}

bool QtnPropertyDelegateWithValueEditor::isNormalPainterState(
	const QStylePainter &painter) const
{
	if (!stateProperty())
	{
		return false;
	}

	if (!stateProperty()->isEditableByUser())
	{
		return false;
	}

	auto palette = painter.style()->standardPalette();
	return palette.currentColorGroup() != QPalette::Disabled &&
		painter.brush().color() != palette.color(QPalette::Highlight);
}

bool QtnPropertyDelegateWithValueEditor::isPlaceholderColor() const
{
	return false;
}

void QtnPropertyDelegateWithValueEditor::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	if (stateProperty()->isMultiValue())
	{
		qtnDrawValueText(QtnMultiProperty::getMultiValuePlaceholder(), painter,
			rect, painter.style());
		return;
	}

	QString strValue;
	if (propertyValueToStrImpl(strValue))
	{
		QPen oldPen;
		bool penChanged = false;
		if (isNormalPainterState(painter) && isPlaceholderColor())
		{
			oldPen = painter.pen();
			penChanged = true;
			painter.setPen(disabledTextColor(painter));
		}
		qtnDrawValueText(strValue, painter, rect, painter.style());
		if (penChanged)
		{
			painter.setPen(oldPen);
		}
	}
}

bool QtnPropertyDelegateWithValueEditor::acceptKeyPressedForInplaceEditImpl(
	QKeyEvent *keyEvent) const
{
	int key = keyEvent->key();
    return  (key == Qt::Key_Space) ||
            (key == Qt::Key_Return) ||
            (key == Qt::Key_Enter);
}

QLineEdit *QtnPropertyDelegateWithValueEditor::createValueEditorLineEdit(
	QWidget *parent, const QRect &rect, bool readOnly,
	QtnInplaceInfo *inplaceInfo) const
{
	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setGeometry(rect);
	lineEdit->setReadOnly(readOnly);
	lineEdit->setPlaceholderText(QtnMultiProperty::getMultiValuePlaceholder());

	if (!stateProperty()->isMultiValue())
	{
		QString strValue;
		propertyValueToStrImpl(strValue);
		lineEdit->setText(strValue);
	}

	if (inplaceInfo)
	{
		lineEdit->selectAll();
	}

	return lineEdit;
}

QtnPropertyDelegateError::QtnPropertyDelegateError(
	QtnPropertyBase &owner, const QString &error)
	: QtnPropertyDelegateWithValue(owner)
	, m_error(error)
{
}

bool QtnPropertyDelegateError::createSubItemValueImpl(
	QtnDrawContext & /*context*/, QtnSubItem &subItemValue)
{
	subItemValue.drawHandler = [this](QtnDrawContext &context,
								   const QtnSubItem &item) {
		QPen oldPen = context.painter->pen();
		context.painter->setPen(Qt::red);
		qtnDrawValueText(m_error, *context.painter, item.rect, context.style());
		context.painter->setPen(oldPen);
	};

	return true;
}

QtnPropertyDelegate *qtnCreateDelegateError(
	QtnPropertyBase &owner, QString error)
{
	return new QtnPropertyDelegateError(owner, error);
}

QtnInplaceInfo::QtnInplaceInfo()
	: activationEvent(0)
{
}

void QtnPropertyDelegate::drawButton(const QtnDrawContext &context,
	const QtnSubItem &item, const QIcon &icon, const QString &text)
{
	auto style = context.style();

	QStyleOptionToolButton option;
	context.initStyleOption(option);

	option.state = state(context.isActive, item);
	option.state &= ~QStyle::State_HasFocus;
	if (0 == (option.state & QStyle::State_Sunken))
	{
		option.state |= QStyle::State_Raised;
	}
	// dont initialize styleObject from widget for QWindowsVistaStyle
	// this disables buggous animations
	if (style->inherits("QWindowsVistaStyle"))
		option.styleObject = nullptr;
#ifdef Q_OS_MAC
	option.state &= ~QStyle::State_MouseOver;
#endif
	option.features = QStyleOptionToolButton::None;
	option.subControls = QStyle::SC_ToolButton;
	option.activeSubControls = QStyle::SC_ToolButton;
	option.toolButtonStyle = Qt::ToolButtonIconOnly;
	option.rect = item.rect;
	option.arrowType = Qt::NoArrow;
	if (!icon.availableSizes().empty())
	{
		option.icon = icon;
		option.iconSize = icon.actualSize(item.rect.size());
	} else
	{
		option.text = text.isEmpty() ? QStringLiteral("*") : text;
	}

	// draw button
	style->drawComplexControl(
		QStyle::CC_ToolButton, &option, context.painter, context.widget);
}
