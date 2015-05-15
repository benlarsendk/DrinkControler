#-------------------------------------------------
#
# Project created by QtCreator 2015-05-13T20:53:03
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = orderAdmin
CONFIG   += console
CONFIG   -= app_bundle
LIBS    += -lboost_system
LIBS    += -lboost_thread

TEMPLATE = app


SOURCES += main.cpp \
    orderadmin.cpp \
    dbif.cpp \
    log.cpp

HEADERS += \
    orderadmin.h \
    guinf.h \
    log.h \
    drink.h \
    dbif.h
