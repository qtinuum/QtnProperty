include($$_PRO_FILE_PWD_/../Common.pri)

QT += core gui widgets script

TARGET = QtnPropertyAC
TEMPLATE = lib
VERSION = 1.3.0

DEFINES += QTN_DYNAMIC_LIBRARY

macx {
    QMAKE_SONAME_PREFIX = @executable_path/../lib
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
    Core/PropertyQString.cpp \
    Core/PropertyQRect.cpp \
    Core/PropertyEnum.cpp \
    Core/PropertyEnumFlags.cpp \
    Core/PropertyQSize.cpp \
    Core/PropertyQPoint.cpp \
    GUI/PropertyQColor.cpp \
    GUI/PropertyQFont.cpp \
    PropertyWidget.cpp \
    PropertyView.cpp \
    Utils/InplaceEditing.cpp \
    Delegates/PropertyDelegateFactory.cpp \
    Delegates/Core/PropertyDelegateBool.cpp \
    Delegates/Core/PropertyDelegateInt.cpp \
    Delegates/Core/PropertyDelegateUInt.cpp \
    Delegates/Core/PropertyDelegateQString.cpp \
    Delegates/Core/PropertyDelegateDouble.cpp \
    Delegates/Core/PropertyDelegateEnum.cpp \
    Delegates/Core/PropertyDelegateQRect.cpp \
    Delegates/PropertyEditorHandler.cpp \
    Delegates/GUI/PropertyDelegateQColor.cpp \
    Delegates/Core/PropertyDelegateEnumFlags.cpp \
    Delegates/PropertyDelegate.cpp \
    Delegates/PropertyEditorAux.cpp \
    Delegates/Core/PropertyDelegateQSize.cpp \
    Delegates/Core/PropertyDelegateQPoint.cpp \
    Delegates/GUI/PropertyDelegateQFont.cpp \
    Utils/AccessibilityProxy.cpp \
    Utils/DoubleSpinBox.cpp \
    Utils/MultilineTextDialog.cpp \
    PropertyInt64.cpp \
    PropertyUInt64.cpp \
    PropertyQRectF.cpp \
    PropertyQPointF.cpp \
    PropertyQSizeF.cpp \
    PropertyWidgetEx.cpp \
    CustomPropertyEditorDialog.cpp \
    CustomPropertyOptionsDialog.cpp \
    VarProperty.cpp \
    PropertyQVariant.cpp \
    CustomPropertyWidget.cpp \
    QObjectPropertyWidget.cpp \
    MultiProperty.cpp \
    PropertyConnector.cpp \
    Config.cpp \
    Utils/QtnConnections.cpp \
    Utils/QtnInt64SpinBox.cpp \
    Auxiliary/PropertyDelegateInfo.cpp

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
    Core/PropertyQString.h \
    Core/PropertyQRect.h \
    Core/PropertyEnum.h \
    Core/PropertyEnumFlags.h \
    Core/PropertyQSize.h \
    Core/PropertyQPoint.h \
    GUI/PropertyQColor.h \
    GUI/PropertyQFont.h \
    PropertyWidget.h \
    PropertyView.h \
    Utils/InplaceEditing.h \
    Delegates/PropertyDelegate.h \
    Delegates/PropertyDelegateFactory.h \
    Delegates/Core/PropertyDelegateBool.h \
    Delegates/Core/PropertyDelegateInt.h \
    Delegates/Core/PropertyDelegateUInt.h \
    Delegates/Core/PropertyDelegateQString.h \
    Delegates/Core/PropertyDelegateDouble.h \
    Delegates/Core/PropertyDelegateEnum.h \
    Delegates/Core/PropertyDelegateQRect.h \
    Delegates/PropertyEditorHandler.h \
    Delegates/GUI/PropertyDelegateQColor.h \
    Delegates/Core/PropertyDelegateEnumFlags.h \
    Delegates/PropertyEditorAux.h \
    Delegates/Core/PropertyDelegateQSize.h \
    Delegates/Core/PropertyDelegateQPoint.h \
    Delegates/GUI/PropertyDelegateQFont.h \
    Utils/AccessibilityProxy.h \
    Utils/DoubleSpinBox.h \
    Utils/MultilineTextDialog.h \
    PropertyInt64.h \
    PropertyUInt64.h \
    PropertyQRectF.h \
    PropertyQPointF.h \
    PropertyQSizeF.h \
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
    Config.h \
    Utils/QtnConnections.h \
    Utils/QtnInt64SpinBox.h \
    PropertyDelegateAttrs.h

win32 {
    TARGET_EXT = .dll
}

TRANSLATIONS = \
    Translations/en.ts \
    Translations/ru.ts

for(tr, TRANSLATIONS):system($$[QT_INSTALL_BINS]/lrelease $$_PRO_FILE_PWD_/$${tr})

RESOURCES = QtnProperty.qrc

FORMS += \
    CustomPropertyEditorDialog.ui \
    CustomPropertyOptionsDialog.ui \
    Utils/MultilineTextDialog.ui

INCLUDEPATH += \
    $$_PRO_FILE_PWD_/. \
    $$_PRO_FILE_PWD_/..
