include(../Config.pri)
PEG_TOOL = $$BIN_DIR/QtnPEG
include(../PEG.pri)

QT += core gui widgets script scripttools

TARGET = QtnPropertyDemo
TEMPLATE = app

SOURCES +=  main.cpp\
            MainWindow.cpp \
            mydialog.cpp \
            AB/PropertyABColor.cpp \
            AB/PropertyDelegateABColor.cpp \
            Int/PropertyDelegateIntList.cpp

HEADERS  += MainWindow.h \
            mydialog.h \
            AB/PropertyABColor.h \
            AB/PropertyDelegateABColor.h \
            Int/PropertyDelegateIntList.h

FORMS    += MainWindow.ui \
            mydialog.ui

PEG_SOURCES += Demo.pef

INCLUDEPATH += $$TOP_SRC_DIR/Core
INCLUDEPATH += $$TOP_SRC_DIR/PropertyWidget

LIBS += -L$$BIN_DIR -lQtnPropertyCore -lQtnPropertyWidget

unix:PRE_TARGETDEPS += $$BIN_DIR/libQtnPropertyCore.a $$BIN_DIR/libQtnPropertyWidget.a
else:PRE_TARGETDEPS += $$BIN_DIR/QtnPropertyCore.lib  $$BIN_DIR/QtnPropertyWidget.lib

win32 {
} else:unix {
    QMAKE_LFLAGS += -Wl,-rpath,\'\$$ORIGIN\'
}

OTHER_FILES += \
    Demo.pef
