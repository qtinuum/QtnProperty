isEmpty(TOP_SRC_DIR):TOP_SRC_DIR = $$PWD

macx:BIN_DIR = $$TOP_SRC_DIR/bin-osx
win32:BIN_DIR = $$TOP_SRC_DIR/bin-win
linux:BIN_DIR = $$TOP_SRC_DIR/bin-linux

isEmpty(BIN_DIR) {
    MESSAGE(CRITICAL, "Only mac/win32/linux supported")
} else {
    CONFIG(debug, debug|release) {
        DEFINES += DEBUG
        BIN_DIR = $$BIN_DIR/debug
    } else {
        DEFINES += NDEBUG
    }

    DESTDIR = $$BIN_DIR
}

CONFIG += c++11

msvc {
    DEFINES += _CRT_SECURE_NO_WARNINGS
}
