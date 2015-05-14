#-------------------------------------------------
#
# Project created by QtCreator 2015-03-29T14:06:39
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = admin
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    src/admin.cpp \
    src/dbif.cpp \
    src/log.cpp \
    src/server.cpp

HEADERS += \
    dbif.h \
    drink.h \
    admin.h \
    guidummy.h \
    log.h \
    src/admin.h \
    src/dbif.h \
    src/drink.h \
    src/guidummy.h \
    src/log.h \
    src/server.h
