CONFIG += qtn_unity_build
QT += script widgets

QTNPROPVERSION = 1.0.0

INCLUDEPATH += $$PWD $$PWD/Core $$PWD/PropertyWidget

VPATH += $$PWD/Core $$PWD/PropertyWidget

SOURCES_CORE += PropertyBase.cpp \
    Property.cpp \
    PropertySet.cpp \
    Enum.cpp \
    QObjectPropertySet.cpp \
    Core/PropertyBool.cpp \
    Core/PropertyInt.cpp \
    Core/PropertyUInt.cpp \
    Core/PropertyFloat.cpp \
    Core/PropertyDouble.cpp \
    Core/PropertyQString.cpp \
    Core/PropertyQRect.cpp \
    Core/PropertyEnum.cpp \
    Core/PropertyEnumFlags.cpp \
    Core/PropertyQSize.cpp \
    Core/PropertyQPoint.cpp \
    GUI/PropertyQColor.cpp \
    GUI/PropertyButton.cpp \
    GUI/PropertyQPen.cpp \
    GUI/PropertyQBrush.cpp \
    GUI/PropertyQFont.cpp

SOURCES_WIDGET += PropertyWidget.cpp \
    PropertyView.cpp \
    Utils/InplaceEditing.cpp \
    Delegates/PropertyDelegateFactory.cpp \
    Delegates/Core/PropertyDelegateBool.cpp \
    Delegates/Core/PropertyDelegateInt.cpp \
    Delegates/Core/PropertyDelegateUInt.cpp \
    Delegates/Core/PropertyDelegateQString.cpp \
    Delegates/Core/PropertyDelegateFloat.cpp \
    Delegates/Core/PropertyDelegateDouble.cpp \
    Delegates/Core/PropertyDelegateEnum.cpp \
    Delegates/Core/PropertyDelegateQRect.cpp \
    Delegates/Utils/PropertyEditorHandler.cpp \
    Delegates/Core/PropertyDelegateEnumFlags.cpp \
    Delegates/PropertyDelegate.cpp \
    Delegates/PropertyDelegateAux.cpp \
    Delegates/Utils/PropertyDelegatePropertySet.cpp \
    Delegates/Utils/PropertyDelegateSliderBox.cpp \
    Delegates/Utils/PropertyDelegateMisc.cpp \
    Delegates/Utils/PropertyEditorAux.cpp \
    Delegates/Core/PropertyDelegateQSize.cpp \
    Delegates/Core/PropertyDelegateQPoint.cpp \
    Delegates/GUI/PropertyDelegateQFont.cpp \
    Delegates/GUI/PropertyDelegateQColor.cpp \
    Delegates/GUI/PropertyDelegateButton.cpp \
    Delegates/GUI/PropertyDelegateQPen.cpp \
    Delegates/GUI/PropertyDelegateQBrush.cpp \
    Utils/AccessibilityProxy.cpp

qtn_unity_build: SOURCES += $$PWD/QtnPropertyUnity.cpp
else: SOURCES += $$SOURCES_CORE $$SOURCES_WIDGET

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
    Core/PropertyFloat.h \
    Core/PropertyDouble.h \
    Core/PropertyQString.h \
    Core/PropertyQRect.h \
    Core/PropertyEnum.h \
    Core/PropertyEnumFlags.h \
    Core/PropertyQSize.h \
    Core/PropertyQPoint.h \
    GUI/PropertyQColor.h \
    GUI/PropertyButton.h \
    GUI/PropertyQPen.h \
    GUI/PropertyQBrush.h \
    GUI/PropertyQFont.h

HEADERS += PropertyWidgetAPI.h \
    PropertyWidget.h \
    PropertyView.h \
    Utils/InplaceEditing.h \
    Delegates/PropertyDelegate.h \
    Delegates/Utils/PropertyDelegatePropertySet.h \
    Delegates/Utils/PropertyDelegateSliderBox.h \
    Delegates/Utils/PropertyDelegateMisc.h \
    Delegates/PropertyDelegateAux.h \
    Delegates/PropertyDelegateFactory.h \
    Delegates/Core/PropertyDelegateBool.h \
    Delegates/Core/PropertyDelegateInt.h \
    Delegates/Core/PropertyDelegateUInt.h \
    Delegates/Core/PropertyDelegateQString.h \
    Delegates/Core/PropertyDelegateFloat.h \
    Delegates/Core/PropertyDelegateDouble.h \
    Delegates/Core/PropertyDelegateEnum.h \
    Delegates/Core/PropertyDelegateQRect.h \
    Delegates/Utils/PropertyEditorHandler.h \
    Delegates/Core/PropertyDelegateEnumFlags.h \
    Delegates/Utils/PropertyEditorAux.h \
    Delegates/Core/PropertyDelegateQSize.h \
    Delegates/Core/PropertyDelegateQPoint.h \
    Delegates/GUI/PropertyDelegateQFont.h \
    Delegates/GUI/PropertyDelegateQColor.h \
    Delegates/GUI/PropertyDelegateButton.h \
    Delegates/GUI/PropertyDelegateQPen.h \
    Delegates/GUI/PropertyDelegateQBrush.h \
    Utils/AccessibilityProxy.h
