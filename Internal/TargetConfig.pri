isEmpty(QTNPROPERTY_BIN) {
    include($$PWD/BaseConfig.pri)
}

DESTDIR = $$QTNPROPERTY_BIN

CONFIG += c++17

msvc {
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

clang:QMAKE_CXXFLAGS_WARN_ON += \
    -Wno-deprecated-copy \
    -Wno-unknown-warning-option \
    -Wno-deprecated-declarations

macx:DYNAMIC_LIBS.path = "Contents/MacOS"

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
} else {
    DEFINES += NDEBUG
}
