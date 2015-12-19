mac:BIN_DIR = $$TOP_BUILD_DIR/bin-osx
win32:BIN_DIR = $$TOP_BUILD_DIR/bin-win
linux:BIN_DIR = $$TOP_BUILD_DIR/bin-linux
isEmpty(BIN_DIR):MESSAGE(CRITICAL, "Only mac/win32/linux supported")
else:DESTDIR = $$BIN_DIR

include("Common.pri")
