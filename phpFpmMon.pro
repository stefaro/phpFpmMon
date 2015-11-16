#-------------------------------------------------
#
# Project created by QtCreator 2015-06-23T08:24:54
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phpFpmMon
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    restclient.cpp \
    phpfpmstatus.cpp

HEADERS  += mainwindow.h \
    restclient.h \
    phpfpmstatus.h

FORMS    += mainwindow.ui
