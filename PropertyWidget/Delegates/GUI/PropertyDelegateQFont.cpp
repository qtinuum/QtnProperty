/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

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
#include "../../../Core/GUI/PropertyQFont.h"
#include "../../../Core/Core/PropertyQString.h"
#include "../../../Core/Core/PropertyUInt.h"
#include "../../../Core/Core/PropertyBool.h"
#include "../../../Core/Core/PropertyEnum.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"
#include "../Utils/PropertyEditorAux.h"

#include <QFontDialog>
#include <QFontDatabase>

void regQFontDelegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyQFontBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateQFont, QtnPropertyQFontBase>
                 , "LineEditBttn");
}

class QtnPropertyQFontLineEditBttnHandler: public QtnPropertyEditorHandler<QtnPropertyQFontBase, QtnLineEditBttn>
{
public:
    QtnPropertyQFontLineEditBttnHandler(QtnPropertyQFontBase& property, QtnLineEditBttn& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        editor.lineEdit->setReadOnly(true);

        if (!property.isEditableByUser())
        {
	  // editor.lineEdit->setReadOnly(true);
	  editor.toolButton->setEnabled(false);
        }

        updateEditor();

        QObject::connect(  editor.toolButton, &QToolButton::clicked
                         , this, &QtnPropertyQFontLineEditBttnHandler::onToolButtonClicked);
    }

private:
    void updateEditor() override
    {
        QFont font = property();
        editor().lineEdit->setText(QString("[%1, %2]").arg(font.family()).arg(font.pointSize()));
    }

    void onToolButtonClicked(bool checked)
    {
        Q_UNUSED(checked);

        QFontDialog dlg(property(), &editor());
        if (dlg.exec() == QDialog::Accepted)
        {
            property() = dlg.currentFont();
        }
    }
};

static QtnEnumInfo* styleStrategyEnum()
{
    static QtnEnumInfo* enumInfo = nullptr;
    if (!enumInfo)
    {
        QVector<QtnEnumValueInfo> items;
        items.append(QtnEnumValueInfo(QFont::PreferDefault, "PreferDefault"));
        items.append(QtnEnumValueInfo(QFont::NoAntialias, "NoAntialias"));
        items.append(QtnEnumValueInfo(QFont::PreferAntialias, "PreferAntialias"));
        enumInfo = new QtnEnumInfo("StyleStrategy", items);
    }

    return enumInfo;
}

QtnPropertyDelegateQFont::QtnPropertyDelegateQFont(QtnPropertyQFontBase& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyQFontBase>(owner)
{
    QtnPropertyQStringCallback* propertyFamily = new QtnPropertyQStringCallback(0);
    addSubProperty(propertyFamily);
    propertyFamily->setName(owner.tr("Family"));
    propertyFamily->setDescription(owner.tr("Font Family for %1.").arg(owner.name()));
    propertyFamily->setCallbackValueGet([&owner]()->QString {
        return owner.value().family();
    });
    propertyFamily->setCallbackValueSet([&owner](QString value) {
        QFont font = owner.value();
        font.setFamily(value);
        owner.setValue(font);
    });
    QtnPropertyDelegateInfo delegate;
    delegate.name = "List";
    QFontDatabase fDB;
    delegate.attributes["items"] = fDB.families();
    propertyFamily->setDelegate(delegate);

    QtnPropertyUIntCallback* propertyPointSize = new QtnPropertyUIntCallback(0);
    addSubProperty(propertyPointSize);
    propertyPointSize->setName(owner.tr("PointSize"));
    propertyPointSize->setDescription(owner.tr("Point size for %1.").arg(owner.name()));
    propertyPointSize->setCallbackValueGet([&owner]()->quint32 {
        int ps = owner.value().pointSize();
        return ps != -1 ? ps : 1;
    });
    propertyPointSize->setCallbackValueSet([&owner](quint32 value) {
        QFont font = owner.value();
        font.setPointSize((int)value);
        owner.setValue(font);
    });
    propertyPointSize->setMinValue(1);
    propertyPointSize->setMaxValue((quint32)std::numeric_limits<int>::max());

    QtnPropertyBoolCallback* propertyBold = new QtnPropertyBoolCallback(0);
    addSubProperty(propertyBold);
    propertyBold->setName(owner.tr("Bold"));
    propertyBold->setDescription(owner.tr("Bold flag for %1").arg(owner.name()));
    propertyBold->setCallbackValueGet([&owner]()->bool {
        return owner.value().bold();
    });
    propertyBold->setCallbackValueSet([&owner](bool value) {
        QFont font = owner.value();
        font.setBold(value);
        owner.setValue(font);
    });

    QtnPropertyBoolCallback* propertyItalic = new QtnPropertyBoolCallback(0);
    addSubProperty(propertyItalic);
    propertyItalic->setName(owner.tr("Italic"));
    propertyItalic->setDescription(owner.tr("Italic flag for %1").arg(owner.name()));
    propertyItalic->setCallbackValueGet([&owner]()->bool {
        return owner.value().italic();
    });
    propertyItalic->setCallbackValueSet([&owner](bool value) {
        QFont font = owner.value();
        font.setItalic(value);
        owner.setValue(font);
    });

    QtnPropertyBoolCallback* propertyUnderline = new QtnPropertyBoolCallback(0);
    addSubProperty(propertyUnderline);
    propertyUnderline->setName(owner.tr("Underline"));
    propertyUnderline->setDescription(owner.tr("Underline flag for %1").arg(owner.name()));
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
    propertyStrikeout->setName(owner.tr("Strikeout"));
    propertyStrikeout->setDescription(owner.tr("Strikeout flag for %1").arg(owner.name()));
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
    propertyKerning->setName(owner.tr("Kerning"));
    propertyKerning->setDescription(owner.tr("Kerning flag for %1").arg(owner.name()));
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
    propertyAntialiasing->setName(owner.tr("Antialiasing"));
    propertyAntialiasing->setDescription(owner.tr("Antialiasing flag for %1.").arg(owner.name()));
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

bool QtnPropertyDelegateQFont::propertyValueToStrImpl(QString& strValue) const
{
    QFont value = owner().value();
    strValue = QString("[%1, %2]").arg(value.family()).arg(value.pointSize());
    return true;
}
