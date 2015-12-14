BIN_DIR = $$TOP_BUILD_DIR/bin
DESTDIR = $$BIN_DIR

CONFIG+=c++11

win32:QMAKE_CXXFLAGS += /wd4100 /wd4065 -D_CRT_SECURE_NO_WARNINGS
unix {
    QMAKE_CFLAGS_WARN_ON -= -Wall -W
    QMAKE_CXXFLAGS_WARN_ON -= -Wall -W

    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += \
        -Wno-c++0x-compat \
        -Wno-unused-parameter \
        -Wno-unused-function \
        -Wno-inconsistent-missing-override \
        -Wno-deprecated-register \
        -Wno-unused-const-variable \
        -Wno-switch-bool
}
