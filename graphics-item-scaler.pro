#-------------------------------------------------
#
# Project created by QtCreator 2016-05-05T16:14:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphics-item-scaler
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        scaler/graphicsitemscaler.cpp \
        scaler/handleitem.cpp \
        scaler/handlerstrategies.cpp

HEADERS  += mainwindow.h \
    scaler/graphicsitemscaler.h \
    scaler/handleitem.h \
    scaler/handlerstrategies.h

FORMS    += mainwindow.ui

DISTFILES +=
