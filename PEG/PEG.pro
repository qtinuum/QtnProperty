include(../Internal/TargetConfig.pri)
include(Bison.pri)
include(Flex.pri)

QT += core
QT -= gui

TARGET = QtnPEG

CONFIG += cmdline

TEMPLATE = app
VERSION = 2.0.0

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

msvc:QMAKE_CXXFLAGS += /wd4065 /wd4267 /wd4005
