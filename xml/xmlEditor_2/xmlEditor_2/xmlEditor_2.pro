#-------------------------------------------------
#
# Project created by QtCreator 2015-01-29T18:01:29
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xmlEditor_2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        TreeItem.cpp \
        TreeModel.cpp \

HEADERS  += mainwindow.h\
            TreeItem.h \
            TreeModel.h \

FORMS    += mainwindow.ui
