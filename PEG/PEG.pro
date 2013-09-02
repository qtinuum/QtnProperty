include(../Common.pri)
include(Bison.pri)
include(Flex.pri)

QT       += core
QT       -= gui

TARGET = peg

DESTDIR = $$BIN_DIR

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    PropertyEnumGenerator.cpp

BISON_SOURCES += PropertyEnum.y
FLEX_SOURCES += PropertyEnum.l

OTHER_FILES += \
    PropertyEnum.l \
    PropertyEnum.y

HEADERS += \
    PropertyEnumGeneratorCommon.h \
    PropertyEnumGenerator.h

#LIBS += -L$$BIN_DIR -lProperty
