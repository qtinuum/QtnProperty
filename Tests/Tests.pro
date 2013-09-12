include(../Common.pri)
PEG_TOOL = $$BIN_DIR/peg
include(../PEG.pri)

QT       += core
QT       += testlib

TARGET = property_tests

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#DEFINES += DISABLE_QTINUUM_NAMESPACE

PEG_SOURCES += PEG/test.pef \
                PEG/test2.pef

OTHER_FILES += $$PEG_SOURCES

HEADERS += \
    TestProperty.h \
    TestGeneratedProperty.h \
    TestEnum.h

SOURCES += main.cpp \
    TestProperty.cpp \
    TestGeneratedProperty.cpp \
    TestEnum.cpp


LIBS += -L$$BIN_DIR -lqtn_property_core
win32 {
} else:unix {
    QMAKE_LFLAGS += -Wl,-rpath,$$BIN_DIR
}

INCLUDEPATH += $$ROOT_DIR/Core/


