/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyDelegateQFont.h"
#include "GUI/PropertyQFont.h"
#include "Core/PropertyQString.h"
#include "Core/PropertyUInt.h"
#include "Core/PropertyBool.h"
#include "Core/PropertyEnum.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Delegates/PropertyEditorHandler.h"
#include "Delegates/PropertyEditorAux.h"
#include "PropertySet.h"

#include <QFontDialog>
#include <QFontDatabase>

#include <memory>

class QtnPropertyQFontLineEditBttnHandler
		: public QtnPropertyEditorBttnHandler<QtnPropertyQFontBase, QtnLineEditBttn>
{
public:
	QtnPropertyQFontLineEditBttnHandler(QtnPropertyQFontBase& property, QtnLineEditBttn& editor);

protected:
	virtual void onToolButtonClick() override;
	virtual void updateEditor() override;

private:
	void onToolButtonClicked(bool);
};

static bool regQFontDelegate = QtnPropertyDelegateFactory::staticInstance()
								.registerDelegateDefault(&QtnPropertyQFontBase::staticMetaObject
								, &qtnCreateDelegate<QtnPropertyDelegateQFont, QtnPropertyQFontBase>
								, "LineEditBttn");

static QtnEnumInfo* styleStrategyEnum()
{
	static QtnEnumInfo* enumInfo = nullptr;
	if (!enumInfo)
	{
		QVector<QtnEnumValueInfo> items;
		items.append(QtnEnumValueInfo(QFont::PreferDefault, QtnPropertyQFont::getPreferDefaultStr()));
		items.append(QtnEnumValueInfo(QFont::NoAntialias, QtnPropertyQFont::getNoAntialiasStr()));
		items.append(QtnEnumValueInfo(QFont::PreferAntialias, QtnPropertyQFont::getPreferAntialiasStr()));
		enumInfo = new QtnEnumInfo("FontStyleStrategy", items);
	}

	return enumInfo;
}

enum
{
	SizeUnitPixel,
	SizeUnitPoint
};


static QtnEnumInfo* sizeUnitEnum()
{
	static QtnEnumInfo* enumInfo = nullptr;
	if (!enumInfo)
	{
		QVector<QtnEnumValueInfo> items;
		items.append(QtnEnumValueInfo(SizeUnitPixel, QtnPropertyQFont::getPixelStr()));
		items.append(QtnEnumValueInfo(SizeUnitPoint, QtnPropertyQFont::getPointStr()));
		enumInfo = new QtnEnumInfo("FontSizeUnit", items);
	}

	return enumInfo;
}

static void applyFontStyle(QFont &font)
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

QtnPropertyDelegateQFont::QtnPropertyDelegateQFont(QtnPropertyQFontBase& owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyQFontBase>(owner)
{
	auto propertyStyle = new QtnPropertyQStringCallback(0);


	QtnPropertyQStringCallback* propertyFamily = new QtnPropertyQStringCallback(0);
	addSubProperty(propertyFamily);

	propertyFamily->setName(QtnPropertyQFont::getFamilyLabel());
	propertyFamily->setDescription(QtnPropertyQFont::getFamilyDescription(owner.name()));
	propertyFamily->setCallbackValueGet([&owner]()->QString {
		return owner.value().family();
	});
	propertyFamily->setCallbackValueSet([&owner, propertyStyle, propertyFamily](QString value) {
		QFont font = owner.value();
		font.setFamily(value);
		applyFontStyle(font);
		owner.setValue(font);

#ifdef Q_OS_MAC
		QtnPropertyDelegateInfo delegate;
		delegate.name = "List";
		QFontDatabase fDB;
		delegate.attributes["items"] = QStringList("") + fDB.styles(value);
		delegate.attributes["editable"] = true;
		propertyStyle->setDelegate(delegate);

		std::shared_ptr<QMetaObject::Connection> c(new QMetaObject::Connection);
		*c.get() = QObject::connect(propertyFamily, &QtnPropertyBase::propertyDidChange, [&owner, c]() mutable
		{
			QObject::disconnect(*c.get());
			c = nullptr;

			emit owner.propertyDidChange(QtnPropertyChangeReasonChildren);
		});
#endif
	});

	QtnPropertyDelegateInfo delegate;
	delegate.name = "List";
	QFontDatabase fDB;
	delegate.attributes["items"] = fDB.families();
	propertyFamily->setDelegate(delegate);

	propertyStyle->setName(QtnPropertyQFont::getStyleLabel());
	propertyStyle->setDescription(QtnPropertyQFont::getStyleDescription(owner.name()));
	propertyStyle->setCallbackValueGet([&owner]()->QString {
		return owner.value().styleName();
	});
	propertyStyle->setCallbackValueSet([&owner](QString value) {
		QFont font = owner.value();
		font.setStyleName(value);
		applyFontStyle(font);
		owner.setValue(font);
	});

#ifdef Q_OS_MAC
	delegate.name = "List";
	delegate.attributes["items"] = QStringList("") + fDB.styles(owner.value().family());
	delegate.attributes["editable"] = true;
#else
	delegate.name = "LineEdit";
	delegate.attributes["multiline_edit"] = false;
#endif
	propertyStyle->setDelegate(delegate);


	addSubProperty(propertyStyle);

	QtnPropertyUIntCallback* propertySize = new QtnPropertyUIntCallback(0);
	addSubProperty(propertySize);
	propertySize->setName(QtnPropertyQFont::getSizeLabel());
	propertySize->setDescription(QtnPropertyQFont::getSizeDescription(owner.name()));
	propertySize->setCallbackValueGet([&owner]() -> quint32
	{
		int ps = owner.value().pointSize();
		if (ps < 0)
			ps = owner.value().pixelSize();
		if (ps < 1)
			ps = 1;
		return ps;
	});
	propertySize->setCallbackValueSet([&owner](quint32 value)
	{
		if (value == 0)
			value = 1;

		QFont font = owner.value();
		if (font.pointSize() > 0)
		{
			if (value > 128)
				value = 128;

			font.setPointSize((int) value);
		} else
		{
			if (value > 256)
				value = 256;

			font.setPixelSize((int) value);
		}
		owner.setValue(font);
	});

	QtnPropertyEnumCallback* propertySizeUnit = new QtnPropertyEnumCallback(0);
	addSubProperty(propertySizeUnit);
	propertySizeUnit->setName(QtnPropertyQFont::getSizeUnitLabel());
	propertySizeUnit->setDescription(QtnPropertyQFont::getSizeUnitDescription(owner.name()));
	propertySizeUnit->setEnumInfo(sizeUnitEnum());
	propertySizeUnit->setCallbackValueGet([&owner]() -> QtnEnumValueType
	{
		if (owner.value().pointSize() < 0)
			return SizeUnitPixel;

		return SizeUnitPoint;
	});
	propertySizeUnit->setCallbackValueSet([&owner](QtnEnumValueType value)
	{
		QFont font = owner.value();

		int size = std::max(font.pointSize(), font.pixelSize());

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

		owner.setValue(font);
	});

	QtnPropertyBoolCallback* propertyBold = new QtnPropertyBoolCallback(0);
	addSubProperty(propertyBold);
	propertyBold->setName(QtnPropertyQFont::getBoldLabel());
	propertyBold->setDescription(QtnPropertyQFont::getBoldDescription(owner.name()));
	propertyBold->setCallbackValueGet([&owner]()->bool {
		return owner.value().bold();
	});
	propertyBold->setCallbackValueSet([&owner](bool value) {
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
			owner.setValue(font);
		}
	});

	QtnPropertyBoolCallback* propertyItalic = new QtnPropertyBoolCallback(0);
	addSubProperty(propertyItalic);
	propertyItalic->setName(QtnPropertyQFont::getItalicLabel());
	propertyItalic->setDescription(QtnPropertyQFont::getItalicDescription(owner.name()));
	propertyItalic->setCallbackValueGet([&owner]()->bool {
		return owner.value().italic();
	});
	propertyItalic->setCallbackValueSet([&owner](bool value) {
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
			owner.setValue(font);
		}
	});

	QtnPropertyBoolCallback* propertyUnderline = new QtnPropertyBoolCallback(0);
	addSubProperty(propertyUnderline);
	propertyUnderline->setName(QtnPropertyQFont::getUnderlineLabel());
	propertyUnderline->setDescription(QtnPropertyQFont::getUnderlineDescription(owner.name()));
	propertyUnderline->setCallbackValueGet([&owner]()->bool {
		return owner.value().underline();
	});
	propertyUnderline->setCallbackValueSet([&owner](bool value) {
		QFont font = owner.value();
		font.setUnderline(value);
		owner.setValue(font);
	});

	QtnPropertyBoolCallback* propertyStrikeout = new QtnPropertyBoolCallback(0);
	addSubProperty(propertyStrikeout);
	propertyStrikeout->setName(QtnPropertyQFont::getStrikeoutLabel());
	propertyStrikeout->setDescription(QtnPropertyQFont::getStrikeoutDescription(owner.name()));
	propertyStrikeout->setCallbackValueGet([&owner]()->bool {
		return owner.value().strikeOut();
	});
	propertyStrikeout->setCallbackValueSet([&owner](bool value) {
		QFont font = owner.value();
		font.setStrikeOut(value);
		owner.setValue(font);
	});

	QtnPropertyBoolCallback* propertyKerning = new QtnPropertyBoolCallback(0);
	addSubProperty(propertyKerning);
	propertyKerning->setName(QtnPropertyQFont::getKerningLabel());
	propertyKerning->setDescription(QtnPropertyQFont::getKerningDescription(owner.name()));
	propertyKerning->setCallbackValueGet([&owner]()->bool {
		return owner.value().kerning();
	});
	propertyKerning->setCallbackValueSet([&owner](bool value) {
		QFont font = owner.value();
		font.setKerning(value);
		owner.setValue(font);
	});

	QtnPropertyEnumCallback* propertyAntialiasing = new QtnPropertyEnumCallback(0);
	addSubProperty(propertyAntialiasing);
	propertyAntialiasing->setName(QtnPropertyQFont::getAntialiasingLabel());
	propertyAntialiasing->setDescription(QtnPropertyQFont::getAntialiasingDescription(owner.name()));
	propertyAntialiasing->setEnumInfo(styleStrategyEnum());
	propertyAntialiasing->setCallbackValueGet([&owner]()->QtnEnumValueType {
		return owner.value().styleStrategy();
	});
	propertyAntialiasing->setCallbackValueSet([&owner](QtnEnumValueType value) {
		QFont font = owner.value();
		font.setStyleStrategy((QFont::StyleStrategy)value);
		owner.setValue(font);
	});
}

QString QtnPropertyDelegateQFont::fontToStrWithFormat(const QFont &font,
													  const QString &format)
{
	return QString(format).arg(fontToStr(font));
}

QString QtnPropertyDelegateQFont::fontToStr(const QFont &font)
{
	int size = font.pointSize();
	bool pixels = (size < 0);
	if (pixels)
		size = font.pixelSize();

	return QString("%1, %2 %3").arg(font.family(), QString::number(size), QString(pixels ? "px" : "pt"));
}

void QtnPropertyDelegateQFont::drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip) const
{
	QFont value = owner().value();

	QRect textRect = rect;

	if (textRect.isValid())
	{
		QRect br;

		QFont oldFont = painter.font();
		QFont newFont(value);
		newFont.setPointSize(oldFont.pointSize());
		painter.setFont(newFont);
		painter.drawText(textRect, Qt::AlignLeading | Qt::AlignVCenter, "A", &br);
		painter.setFont(oldFont);

		textRect.setLeft(br.right() + 3);
	}

	if (textRect.isValid())
	{
		QtnPropertyDelegateTypedEx<QtnPropertyQFontBase>::drawValueImpl(painter, textRect, state, needTooltip);
	}
}

QWidget* QtnPropertyDelegateQFont::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
	QtnLineEditBttn* editor = new QtnLineEditBttn(parent);
	editor->setGeometry(rect);

	new QtnPropertyQFontLineEditBttnHandler(owner(), *editor);

	if (inplaceInfo)
	{
		editor->lineEdit->selectAll();
	}

	return editor;
}

bool QtnPropertyDelegateQFont::propertyValueToStr(QString& strValue) const
{
	strValue = fontToStrWithFormat(owner().value());
	return true;
}

QtnPropertyQFontLineEditBttnHandler::QtnPropertyQFontLineEditBttnHandler(QtnPropertyQFontBase &property, QtnLineEditBttn &editor)
	: QtnPropertyEditorHandlerType(property, editor)
{
	editor.lineEdit->setReadOnly(true);

	if (!property.isEditableByUser())
	{
		editor.toolButton->setEnabled(false);
	}

	updateEditor();
	editor.lineEdit->installEventFilter(this);
	QObject::connect(editor.toolButton, &QToolButton::clicked,
					 this, &QtnPropertyQFontLineEditBttnHandler::onToolButtonClicked);
}

void QtnPropertyQFontLineEditBttnHandler::onToolButtonClick()
{
	onToolButtonClicked(false);
}

void QtnPropertyQFontLineEditBttnHandler::updateEditor()
{
	editor().setTextForProperty(&property(),
			QtnPropertyDelegateQFont::fontToStrWithFormat(property()));
	editor().lineEdit->selectAll();
}

void QtnPropertyQFontLineEditBttnHandler::onToolButtonClicked(bool)
{
	auto property = &this->property();
	volatile bool destroyed = false;
	auto connection = QObject::connect(property, &QObject::destroyed, [&destroyed]() mutable
	{
		destroyed = true;
	});
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

		if (property->edit(font))
			emit property->propertyDidChange(QtnPropertyChangeReasonChildren);
	}

	if (!destroyed)
		QObject::disconnect(connection);
	Q_UNUSED(dialogContainer);
}
