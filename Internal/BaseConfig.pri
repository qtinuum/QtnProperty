QTNPROPERTY_BIN_PREFIX = $$PWD/..

QTNPROPERTY_BIN = $$QTNPROPERTY_BIN_PREFIX/bin

win32 {
    QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-win32
    msvc:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-msvc
}

emscripten:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-emscripten
else:macx:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-osx
else:linux:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-linux

isEmpty(QTNPROPERTY_BIN) {
    message("Only mac/win32/linux/emscripten supported")
    QTNPROPERTY_BIN = "FAIL"
} else {
    clang:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-clang
    else:gcc:QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-gcc

    QTNPROPERTY_BIN = $$QTNPROPERTY_BIN-$$QT_ARCH
    CONFIG(debug, debug|release) {
        QTNPROPERTY_BIN = $$QTNPROPERTY_BIN/debug
    }
}
