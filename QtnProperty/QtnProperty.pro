include(../Internal/TargetConfig.pri)

TARGET = QtnProperty
TEMPLATE = lib
VERSION = 2.0.2

qtnproperty_dynamic {
    DEFINES += QTN_DYNAMIC_LIBRARY
    macx {
        QMAKE_SONAME_PREFIX = @rpath
    }

    CONFIG += shared
} else {
    CONFIG += static
}

include(./QtnProperty.pri)
