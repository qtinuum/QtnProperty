include(../Internal/TargetConfig.pri)

QT += core gui widgets script

TARGET = QtnProperty
TEMPLATE = lib
VERSION = 2.0.0

qtnproperty_dynamic {
    DEFINES += QTN_DYNAMIC_LIBRARY
    macx {
        QMAKE_SONAME_PREFIX = @rpath
    }

    CONFIG += shared
} else {
    CONFIG += static
}

SOURCES += PropertyBase.cpp \
    Property.cpp \
    PropertySet.cpp \
    Enum.cpp \
    QObjectPropertySet.cpp \
    Core/PropertyBool.cpp \
    Core/PropertyInt.cpp \
    Core/PropertyUInt.cpp \
    Core/PropertyDouble.cpp \
    Core/PropertyFloat.cpp \
    Core/PropertyQString.cpp \
    Core/PropertyQRect.cpp \
    Core/PropertyEnum.cpp \
    Core/PropertyEnumFlags.cpp \
    Core/PropertyQSize.cpp \
    Core/PropertyQPoint.cpp \
    GUI/PropertyQColor.cpp \
    GUI/PropertyQFont.cpp \
    GUI/PropertyQBrush.cpp \
    GUI/PropertyQPen.cpp \
    GUI/PropertyButton.cpp \
    PropertyWidget.cpp \
    PropertyView.cpp \
    Utils/InplaceEditing.cpp \
    Delegates/PropertyDelegate.cpp \
    Delegates/PropertyDelegateAux.cpp \
    Delegates/PropertyDelegateFactory.cpp \
    Delegates/Core/PropertyDelegateBool.cpp \
    Delegates/Core/PropertyDelegateInt.cpp \
    Delegates/Core/PropertyDelegateUInt.cpp \
    Delegates/Core/PropertyDelegateQString.cpp \
    Delegates/Core/PropertyDelegateDouble.cpp \
    Delegates/Core/PropertyDelegateFloat.cpp \
    Delegates/Core/PropertyDelegateEnum.cpp \
    Delegates/Core/PropertyDelegateQRect.cpp \
    Delegates/Core/PropertyDelegateQRectF.cpp \
    Delegates/Core/PropertyDelegateEnumFlags.cpp \
    Delegates/Core/PropertyDelegateQSize.cpp \
    Delegates/Core/PropertyDelegateQSizeF.cpp \
    Delegates/Core/PropertyDelegateQPoint.cpp \
    Delegates/Core/PropertyDelegateQPointF.cpp \
    Delegates/GUI/PropertyDelegateQColor.cpp \
    Delegates/GUI/PropertyDelegateQBrush.cpp \
    Delegates/GUI/PropertyDelegateQPen.cpp \
    Delegates/GUI/PropertyDelegateQFont.cpp \
    Delegates/GUI/PropertyDelegateButton.cpp \
    Delegates/Utils/PropertyEditorHandler.cpp \
    Delegates/Utils/PropertyEditorAux.cpp \
    Delegates/Utils/PropertyDelegateMisc.cpp \
    Delegates/Utils/PropertyDelegatePropertySet.cpp \
    Delegates/Utils/PropertyDelegateSliderBox.cpp \
    Delegates/Utils/PropertyDelegateGeoCoord.cpp \
    Delegates/Utils/PropertyDelegateGeoPoint.cpp \
    Utils/AccessibilityProxy.cpp \
    Utils/DoubleSpinBox.cpp \
    Utils/MultilineTextDialog.cpp \
    PropertyInt64.cpp \
    PropertyUInt64.cpp \
    Core/PropertyQRectF.cpp \
    Core/PropertyQPointF.cpp \
    Core/PropertyQSizeF.cpp \
    PropertyWidgetEx.cpp \
    CustomPropertyEditorDialog.cpp \
    CustomPropertyOptionsDialog.cpp \
    VarProperty.cpp \
    PropertyQVariant.cpp \
    CustomPropertyWidget.cpp \
    QObjectPropertyWidget.cpp \
    MultiProperty.cpp \
    PropertyConnector.cpp \
    Utils/QtnConnections.cpp \
    Utils/QtnInt64SpinBox.cpp \
    Auxiliary/PropertyDelegateInfo.cpp \
    PropertyQKeySequence.cpp \
    PropertyDelegateMetaEnum.cpp \
    Install.cpp

HEADERS += CoreAPI.h\
    PropertyBase.h \
    Property.h\
    PropertySet.h\
    Enum.h\
    QObjectPropertySet.h \
    PropertyCore.h \
    PropertyGUI.h \
    Auxiliary/PropertyTemplates.h \
    Auxiliary/PropertyMacro.h \
    Auxiliary/PropertyAux.h \
    Auxiliary/PropertyDelegateInfo.h \
    Core/PropertyBool.h \
    Core/PropertyInt.h \
    Core/PropertyUInt.h \
    Core/PropertyDouble.h \
    Core/PropertyFloat.h \
    Core/PropertyQString.h \
    Core/PropertyQRect.h \
    Core/PropertyEnum.h \
    Core/PropertyEnumFlags.h \
    Core/PropertyQSize.h \
    Core/PropertyQPoint.h \
    GUI/PropertyQColor.h \
    GUI/PropertyQFont.h \
    GUI/PropertyQBrush.h \
    GUI/PropertyQPen.h \
    GUI/PropertyButton.h \
    PropertyWidget.h \
    PropertyWidgetAPI.h \
    PropertyView.h \
    Utils/InplaceEditing.h \
    Delegates/PropertyDelegate.h \
    Delegates/PropertyDelegateAux.h \
    Delegates/PropertyDelegateFactory.h \
    Delegates/Core/PropertyDelegateBool.h \
    Delegates/Core/PropertyDelegateInt.h \
    Delegates/Core/PropertyDelegateUInt.h \
    Delegates/Core/PropertyDelegateQString.h \
    Delegates/Core/PropertyDelegateDouble.h \
    Delegates/Core/PropertyDelegateFloat.h \
    Delegates/Core/PropertyDelegateEnum.h \
    Delegates/Core/PropertyDelegateQRect.h \
    Delegates/Core/PropertyDelegateQRectF.h \
    Delegates/Core/PropertyDelegateEnumFlags.h \
    Delegates/Core/PropertyDelegateQSize.h \
    Delegates/Core/PropertyDelegateQSizeF.h \
    Delegates/Core/PropertyDelegateQPoint.h \
    Delegates/Core/PropertyDelegateQPointF.h \
    Delegates/GUI/PropertyDelegateQColor.h \
    Delegates/GUI/PropertyDelegateQFont.h \
    Delegates/GUI/PropertyDelegateQBrush.h \
    Delegates/GUI/PropertyDelegateQPen.h \
    Delegates/GUI/PropertyDelegateButton.h \
    Delegates/Utils/PropertyEditorHandler.h \
    Delegates/Utils/PropertyEditorAux.h \
    Delegates/Utils/PropertyDelegateMisc.h \
    Delegates/Utils/PropertyDelegatePropertySet.h \
    Delegates/Utils/PropertyDelegateSliderBox.h \
    Delegates/Utils/PropertyDelegateGeoCoord.h \
    Delegates/Utils/PropertyDelegateGeoPoint.h \
    Utils/AccessibilityProxy.h \
    Utils/DoubleSpinBox.h \
    Utils/MultilineTextDialog.h \
    PropertyInt64.h \
    PropertyUInt64.h \
    Core/PropertyQRectF.h \
    Core/PropertyQPointF.h \
    Core/PropertyQSizeF.h \
    PropertyWidgetEx.h \
    CustomPropertyEditorDialog.h \
    CustomPropertyOptionsDialog.h \
    VarProperty.h \
    PropertyQVariant.h \
    CustomPropertyWidget.h \
    QObjectPropertyWidget.h \
    IQtnPropertyStateProvider.h \
    MultiProperty.h \
    StructPropertyBase.h \
    PropertyConnector.h \
    Utils/QtnConnections.h \
    Utils/QtnInt64SpinBox.h \
    PropertyDelegateAttrs.h \
    PropertyQKeySequence.h \
    PropertyDelegateMetaEnum.h \
    Install.h

TRANSLATIONS = \
    Translations/en.ts \
    Translations/ru.ts

for(tr, TRANSLATIONS):system($$[QT_INSTALL_BINS]/lrelease $$_PRO_FILE_PWD_/$${tr})

RESOURCES = QtnProperty.qrc

FORMS += \
    CustomPropertyEditorDialog.ui \
    CustomPropertyOptionsDialog.ui \
    Utils/MultilineTextDialog.ui

INCLUDEPATH += $$PWD/..
