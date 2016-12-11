TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L/usr/bin/ -lsqlite3

SOURCES += main.cpp \
    networking.cpp \
    geemailserver.cpp \
    rsa_server.cpp

HEADERS += \
    networking.h \
    geemailserver.h \
    rsa_server.h

DISTFILES +=
