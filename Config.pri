
isEmpty(TOP_SRC_DIR):MESAGE(FATAL "TOP_SRC_DIR directory is not defined.")

macx:BIN_DIR = $$TOP_SRC_DIR/bin-osx
win32:BIN_DIR = $$TOP_SRC_DIR/bin-win
linux:BIN_DIR = $$TOP_SRC_DIR/bin-linux
isEmpty(BIN_DIR):MESSAGE(CRITICAL, "Only mac/win32/linux supported")
else:DESTDIR = $$BIN_DIR


debug: DBG = -dgb

OBJECTS_DIR = $$TOP_SRC_DIR/workdir/$$DBG-$$[QMAKE_SPEC]/$$TARGET/obj
MOC_DIR = $$TOP_SRC_DIR/workdir/$$DBG-$$[QMAKE_SPEC]/$$TARGET/obj
UI_DIR = $$TOP_SRC_DIR/workdir/$$DBG-$$[QMAKE_SPEC]/$$TARGET/obj
RCC_DIR = $$TOP_SRC_DIR/workdir/$$DBG-$$[QMAKE_SPEC]/$$TARGET/obj

include("Common.pri")
