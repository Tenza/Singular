#-------------------------------------------------
#
# Project created by QtCreator 2014-09-27T01:51:13
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Singular
TEMPLATE = app


SOURCES += main.cpp\
    singular.cpp \
    camerasurface.cpp \
    camerawidget.cpp \
    sensors.cpp \
    textstream.cpp \
    settingsmanager.cpp \
    output.cpp \
    helper.cpp \
    audiowidget.cpp \
    audioinputsurface.cpp \
    audiooutputsurface.cpp

HEADERS  += singular.h \
    camerasurface.h \
    camerawidget.h \
    sensors.h \
    defines.h \
    textstream.h \
    settingsmanager.h \
    output.h \
    helper.h \
    audiowidget.h \
    audioinputsurface.h \
    audiooutputsurface.h

FORMS    += singular.ui
