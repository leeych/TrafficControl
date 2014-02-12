#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T10:37:32
#
#-------------------------------------------------

QT       += core gui
QT       += network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrafficControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    synccommand.cpp \
    command.cpp \
    mutility.cpp \
    systeminstaller.cpp \
    xmlhelper.cpp

HEADERS  += mainwindow.h \
    synccommand.h \
    command.h \
    macrostring.h \
    mutility.h \
    systeminstaller.h \
    xmlhelper.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

TRANSLATIONS += ../resource/languages/trafficCtrl_cn.ts

RESOURCES += \
    ../resource/trafficCtrl.qrc
