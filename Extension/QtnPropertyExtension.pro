include($$_PRO_FILE_PWD_/../Common.pri)
include($$_PRO_FILE_PWD_/../QtnProperty.pri)

QT += core gui widgets

TARGET = QtnPropertyExtension
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    PropertyInt64.h \
    PropertyUInt64.h \
    Extension.h \
    PropertyQRectF.h \
    PropertyQPointF.h \
    PropertyQSizeF.h \
    PropertyWidgetEx.h \
    PropertyPercent.h \
    CustomPropertyEditorDialog.h \
    CustomPropertyOptionsDialog.h \
    VarProperty.h \
    PropertyQVariant.h \
    CustomPropertyWidget.h \
    QObjectPropertyWidget.h

SOURCES += \
    PropertyInt64.cpp \
    PropertyUInt64.cpp \
    Extension.cpp \
    PropertyQRectF.cpp \
    PropertyQPointF.cpp \
    PropertyQSizeF.cpp \
    PropertyWidgetEx.cpp \
    PropertyPercent.cpp \
    CustomPropertyEditorDialog.cpp \
    CustomPropertyOptionsDialog.cpp \
    VarProperty.cpp \
    PropertyQVariant.cpp \
    CustomPropertyWidget.cpp \
    QObjectPropertyWidget.cpp

TRANSLATIONS = \
    Translations/QtnPropertyExtension_en.ts \
    Translations/QtnPropertyExtension_ru.ts

for(tr, TRANSLATIONS):system($$[QT_INSTALL_BINS]/lrelease $$_PRO_FILE_PWD_/$${tr})

RESOURCES = QtnPropertyExtension.qrc

FORMS += \
    CustomPropertyEditorDialog.ui \
    CustomPropertyOptionsDialog.ui
