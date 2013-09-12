ROOT_DIR = $$PWD

CONFIG(release, debug|release): CONFIG_DIR = Release
CONFIG(debug, debug|release): CONFIG_DIR = Debug

BIN_DIR = $$ROOT_DIR/bin/$$CONFIG_DIR
TMP_DIR = $$ROOT_DIR/tmp/$$CONFIG_DIR

win32:QMAKE_CXXFLAGS += /wd4100 /wd4065 -D_CRT_SECURE_NO_WARNINGS
unix:QMAKE_CXXFLAGS += -std=c++0x -Wno-c++0x-compat -Wno-unused-parameter -Wno-unused-function

DESTDIR = $$BIN_DIR/
OBJECTS_DIR = $$TMP_DIR/$$TARGET
MOC_DIR = $$TMP_DIR/$$TARGET
