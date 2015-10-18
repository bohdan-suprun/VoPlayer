#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T22:39:54
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VoPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    timeconteiner.cpp \
    subtitles.cpp \
    videoplayer.cpp \
    registration.cpp \
    requestmanager.cpp \
    request.cpp

HEADERS  += mainwindow.h \
    timeconteiner.h \
    subtitles.h \
    videoplayer.h \
    registration.h \
    requestmanager.h \
    request.h

FORMS    += mainwindow.ui \
    registration.ui
