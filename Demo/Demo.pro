include(../QtnPropertyDepend.pri)
include(../Internal/TargetConfig.pri)
include(../PEG/PEG.pri)

QT += core gui widgets script scripttools

TARGET = QtnPropertyDemo
TEMPLATE = app

SOURCES +=  main.cpp\
            MainWindow.cpp \
            mydialog.cpp \
            AB/PropertyABColor.cpp \
            AB/PropertyDelegateABColor.cpp \
            Int/PropertyDelegateIntList.cpp \
            Layer/PropertyLayer.cpp \
            Layer/PropertyDelegateLayer.cpp \
            PenWidth/PropertyPenWidth.cpp \
            PenWidth/PropertyDelegatePenWidth.cpp \
            Freq/PropertyFreq.cpp \
            Freq/PropertyDelegateFreq.cpp

HEADERS  += MainWindow.h \
            mydialog.h \
            AB/PropertyABColor.h \
            AB/PropertyDelegateABColor.h \
            Int/PropertyDelegateIntList.h \
            Layer/PropertyLayer.h \
            Layer/PropertyDelegateLayer.h \
            PenWidth/PropertyPenWidth.h \
            PenWidth/PropertyDelegatePenWidth.h \
            Freq/PropertyFreq.h \
            Freq/PropertyDelegateFreq.h

FORMS    += MainWindow.ui \
            mydialog.ui

PEG_SOURCES += Demo.pef

OTHER_FILES += \
    Demo.pef

DYNAMIC_LIBS.path = "Contents/MacOS"

QMAKE_BUNDLE_DATA += DYNAMIC_LIBS
