TEMPLATE   = subdirs
SUBDIRS   += \
    QtnProperty \
    PEG \
    Tests \
    Demo

Tests.depends = QtnProperty
Demo.depends = QtnProperty