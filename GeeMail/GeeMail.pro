#-------------------------------------------------
#
# Project created by QtCreator 2016-11-08T20:52:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeeMail
TEMPLATE = app


SOURCES += main.cpp\
    gee_login.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    gee_main.cpp \
    networking.cpp \
    composewindow.cpp

HEADERS  += \
    gee_login.h \
    loginwindow.h \
    mainwindow.h \
    gee_main.h \
    networking.h \
    composewindow.h

FORMS    += \
    loginwindow.ui \
    mainwindow.ui \
    composewindow.ui
