#-------------------------------------------------
#
# Project created by QtCreator 2014-10-04T00:10:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_function
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    registration.cpp \
    request.cpp \
    requestmanager.cpp

HEADERS  += mainwindow.h \
    registration.h \
    request.h \
    requestmanager.h

FORMS    += mainwindow.ui \
    registration.ui
