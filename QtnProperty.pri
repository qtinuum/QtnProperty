QTNPROPERTY_PATH = $$PWD

isEmpty(QTNPROPERTY_LIB) {
    QTNPROPERTY_LIB = $$BIN_DIR
}

macx {
    DYNAMIC_LIBS.files += $$QTNPROPERTY_LIB/libQtnProperty.dylib
}

win32-msvc* {
    PRE_TARGETDEPS += $$QTNPROPERTY_LIB/QtnProperty.lib
}

LIBS += -L$$QTNPROPERTY_LIB
LIBS += -lQtnProperty

INCLUDEPATH += $$QTNPROPERTY_PATH
