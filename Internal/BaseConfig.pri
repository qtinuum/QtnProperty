macx:BIN_DIR = $$PWD/bin-osx
win32:BIN_DIR = $$PWD/bin-win
linux:BIN_DIR = $$PWD/bin-linux

isEmpty(BIN_DIR) {
    MESSAGE(CRITICAL, "Only mac/win32/linux supported")
} else {
    CONFIG(debug, debug|release) {
        DEFINES += DEBUG
        BIN_DIR = $$BIN_DIR/debug
    } else {
        DEFINES += NDEBUG
    }
}