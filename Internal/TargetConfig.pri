isEmpty(QTNPROPERTY_BIN) {
    include($$PWD/BaseConfig.pri)
}

DESTDIR = $$QTNPROPERTY_BIN

CONFIG += c++11

msvc {
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

macx:DYNAMIC_LIBS.path = "Contents/MacOS"

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
} else {
    DEFINES += NDEBUG
}
