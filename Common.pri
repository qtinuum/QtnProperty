BIN_DIR = $$TOP_BUILD_DIR/bin
DESTDIR = $$BIN_DIR

CONFIG += c++11 warn_off

unix|win32-g++ {
    QMAKE_CXXFLAGS_WARN_OFF -= -w
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += \
        -Wno-c++0x-compat \
        -Wno-unused-parameter \
        -Wno-unused-function \
        -Wno-inconsistent-missing-override \
        -Wno-deprecated-register \
        -Wno-unused-const-variable \
        -Wno-switch-bool
} else {
    win32 {
        QMAKE_CXXFLAGS_WARN_OFF -= -W0
        QMAKE_CXXFLAGS += -W3 /wd4100 /wd4065
        DEFINES += _CRT_SECURE_NO_WARNINGS
    }
}
