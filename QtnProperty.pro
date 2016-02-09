TEMPLATE   = subdirs
SUBDIRS   += Core \
             PEG \
             Tests \
             Widget \
             Extension \
             Demo

Core.file = Core/QtnPropertyCore.pro
Widget.file = Widget/QtnPropertyWidget.pro
Extension.file = Extension/QtnPropertyExtension.pro

PEG.depends = Core
Tests.depends = Core
Widget.depends = Core
Extension.depends = Widget
Demo.depends = Widget
