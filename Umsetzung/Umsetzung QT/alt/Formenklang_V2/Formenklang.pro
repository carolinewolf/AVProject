#-------------------------------------------------
#
# Project created by QtCreator 2018-10-30T10:40:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Formenklang
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    formkeyer.cpp \
    form.cpp

HEADERS += \
        mainwindow.h \
    formkeyer.h \
    form.h

FORMS += \
        mainwindow.ui


include(../opencv/videoengine.pri)
include(../drumstick/drumstick.pro)

