include(../Common.pri)
PEG_TOOL = $$BIN_DIR/peg
include(../PEG.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo_app
TEMPLATE = app
DESTDIR = $$BIN_DIR

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

PEG_SOURCES += Demo.pef

INCLUDEPATH += $$ROOT_DIR/PropertyWidget

LIBS += -L$$BIN_DIR -lqtn_property_core -lqtn_property_widget

win32 {
} else:unix {
    QMAKE_LFLAGS += -Wl,-rpath,$$BIN_DIR
}

OTHER_FILES += \
    Demo.pef
