TEMPLATE   = subdirs
SUBDIRS   += PEG\
             Core\
             Tests\
             PropertyWidget\
             Demo

#PEG.depends = Core
Tests.depends = Core
PropertyWidget.depends = Core
Demo.depends = Core
Demo.depends = PropertyWidget
