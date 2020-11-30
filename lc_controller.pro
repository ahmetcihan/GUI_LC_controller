#-------------------------------------------------
#
# Project created by QtCreator 2019-03-06T17:54:14
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = lc_controller
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    remoteconnection.cpp \
    ethernet.cpp

HEADERS  += mainwindow.h \
    remoteconnection.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
