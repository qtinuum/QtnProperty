QTNPROPERTY_BIN_PREFIX = $$PWD/..

win32 {
    QTNPROPERTY_BIN = $$QTNPROPERTY_BIN_PREFIX/bin-win32
    msvc {
        QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-msvc
    }
}

macx:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN_PREFIX/bin-osx
else:linux:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN_PREFIX/bin-linux

isEmpty(QTNPROPERTY_BIN) {
    message("Only mac/win32/linux supported")
    QTNPROPERTY_BIN = "FAIL"
} else {
    clang:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-clang
    else:gcc:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-gcc

    QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-$$QT_ARCH
    CONFIG(debug, debug|release) {
        QTNPROPERTY_BIN = $$QTNPROPERTY_BIN/debug
    }
}
