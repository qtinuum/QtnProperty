include(../QtnPropertyDepend.pri)
include(../Internal/TargetConfig.pri)
include(../PEG/PEG.pri)

QT += core gui widgets script testlib

TARGET = QtnPropertyTests

CONFIG   += cmdline

TEMPLATE = app

HEADERS += \
    TestProperty.h \
    TestGeneratedProperty.h \
    TestEnum.h

SOURCES += main.cpp \
    TestProperty.cpp \
    TestGeneratedProperty.cpp \
    TestEnum.cpp

PEG_SOURCES += PEG/test.pef \
               PEG/test2.pef

OTHER_FILES += $$PEG_SOURCES


