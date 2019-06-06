include($$PWD/../Internal/BaseConfig.pri)
include($$PWD/../Internal/Config.pri)

QT += core gui widgets script

TARGET = QtnProperty
TEMPLATE = lib
VERSION = 2.0.0

SOURCES += $$PWD/PropertyBase.cpp \
    $$PWD/Property.cpp \
    $$PWD/PropertySet.cpp \
    $$PWD/Enum.cpp \
    $$PWD/QObjectPropertySet.cpp \
    $$PWD/Core/PropertyBool.cpp \
    $$PWD/Core/PropertyInt.cpp \
    $$PWD/Core/PropertyUInt.cpp \
    $$PWD/Core/PropertyDouble.cpp \
    $$PWD/Core/PropertyFloat.cpp \
    $$PWD/Core/PropertyQString.cpp \
    $$PWD/Core/PropertyQRect.cpp \
    $$PWD/Core/PropertyEnum.cpp \
    $$PWD/Core/PropertyEnumFlags.cpp \
    $$PWD/Core/PropertyQSize.cpp \
    $$PWD/Core/PropertyQPoint.cpp \
    $$PWD/GUI/PropertyQColor.cpp \
    $$PWD/GUI/PropertyQFont.cpp \
    $$PWD/GUI/PropertyQBrush.cpp \
    $$PWD/GUI/PropertyQPen.cpp \
    $$PWD/GUI/PropertyButton.cpp \
    $$PWD/PropertyWidget.cpp \
    $$PWD/PropertyView.cpp \
    $$PWD/Utils/InplaceEditing.cpp \
    $$PWD/Delegates/PropertyDelegate.cpp \
    $$PWD/Delegates/PropertyDelegateAux.cpp \
    $$PWD/Delegates/PropertyDelegateFactory.cpp \
    $$PWD/Delegates/Core/PropertyDelegateBool.cpp \
    $$PWD/Delegates/Core/PropertyDelegateInt.cpp \
    $$PWD/Delegates/Core/PropertyDelegateUInt.cpp \
    $$PWD/Delegates/Core/PropertyDelegateQString.cpp \
    $$PWD/Delegates/Core/PropertyDelegateDouble.cpp \
    $$PWD/Delegates/Core/PropertyDelegateFloat.cpp \
    $$PWD/Delegates/Core/PropertyDelegateEnum.cpp \
    $$PWD/Delegates/Core/PropertyDelegateQRect.cpp \
    $$PWD/Delegates/Core/PropertyDelegateQRectF.cpp \
    $$PWD/Delegates/Core/PropertyDelegateEnumFlags.cpp \
    $$PWD/Delegates/Core/PropertyDelegateQSize.cpp \
    $$PWD/Delegates/Core/PropertyDelegateQSizeF.cpp \
    $$PWD/Delegates/Core/PropertyDelegateQPoint.cpp \
    $$PWD/Delegates/Core/PropertyDelegateQPointF.cpp \
    $$PWD/Delegates/GUI/PropertyDelegateQColor.cpp \
    $$PWD/Delegates/GUI/PropertyDelegateQBrush.cpp \
    $$PWD/Delegates/GUI/PropertyDelegateQPen.cpp \
    $$PWD/Delegates/GUI/PropertyDelegateQFont.cpp \
    $$PWD/Delegates/GUI/PropertyDelegateButton.cpp \
    $$PWD/Delegates/Utils/PropertyEditorHandler.cpp \
    $$PWD/Delegates/Utils/PropertyEditorAux.cpp \
    $$PWD/Delegates/Utils/PropertyDelegateMisc.cpp \
    $$PWD/Delegates/Utils/PropertyDelegatePropertySet.cpp \
    $$PWD/Delegates/Utils/PropertyDelegateSliderBox.cpp \
    $$PWD/Delegates/Utils/PropertyDelegateGeoCoord.cpp \
    $$PWD/Delegates/Utils/PropertyDelegateGeoPoint.cpp \
    $$PWD/Utils/AccessibilityProxy.cpp \
    $$PWD/Utils/DoubleSpinBox.cpp \
    $$PWD/Utils/MultilineTextDialog.cpp \
    $$PWD/PropertyInt64.cpp \
    $$PWD/PropertyUInt64.cpp \
    $$PWD/Core/PropertyQRectF.cpp \
    $$PWD/Core/PropertyQPointF.cpp \
    $$PWD/Core/PropertyQSizeF.cpp \
    $$PWD/PropertyWidgetEx.cpp \
    $$PWD/CustomPropertyEditorDialog.cpp \
    $$PWD/CustomPropertyOptionsDialog.cpp \
    $$PWD/VarProperty.cpp \
    $$PWD/PropertyQVariant.cpp \
    $$PWD/CustomPropertyWidget.cpp \
    $$PWD/QObjectPropertyWidget.cpp \
    $$PWD/MultiProperty.cpp \
    $$PWD/PropertyConnector.cpp \
    $$PWD/Config.cpp \
    $$PWD/Utils/QtnConnections.cpp \
    $$PWD/Utils/QtnInt64SpinBox.cpp \
    $$PWD/Auxiliary/PropertyDelegateInfo.cpp \
    $$PWD/PropertyQKeySequence.cpp \
    $$PWD/PropertyDelegateMetaEnum.cpp

HEADERS += $$PWD/CoreAPI.h\
    $$PWD/PropertyBase.h \
    $$PWD/Property.h\
    $$PWD/PropertySet.h\
    $$PWD/Enum.h\
    $$PWD/QObjectPropertySet.h \
    $$PWD/PropertyCore.h \
    $$PWD/PropertyGUI.h \
    $$PWD/Auxiliary/PropertyTemplates.h \
    $$PWD/Auxiliary/PropertyMacro.h \
    $$PWD/Auxiliary/PropertyAux.h \
    $$PWD/Auxiliary/PropertyDelegateInfo.h \
    $$PWD/Core/PropertyBool.h \
    $$PWD/Core/PropertyInt.h \
    $$PWD/Core/PropertyUInt.h \
    $$PWD/Core/PropertyDouble.h \
    $$PWD/Core/PropertyFloat.h \
    $$PWD/Core/PropertyQString.h \
    $$PWD/Core/PropertyQRect.h \
    $$PWD/Core/PropertyEnum.h \
    $$PWD/Core/PropertyEnumFlags.h \
    $$PWD/Core/PropertyQSize.h \
    $$PWD/Core/PropertyQPoint.h \
    $$PWD/GUI/PropertyQColor.h \
    $$PWD/GUI/PropertyQFont.h \
    $$PWD/GUI/PropertyQBrush.h \
    $$PWD/GUI/PropertyQPen.h \
    $$PWD/GUI/PropertyButton.h \
    $$PWD/PropertyWidget.h \
    $$PWD/PropertyWidgetAPI.h \
    $$PWD/PropertyView.h \
    $$PWD/Utils/InplaceEditing.h \
    $$PWD/Delegates/PropertyDelegate.h \
    $$PWD/Delegates/PropertyDelegateAux.h \
    $$PWD/Delegates/PropertyDelegateFactory.h \
    $$PWD/Delegates/Core/PropertyDelegateBool.h \
    $$PWD/Delegates/Core/PropertyDelegateInt.h \
    $$PWD/Delegates/Core/PropertyDelegateUInt.h \
    $$PWD/Delegates/Core/PropertyDelegateQString.h \
    $$PWD/Delegates/Core/PropertyDelegateDouble.h \
    $$PWD/Delegates/Core/PropertyDelegateFloat.h \
    $$PWD/Delegates/Core/PropertyDelegateEnum.h \
    $$PWD/Delegates/Core/PropertyDelegateQRect.h \
    $$PWD/Delegates/Core/PropertyDelegateQRectF.h \
    $$PWD/Delegates/Core/PropertyDelegateEnumFlags.h \
    $$PWD/Delegates/Core/PropertyDelegateQSize.h \
    $$PWD/Delegates/Core/PropertyDelegateQSizeF.h \
    $$PWD/Delegates/Core/PropertyDelegateQPoint.h \
    $$PWD/Delegates/Core/PropertyDelegateQPointF.h \
    $$PWD/Delegates/GUI/PropertyDelegateQColor.h \
    $$PWD/Delegates/GUI/PropertyDelegateQFont.h \
    $$PWD/Delegates/GUI/PropertyDelegateQBrush.h \
    $$PWD/Delegates/GUI/PropertyDelegateQPen.h \
    $$PWD/Delegates/GUI/PropertyDelegateButton.h \
    $$PWD/Delegates/Utils/PropertyEditorHandler.h \
    $$PWD/Delegates/Utils/PropertyEditorAux.h \
    $$PWD/Delegates/Utils/PropertyDelegateMisc.h \
    $$PWD/Delegates/Utils/PropertyDelegatePropertySet.h \
    $$PWD/Delegates/Utils/PropertyDelegateSliderBox.h \
    $$PWD/Delegates/Utils/PropertyDelegateGeoCoord.h \
    $$PWD/Delegates/Utils/PropertyDelegateGeoPoint.h \
    $$PWD/Utils/AccessibilityProxy.h \
    $$PWD/Utils/DoubleSpinBox.h \
    $$PWD/Utils/MultilineTextDialog.h \
    $$PWD/PropertyInt64.h \
    $$PWD/PropertyUInt64.h \
    $$PWD/Core/PropertyQRectF.h \
    $$PWD/Core/PropertyQPointF.h \
    $$PWD/Core/PropertyQSizeF.h \
    $$PWD/PropertyWidgetEx.h \
    $$PWD/CustomPropertyEditorDialog.h \
    $$PWD/CustomPropertyOptionsDialog.h \
    $$PWD/VarProperty.h \
    $$PWD/PropertyQVariant.h \
    $$PWD/CustomPropertyWidget.h \
    $$PWD/QObjectPropertyWidget.h \
    $$PWD/IQtnPropertyStateProvider.h \
    $$PWD/MultiProperty.h \
    $$PWD/StructPropertyBase.h \
    $$PWD/PropertyConnector.h \
    $$PWD/Config.h \
    $$PWD/Utils/QtnConnections.h \
    $$PWD/Utils/QtnInt64SpinBox.h \
    $$PWD/PropertyDelegateAttrs.h \
    $$PWD/PropertyQKeySequence.h \
    $$PWD/PropertyDelegateMetaEnum.h

RESOURCES = $$PWD/QtnProperty.qrc

FORMS += \
    $$PWD/CustomPropertyEditorDialog.ui \
    $$PWD/CustomPropertyOptionsDialog.ui \
    $$PWD/Utils/MultilineTextDialog.ui

INCLUDEPATH += $$PWD $$PWD/..

TRANSLATIONS = \
    $$PWD/Translations/en.ts \
    $$PWD/Translations/ru.ts

for(tr, TRANSLATIONS):system($$[QT_INSTALL_BINS]/lrelease $${tr})
