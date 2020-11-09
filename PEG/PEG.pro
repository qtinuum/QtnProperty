include(../Internal/TargetConfig.pri)
include(Bison.pri)
include(Flex.pri)

QT += core
QT -= gui

clang:QMAKE_CXXFLAGS_WARN_ON += \
    -Wno-deprecated-register \
    -Wno-unneeded-internal-declaration \
    -Wno-unused-const-variable

gcc:QMAKE_CXXFLAGS_WARN_ON += \
    -Wno-unused-function

TARGET = QtnPEG

CONFIG += cmdline
CONFIG -= app_bundle

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
