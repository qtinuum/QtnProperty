TEMPLATE   = subdirs
SUBDIRS   += \
    QtnProperty \
    PEG \
    Tests \
    Demo

Tests.depends = PEG QtnProperty
Demo.depends = PEG QtnProperty
