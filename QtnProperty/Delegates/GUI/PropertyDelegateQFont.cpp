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

#include "PropertyDelegateQFont.h"
#include "QtnProperty/Core/PropertyQString.h"
#include "QtnProperty/Core/PropertyInt.h"
#include "QtnProperty/Core/PropertyBool.h"
#include "QtnProperty/Core/PropertyEnum.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"
#include "QtnProperty/PropertySet.h"
#include "QtnProperty/PropertyDelegateAttrs.h"

#include <QFontDialog>
#include <QFontDatabase>

QByteArray qtnSelectFontDelegate()
{
	return QByteArrayLiteral("SelectFont");
}

class QtnPropertyQFontLineEditBttnHandler
	: public QtnPropertyEditorBttnHandler<QtnPropertyQFontBase, QtnLineEditBttn>
{
public:
	QtnPropertyQFontLineEditBttnHandler(
		QtnPropertyDelegate *delegate, QtnLineEditBttn &editor);

protected:
	virtual void onToolButtonClick() override;
	virtual void updateEditor() override;

private:
	void onToolButtonClicked(bool);
};

static const QtnEnumInfo *styleStrategyEnum()
{
	static QScopedPointer<QtnEnumInfo> enumInfo;

	if (nullptr == enumInfo)
	{
		QVector<QtnEnumValueInfo> items;
		items.append(QtnEnumValueInfo(QFont::PreferDefault, "PreferDefault",
			QtnPropertyQFont::getPreferDefaultStr()));
		items.append(QtnEnumValueInfo(QFont::NoAntialias, "NoAntialias",
			QtnPropertyQFont::getNoAntialiasStr()));
		items.append(QtnEnumValueInfo(QFont::PreferAntialias, "PreferAntialias",
			QtnPropertyQFont::getPreferAntialiasStr()));
		enumInfo.reset(new QtnEnumInfo("FontStyleStrategy", items));
	}

	return enumInfo.data();
}

enum
{
	SizeUnitPixel,
	SizeUnitPoint
};

static const QtnEnumInfo *sizeUnitEnum()
{
	static QScopedPointer<QtnEnumInfo> enumInfo;

	if (nullptr == enumInfo)
	{
		QVector<QtnEnumValueInfo> items;
		items.append(QtnEnumValueInfo(
			SizeUnitPixel, "Pixel", QtnPropertyQFont::getPixelStr()));
		items.append(QtnEnumValueInfo(
			SizeUnitPoint, "Point", QtnPropertyQFont::getPointStr()));
		enumInfo.reset(new QtnEnumInfo("FontSizeUnit", items));
	}

	return enumInfo.data();
}

static void applyFontStyle(QFont &font)
{
#ifdef Q_OS_MAC
	auto style = font.styleName();

	if (!style.isEmpty())
	{
		auto family = font.family();
		QFontDatabase db;

		for (const auto &s : db.styles(family))
		{
			if (s == style)
			{
				font.setBold(db.bold(family, style));
				font.setItalic(db.italic(family, style));
				return;
			}
		}
	}

#else
	Q_UNUSED(font);
#endif
}

QtnPropertyDelegateQFont::QtnPropertyDelegateQFont(QtnPropertyQFontBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQFontBase>(owner)
{
	auto propertyStyle = new QtnPropertyQStringCallback;
	auto propertyFamily = new QtnPropertyQStringCallback;
	addSubProperty(propertyFamily);

	propertyFamily->setName(QStringLiteral("family"));
	propertyFamily->setDisplayName(QtnPropertyQFont::getFamilyLabel());
	propertyFamily->setDescription(
		QtnPropertyQFont::getFamilyDescription(owner.name()));
	propertyFamily->setCallbackValueGet(
		[&owner]() -> QString { return owner.value().family(); });
	propertyFamily->setCallbackValueSet([&owner, propertyStyle](QString value, QtnPropertyChangeReason reason) {
		QFont font = owner.value();
		font.setFamily(value);
		applyFontStyle(font);
		owner.setValue(font, reason);

#ifdef Q_OS_MAC
		QtnPropertyDelegateInfo delegate;
		delegate.name = qtnComboBoxDelegate();
		QFontDatabase fDB;
		delegate.attributes[qtnItemsAttr()] =
			QStringList(QString()) + fDB.styles(value);
		delegate.attributes[qtnEditableAttr()] = true;
		propertyStyle->setDelegateInfo(delegate);

		owner.postUpdateEvent(QtnPropertyChangeReasonChildren);
#else
		Q_UNUSED(propertyStyle);
#endif
	});

	QtnPropertyDelegateInfo delegate;
	delegate.name = qtnComboBoxDelegate();
	QFontDatabase fDB;
	delegate.attributes[qtnItemsAttr()] = fDB.families();
	propertyFamily->setDelegateInfo(delegate);

	propertyStyle->setName(QStringLiteral("style"));
	propertyStyle->setDisplayName(QtnPropertyQFont::getStyleLabel());
	propertyStyle->setDescription(
		QtnPropertyQFont::getStyleDescription(owner.name()));
	propertyStyle->setCallbackValueGet(
		[&owner]() -> QString { return owner.value().styleName(); });
	propertyStyle->setCallbackValueSet([&owner](QString value, QtnPropertyChangeReason reason) {
		QFont font = owner.value();
		font.setStyleName(value);
		applyFontStyle(font);
		owner.setValue(font, reason);
	});

#ifdef Q_OS_MAC
	delegate.name = qtnComboBoxDelegate();
	delegate.attributes[qtnItemsAttr()] =
		QStringList(QString()) + fDB.styles(owner.value().family());
	delegate.attributes[qtnEditableAttr()] = true;
#else
	delegate.name = qtnLineEditDelegate();
	delegate.attributes[qtnMultiLineEditAttr()] = false;
#endif
	propertyStyle->setDelegateInfo(delegate);

	addSubProperty(propertyStyle);

	auto propertySize = new QtnPropertyIntCallback;
	addSubProperty(propertySize);
	propertySize->setName(QStringLiteral("size"));
	propertySize->setDisplayName(QtnPropertyQFont::getSizeLabel());
	propertySize->setDescription(
		QtnPropertyQFont::getSizeDescription(owner.name()));
	propertySize->setCallbackValueGet([&owner]() -> qint32 {
		int ps = owner.value().pointSize();

		if (ps < 0)
			ps = owner.value().pixelSize();

		if (ps <= 0)
			ps = 1;

		if (ps > 256)
			ps = 256;

		return ps;
	});
	propertySize->setCallbackValueSet([&owner](qint32 value, QtnPropertyChangeReason reason) {
		if (value <= 0)
			value = 1;
		else if (value > 256)
			value = 256;

		QFont font = owner.value();

		if (font.pointSize() > 0)
		{
			font.setPointSize(value);
		} else
		{
			font.setPixelSize(value);
		}

		owner.setValue(font, reason);
	});

	propertySize->setMinValue(1);
	propertySize->setMaxValue(256);

	QtnPropertyEnumCallback *propertySizeUnit = new QtnPropertyEnumCallback;
	addSubProperty(propertySizeUnit);
	propertySizeUnit->setName(QStringLiteral("sizeUnit"));
	propertySizeUnit->setDisplayName(QtnPropertyQFont::getSizeUnitLabel());
	propertySizeUnit->setDescription(
		QtnPropertyQFont::getSizeUnitDescription(owner.name()));
	propertySizeUnit->setEnumInfo(sizeUnitEnum());
	propertySizeUnit->setCallbackValueGet([&owner]() -> QtnEnumValueType {
		if (owner.value().pointSize() < 0)
			return SizeUnitPixel;

		return SizeUnitPoint;
	});

	propertySizeUnit->setCallbackValueSet([&owner](QtnEnumValueType value, QtnPropertyChangeReason reason) {
		QFont font = owner.value();

		int size = std::max(font.pointSize(), font.pixelSize());

		if (size <= 0)
			size = 1;

		if (size > 256)
			size = 256;

		switch (value)
		{
			case SizeUnitPixel:
				font.setPixelSize(size);
				break;

			case SizeUnitPoint:
				font.setPointSize(size);
				break;

			default:
				break;
		}

		owner.setValue(font, reason);
	});

	QtnPropertyBoolCallback *propertyBold = new QtnPropertyBoolCallback;
	addSubProperty(propertyBold);
	propertyBold->setName(QStringLiteral("bold"));
	propertyBold->setDisplayName(QtnPropertyQFont::getBoldLabel());
	propertyBold->setDescription(
		QtnPropertyQFont::getBoldDescription(owner.name()));
	propertyBold->setCallbackValueGet(
		[&owner]() -> bool { return owner.value().bold(); });
	propertyBold->setCallbackValueSet([&owner](bool value, QtnPropertyChangeReason reason) {
		QFont font = owner.value();

		if (font.bold() != value)
		{
#ifdef Q_OS_MAC
			auto style = font.styleName();

			if (!style.isEmpty())
			{
				auto family = font.family();
				QFontDatabase db;

				for (auto &s : db.styles(family))
				{
					if (s == style)
					{
						if (value != db.bold(family, style))
							return;

						break;
					}
				}
			}

#endif
			font.setBold(value);
			owner.setValue(font, reason);
		}
	});

	QtnPropertyBoolCallback *propertyItalic = new QtnPropertyBoolCallback;
	addSubProperty(propertyItalic);
	propertyItalic->setName(QStringLiteral("italic"));
	propertyItalic->setDisplayName(QtnPropertyQFont::getItalicLabel());
	propertyItalic->setDescription(
		QtnPropertyQFont::getItalicDescription(owner.name()));
	propertyItalic->setCallbackValueGet(
		[&owner]() -> bool { return owner.value().italic(); });
	propertyItalic->setCallbackValueSet([&owner](bool value, QtnPropertyChangeReason reason) {
		QFont font = owner.value();

		if (font.italic() != value)
		{
#ifdef Q_OS_MAC
			auto style = font.styleName();

			if (!style.isEmpty())
			{
				auto family = font.family();
				QFontDatabase db;

				for (auto &s : db.styles(family))
				{
					if (s == style)
					{
						if (value != db.italic(family, style))
							return;

						break;
					}
				}
			}

#endif
			font.setItalic(value);
			owner.setValue(font, reason);
		}
	});

	QtnPropertyBoolCallback *propertyUnderline = new QtnPropertyBoolCallback;
	addSubProperty(propertyUnderline);
	propertyUnderline->setName(QStringLiteral("underline"));
	propertyUnderline->setDisplayName(QtnPropertyQFont::getUnderlineLabel());
	propertyUnderline->setDescription(
		QtnPropertyQFont::getUnderlineDescription(owner.name()));
	propertyUnderline->setCallbackValueGet(
		[&owner]() -> bool { return owner.value().underline(); });
	propertyUnderline->setCallbackValueSet([&owner](bool value, QtnPropertyChangeReason reason) {
		QFont font = owner.value();
		font.setUnderline(value);
		owner.setValue(font, reason);
	});

	QtnPropertyBoolCallback *propertyStrikeout = new QtnPropertyBoolCallback;
	addSubProperty(propertyStrikeout);
	propertyStrikeout->setName(QStringLiteral("strikeout"));
	propertyStrikeout->setDisplayName(QtnPropertyQFont::getStrikeoutLabel());
	propertyStrikeout->setDescription(
		QtnPropertyQFont::getStrikeoutDescription(owner.name()));
	propertyStrikeout->setCallbackValueGet(
		[&owner]() -> bool { return owner.value().strikeOut(); });
	propertyStrikeout->setCallbackValueSet([&owner](bool value, QtnPropertyChangeReason reason) {
		QFont font = owner.value();
		font.setStrikeOut(value);
		owner.setValue(font, reason);
	});

	QtnPropertyBoolCallback *propertyKerning = new QtnPropertyBoolCallback;
	addSubProperty(propertyKerning);
	propertyKerning->setName(QStringLiteral("kerning"));
	propertyKerning->setDisplayName(QtnPropertyQFont::getKerningLabel());
	propertyKerning->setDescription(
		QtnPropertyQFont::getKerningDescription(owner.name()));
	propertyKerning->setCallbackValueGet(
		[&owner]() -> bool { return owner.value().kerning(); });
	propertyKerning->setCallbackValueSet([&owner](bool value, QtnPropertyChangeReason reason) {
		QFont font = owner.value();
		font.setKerning(value);
		owner.setValue(font, reason);
	});

	auto propertyAntialiasing = new QtnPropertyEnumCallback(nullptr);
	addSubProperty(propertyAntialiasing);
	propertyAntialiasing->setName(QStringLiteral("antialiasing"));
	propertyAntialiasing->setDisplayName(
		QtnPropertyQFont::getAntialiasingLabel());
	propertyAntialiasing->setDescription(
		QtnPropertyQFont::getAntialiasingDescription(owner.name()));
	propertyAntialiasing->setEnumInfo(styleStrategyEnum());
	propertyAntialiasing->setCallbackValueGet([&owner]() -> QtnEnumValueType {
		return owner.value().styleStrategy();
	});
	propertyAntialiasing->setCallbackValueSet([&owner](QtnEnumValueType value, QtnPropertyChangeReason reason) {
		QFont font = owner.value();
		font.setStyleStrategy(static_cast<QFont::StyleStrategy>(value));
		owner.setValue(font, reason);
	});
}

void QtnPropertyDelegateQFont::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyQFontBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateQFont, QtnPropertyQFontBase>,
		qtnSelectFontDelegate());
}

QString QtnPropertyDelegateQFont::fontToStrWithFormat(
	const QFont &font, const QString &format)
{
	return QString(format).arg(fontToStr(font));
}

QString QtnPropertyDelegateQFont::fontToStr(const QFont &font)
{
	int size = font.pointSize();
	bool pixels = (size < 0);

	if (pixels)
		size = font.pixelSize();

	return QString("%1, %2 %3")
		.arg(font.family(), QString::number(size),
			QString(pixels ? "px" : "pt"));
}

void QtnPropertyDelegateQFont::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	if (stateProperty()->isMultiValue())
	{
		QtnPropertyDelegateTypedEx::drawValueImpl(painter, rect);
		return;
	}

	QFont value = owner().value();

	QRect textRect = rect;

	if (textRect.isValid())
	{
		QRect br;

		QFont oldFont = painter.font();
		QFont newFont(value);
		newFont.setPointSize(oldFont.pointSize());
		painter.setFont(newFont);
		painter.drawText(
			textRect, Qt::AlignLeading | Qt::AlignVCenter, "A", &br);
		painter.setFont(oldFont);

		textRect.setLeft(br.right() + 3);
	}

	if (textRect.isValid())
	{
		QtnPropertyDelegateTypedEx<QtnPropertyQFontBase>::drawValueImpl(
			painter, textRect);
	}
}

QWidget *QtnPropertyDelegateQFont::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QtnLineEditBttn *editor = new QtnLineEditBttn(parent);
	editor->setGeometry(rect);

	new QtnPropertyQFontLineEditBttnHandler(this, *editor);

	if (inplaceInfo)
	{
		editor->lineEdit->selectAll();
	}

	return editor;
}

bool QtnPropertyDelegateQFont::propertyValueToStrImpl(QString &strValue) const
{
	strValue = fontToStrWithFormat(owner().value());
	return true;
}

QtnPropertyQFontLineEditBttnHandler::QtnPropertyQFontLineEditBttnHandler(
	QtnPropertyDelegate *delegate, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(delegate, editor)
{
	editor.lineEdit->setReadOnly(true);

	if (!stateProperty()->isEditableByUser())
	{
		editor.toolButton->setEnabled(false);
	}

	updateEditor();
	editor.lineEdit->installEventFilter(this);
	QObject::connect(editor.toolButton, &QToolButton::clicked, this,
		&QtnPropertyQFontLineEditBttnHandler::onToolButtonClicked);
}

void QtnPropertyQFontLineEditBttnHandler::onToolButtonClick()
{
	onToolButtonClicked(false);
}

void QtnPropertyQFontLineEditBttnHandler::updateEditor()
{
	editor().setTextForProperty(stateProperty(),
		QtnPropertyDelegateQFont::fontToStrWithFormat(property()));
	editor().lineEdit->selectAll();
}

void QtnPropertyQFontLineEditBttnHandler::onToolButtonClicked(bool)
{
	auto property = &this->property();
	volatile bool destroyed = false;
	auto connection = QObject::connect(property, &QObject::destroyed,
		[&destroyed]() mutable { destroyed = true; });
	auto dialog = new QFontDialog(property->value(), editorBase());
	auto dialogContainer = connectDialog(dialog);

	if (dialog->exec() == QDialog::Accepted && !destroyed)
	{
		auto font = property->value();
		auto styleStrategy = font.styleStrategy();
		int pixelSize = font.pixelSize();
		font = dialog->currentFont();

		if (pixelSize > 0)
			font.setPixelSize(font.pointSize());

		font.setStyleStrategy(styleStrategy);

		property->setValue(
			font, delegate()->editReason() | QtnPropertyChangeReasonChildren);
	}

	if (!destroyed)
		QObject::disconnect(connection);

	Q_UNUSED(dialogContainer);
}
