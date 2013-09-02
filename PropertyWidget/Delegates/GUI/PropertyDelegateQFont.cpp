/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
 */

#include "PropertyDelegateQFont.h"
#include "../../../Core/GUI/PropertyQFont.h"
#include "../../../Core/Core/PropertyQString.h"
#include "../../../Core/Core/PropertyUInt.h"
#include "../../../Core/Core/PropertyBool.h"
#include "../../../Core/Core/PropertyEnum.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"
#include "../PropertyEditorAux.h"

#include <QFontDialog>
#include <QFontDatabase>

namespace Qtinuum
{

class PropertyQFontLineEditBttnHandler: public PropertyEditorHandler<PropertyQFontBase, LineEditBttn>
{
public:
    PropertyQFontLineEditBttnHandler(PropertyQFontBase &property, LineEditBttn &editor)
        : PropertyEditorHandlerType(property, editor)
    {
        editor.lineEdit->setReadOnly(true);

        if (!property.isEditableByUser())
        {
//            editor.lineEdit->setReadOnly(true);
            editor.toolButton->setEnabled(false);
        }

        updateEditor();

        QObject::connect(  editor.toolButton, &QToolButton::clicked
                         , this, &PropertyQFontLineEditBttnHandler::onToolButtonClicked);
    }

private:
    void updateEditor() override
    {
        QFont font = property();
        editor().lineEdit->setText(QString("[%1, %2]").arg(font.family()).arg(font.pointSize()));
    }

    void onToolButtonClicked(bool checked)
    {
        QFontDialog dlg(property(), &editor());
        if (dlg.exec() == QDialog::Accepted)
        {
            property() = dlg.currentFont();
        }
    }
};

static bool regQFontDelegate = PropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&PropertyQFontBase::staticMetaObject
                                , &createDelegate<PropertyDelegateQFont, PropertyQFontBase>
                                , "LineEditBttn");

static EnumInfo *styleStategyEnum()
{
    static EnumInfo *enumInfo = 0;
    if (!enumInfo)
    {
        QVector<EnumValueInfo> items;
        items.append(EnumValueInfo(QFont::PreferDefault, "PreferDefault"));
        items.append(EnumValueInfo(QFont::NoAntialias, "NoAntialias"));
        items.append(EnumValueInfo(QFont::PreferAntialias, "PreferAntialias"));
        enumInfo = new EnumInfo(items);
    }

    return enumInfo;
}

PropertyDelegateQFont::PropertyDelegateQFont(PropertyQFontBase &owner)
    : PropertyDelegateTypedEx<PropertyQFontBase>(owner)
{
    PropertyQStringCallback *propertyFamily = new PropertyQStringCallback(0);
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
    PropertyDelegateInfo delegate;
    delegate.name = "List";
    QFontDatabase fDB;
    delegate.attributes["items"] = fDB.families();
    propertyFamily->setDelegate(delegate);

    PropertyUIntCallback *propertyPointSize = new PropertyUIntCallback(0);
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

    PropertyBoolCallback *propertyBold = new PropertyBoolCallback(0);
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

    PropertyBoolCallback *propertyItalic = new PropertyBoolCallback(0);
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

    PropertyBoolCallback *propertyUnderline = new PropertyBoolCallback(0);
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

    PropertyBoolCallback *propertyStrikeout = new PropertyBoolCallback(0);
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

    PropertyBoolCallback *propertyKerning = new PropertyBoolCallback(0);
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

    PropertyEnumCallback *propertyAntialiasing = new PropertyEnumCallback(0);
    addSubProperty(propertyAntialiasing);
    propertyAntialiasing->setName(owner.tr("Antialiasing"));
    propertyAntialiasing->setDescription(owner.tr("Antialiasing flag for %1.").arg(owner.name()));
    propertyAntialiasing->setEnumInfo(styleStategyEnum());
    propertyAntialiasing->setCallbackValueGet([&owner]()->EnumValueType {
        return owner.value().styleStrategy();
    });
    propertyAntialiasing->setCallbackValueSet([&owner](EnumValueType value) {
        QFont font = owner.value();
        font.setStyleStrategy((QFont::StyleStrategy)value);
        owner.setValue(font);
    });
}

void PropertyDelegateQFont::drawValueImpl(QStylePainter &painter, const QRect &rect, const QStyle::State &state, bool *needTooltip) const
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
        PropertyDelegateTypedEx<PropertyQFontBase>::drawValueImpl(painter, textRect, state, needTooltip);
    }
}

QWidget *PropertyDelegateQFont::createValueEditorImpl(QWidget *parent, const QRect &rect, InplaceInfo *inplaceInfo)
{
    LineEditBttn *editor = new LineEditBttn(parent);
    editor->setGeometry(rect);

    new PropertyQFontLineEditBttnHandler(owner(), *editor);

    if (inplaceInfo)
    {
        editor->lineEdit->selectAll();
    }

    return editor;
}

bool PropertyDelegateQFont::propertyValueToStr(QString &strValue) const
{
    QFont value = owner().value();
    strValue = QString("[%1, %2]").arg(value.family()).arg(value.pointSize());
    return true;
}


} // end namespace Qtinuum

