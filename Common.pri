BIN_DIR = $$TOP_BUILD_DIR/bin
DESTDIR = $$BIN_DIR

win32:QMAKE_CXXFLAGS += /wd4100 /wd4065 -D_CRT_SECURE_NO_WARNINGS
unix:QMAKE_CXXFLAGS += -std=c++0x -Wno-c++0x-compat -Wno-unused-parameter -Wno-unused-function
