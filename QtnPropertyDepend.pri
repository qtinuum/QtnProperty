isEmpty(QTNPROPERTY_BIN) {
    include($$PWD/Internal/BaseConfig.pri)
}

QTNPROPERTY_PATH = $$PWD

isEmpty(QTNPROPERTY_LIB) {
    QTNPROPERTY_LIB = $$QTNPROPERTY_BIN
}

QTNPROPERTY_LIBNAME = QtnProperty
qtnproperty_dynamic:QTNPROPERTY_LIBNAME = $$join(QTNPROPERTY_LIBNAME, , , 2)

win32 {
    msvc:PRE_TARGETDEPS += $$QTNPROPERTY_LIB/$$join(QTNPROPERTY_LIBNAME, , , .lib)
    else:PRE_TARGETDEPS += $$QTNPROPERTY_LIB/$$join(QTNPROPERTY_LIBNAME, , lib, .a)
}

qtnproperty_dynamic {
    DEFINES += QTN_DYNAMIC_IMPORT
    macx {
        DYNAMIC_LIBS.files += $$QTNPROPERTY_LIB/$$join(QTNPROPERTY_LIBNAME, , lib, .dylib)
    }
} else {
    unix {
        PRE_TARGETDEPS += $$QTNPROPERTY_LIB/$$join(QTNPROPERTY_LIBNAME, , lib, .a)
    }
}

LIBS += -L$$QTNPROPERTY_LIB
LIBS += -l$$QTNPROPERTY_LIBNAME

INCLUDEPATH += $$QTNPROPERTY_PATH
