include(../Common.pri)

TARGET = QtnPropertyWidget
TEMPLATE = lib
VERSION = 1.0.0

QT += core gui widgets

DEFINES += QTN_PE_PROPERTYWIDGET_LIBRARY

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
    Delegates/GUI/PropertyDelegateQFont.cpp \
    Utils/AccessibilityProxy.cpp

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
    Delegates/GUI/PropertyDelegateQFont.h \
    Utils/AccessibilityProxy.h

LIBS += -L$$BIN_DIR -lQtnPropertyCore

win32 {
    TARGET_EXT = .dll
}
