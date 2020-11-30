TEMPLATE   = subdirs
SUBDIRS   += \
    QtnProperty \
    PEG \
    Tests \
    Demo

Tests.depends = PEG QtnProperty
Demo.depends = PEG QtnProperty

OTHER_FILES += \
	README.md \
	CHANGELOG \
	LICENSE \
	AUTHORS \
	TODO