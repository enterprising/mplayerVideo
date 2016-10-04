#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T22:39:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mplayer_video
TEMPLATE = app


SOURCES += main.cpp\
        mplayer_video.cpp \
    fullscreen.cpp

HEADERS  += mplayer_video.h \
    fullscreen.h

FORMS    += mplayer_video.ui \
    fullscreen.ui

RC_FILE += mplayer_video.rc

DISTFILES += \
    mplayer_video.rc

RESOURCES += \
    resorce.qrc
