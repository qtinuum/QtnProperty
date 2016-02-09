isEmpty(QTNPROPERTY_PATH) {
    QTNPROPERTY_PATH = $$_PRO_FILE_PWD_/..
}

isEmpty(QTNPROPERTY_LIB) {
    QTNPROPERTY_LIB = $$BIN_DIR
} else {
    LIBS += -L$$QTNPROPERTY_LIB
}

macx:DYNAMIC_LIBS.files += \
    $$QTNPROPERTY_LIB/libQtnPropertyCoreAC.1.dylib \
    $$QTNPROPERTY_LIB/libQtnPropertyWidgetAC.1.dylib

LIBS += -lQtnPropertyCoreAC -lQtnPropertyWidgetAC

INCLUDEPATH += \
    $$QTNPROPERTY_PATH/../ \
    $$QTNPROPERTY_PATH/Widget

