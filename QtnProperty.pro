TEMPLATE   = subdirs
SUBDIRS   += Library \
             PEG \
             Tests \
             Demo

Library.file = QtnProperty/QtnProperty.pro

PEG.depends = Library
Tests.depends = Library
Demo.depends = Library
