#-------------------------------------------------
#
# Project created by QtCreator 2015-05-17T22:45:41
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = DrinkController
CONFIG   += console
CONFIG   -= app_bundle
LIBS    += -lboost_system
LIBS    += -lboost_thread
LIBS    += -lsqlite3

TEMPLATE = app


SOURCES += main.cpp \
    src/server.cpp \
    src/orderadmin.cpp \
    src/log.cpp \
    src/dbif.cpp \
    src/admin.cpp

HEADERS += \
    src/server.h \
    src/orderadmin.h \
    src/log.h \
    src/guinf.h \
    src/drink.h \
    src/dbif.h \
    src/admin.h
