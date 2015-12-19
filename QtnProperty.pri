include(Common.pri)

QT += script widgets

QTNPROPVERSION = 1.0.0

INCLUDEPATH += $$PWD $$PWD/Core $$PWD/PropertyWidget

DEFINES += STATICLIB QTN_PE_CORE_LIBRARY QTN_PE_PROPERTYWIDGET_LIBRARY
VPATH += $$PWD/Core $$PWD/PropertyWidget

SOURCES += PropertyBase.cpp \
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
    GUI/PropertyQFont.cpp

SOURCES += PropertyWidget.cpp \
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
    Delegates/PropertyEditorHandler.cpp \
    Delegates/GUI/PropertyDelegateQColor.cpp \
    Delegates/Core/PropertyDelegateEnumFlags.cpp \
    Delegates/PropertyDelegate.cpp \
    Delegates/PropertyEditorAux.cpp \
    Delegates/Core/PropertyDelegateQSize.cpp \
    Delegates/Core/PropertyDelegateQPoint.cpp \
    Delegates/GUI/PropertyDelegateQFont.cpp

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
    GUI/PropertyQFont.h

HEADERS += PropertyWidgetAPI.h \
    PropertyWidget.h \
    PropertyView.h \
    Utils/InplaceEditing.h \
    Delegates/PropertyDelegate.h \
    Delegates/PropertyDelegateFactory.h \
    Delegates/Core/PropertyDelegateBool.h \
    Delegates/Core/PropertyDelegateInt.h \
    Delegates/Core/PropertyDelegateUInt.h \
    Delegates/Core/PropertyDelegateQString.h \
    Delegates/Core/PropertyDelegateFloat.h \
    Delegates/Core/PropertyDelegateDouble.h \
    Delegates/Core/PropertyDelegateEnum.h \
    Delegates/Core/PropertyDelegateQRect.h \
    Delegates/PropertyEditorHandler.h \
    Delegates/GUI/PropertyDelegateQColor.h \
    Delegates/Core/PropertyDelegateEnumFlags.h \
    Delegates/PropertyEditorAux.h \
    Delegates/Core/PropertyDelegateQSize.h \
    Delegates/Core/PropertyDelegateQPoint.h \
    Delegates/GUI/PropertyDelegateQFont.h

