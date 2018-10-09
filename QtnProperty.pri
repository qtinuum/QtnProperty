QTNPROPERTY_PATH = $$PWD

isEmpty(QTNPROPERTY_LIB) {
    QTNPROPERTY_LIB = $$BIN_DIR
}

macx {
    DYNAMIC_LIBS.files += $$QTNPROPERTY_LIB/libQtnPropertyAC.1.dylib
}

win32-msvc* {
    PRE_TARGETDEPS += $$QTNPROPERTY_LIB/QtnPropertyAC.lib
}

LIBS += -L$$QTNPROPERTY_LIB
LIBS += -lQtnPropertyAC

INCLUDEPATH += $$QTNPROPERTY_PATH

