#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T22:39:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simplewindow
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    simplewindow.cpp \
    FramelessHelper.cpp \
    FullscreenHelper.cpp \
    panel.cpp

HEADERS  += mainwindow.h \
    simplewindow.h \
    FramelessHelper.h \
    FullscreenHelper.h \
    panel.h

FORMS    += mainwindow.ui \
    panel.ui
