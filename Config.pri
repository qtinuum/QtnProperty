
isEmpty(TOP):MESAGE(FATAL "TOP directory is not defined.")

macx:BIN_DIR = $$TOP/bin-osx
win32:BIN_DIR = $$TOP/bin-win
linux:BIN_DIR = $$TOP/bin-linux
isEmpty(BIN_DIR):MESSAGE(CRITICAL, "Only mac/win32/linux supported")
else:DESTDIR = $$BIN_DIR


debug: DBG = -dgb

OBJECTS_DIR = $$TOP/build$$DBG-$$[QMAKE_SPEC]/$$TARGET/obj
MOC_DIR = $$TOP/build$$DBG-$$[QMAKE_SPEC]/$$TARGET/obj
UI_DIR = $$TOP/build$$DBG-$$[QMAKE_SPEC]/$$TARGET/obj
RCC_DIR = $$TOP/build$$DBG-$$[QMAKE_SPEC]/$$TARGET/obj

include("Common.pri")
