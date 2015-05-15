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
    src/orderadmin.cpp \
    src/dbif.cpp \
    src/log.cpp \
    main.cpp

HEADERS += \
    src/orderadmin.h \
    src/guinf.h \
    src/log.h \
    src/drink.h \
    src/dbif.h
