#-------------------------------------------------
#
# Project created by QtCreator 2018-09-27T17:10:22
#
#-------------------------------------------------

QT       += core gui\
            multimedia\
            widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PrettiesInJungle
TEMPLATE = app


SOURCES += main.cpp\
        main_window.cpp \
    setwidgetbgp_class.cpp \
    custom_btn.cpp \
    gallery_widget.cpp \
    help_widget.cpp \
    junglebubble_widget.cpp \
    record_widget.cpp \
    remind_dlg.cpp

HEADERS  += main_window.h \
    setwidgetbgp_class.h \
    all_define.h \
    custom_btn.h \
    gallery_widget.h \
    help_widget.h \
    junglebubble_widget.h \
    record_widget.h \
    remind_dlg.h

FORMS    += main_window.ui \
    gallery_widget.ui \
    help_widget.ui \
    junglebubble_widget.ui \
    record_widget.ui \
    remind_dlg.ui

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

RESOURCES += \
    BGM.qrc \
    SE.qrc \
    BGP.qrc \
    RoleBGP.qrc \
    CoverLabel.qrc \
    Bubble.qrc \
    Dlg.qrc \
    Button.qrc \
    MarkLabel.qrc \
    INI.qrc

