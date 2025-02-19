#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T12:48:34
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qthivex
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hivemodel.cpp \
    hiveitem.cpp \
    nodeitem.cpp \
    hiveview.cpp \
    hivevaluesmodel.cpp \
    valueitem.cpp

HEADERS  += mainwindow.h \
    hivemodel.h \
    hiveitem.h \
    nodeitem.h \
    hiveview.h \
    hivevaluesmodel.h \
    valueitem.h

FORMS    += mainwindow.ui

CONFIG += c++11

RESOURCES += \
    resources.qrc

unix:!macx: LIBS += -lhivex
