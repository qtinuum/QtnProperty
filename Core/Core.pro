include(../Common.pri)

TARGET = qtn_property_core
TEMPLATE = lib
VERSION = 1.0.1

DESTDIR = $$BIN_DIR

DEFINES += QTN_PE_CORE_LIBRARY

SOURCES += Property.cpp \
    Enum.cpp\
    PropertyBasis.cpp \
    PropertyCore.cpp \
    PropertyGUI.cpp \
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

HEADERS += CoreAPI.h\
    Property.h\
    Enum.h\
    PropertyBasis.h \
    PropertyCore.h \
    PropertyGUI.h \
    PropertyAux.h \
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

win32 {
    TARGET_EXT = .dll
} else:unix {
    target.path = /usr/lib
    INSTALLS += target
}
