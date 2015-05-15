#-------------------------------------------------
#
# Project created by QtCreator 2015-05-14T15:57:13
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = clientCTRL
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += main.cpp \
    src/client.cpp \
    src/adminclient.cpp

HEADERS += \
    src/client.h \
    src/adminclient.h \
    src/drink.h \
    src/guinf.h
