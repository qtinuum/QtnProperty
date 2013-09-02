ROOT_DIR = $$PWD

CONFIG(release, debug|release): BIN_DIR = $$ROOT_DIR/bin/Release/
CONFIG(debug, debug|release): BIN_DIR = $$ROOT_DIR/bin/Debug/

win32:QMAKE_CXXFLAGS += /wd4100 /wd4065 -D_CRT_SECURE_NO_WARNINGS
unix:QMAKE_CXXFLAGS += -std=c++0x -Wno-c++0x-compat -Wno-unused-parameter -Wno-unused-function
