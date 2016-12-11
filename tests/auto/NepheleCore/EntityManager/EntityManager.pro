#-------------------------------------------------
#
# Project created by QtCreator 2016-12-08T13:41:48
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_EntityManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_EntityManager.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../../../common_cxxflags.pri)

## ================
unix:!macx: LIBS += -L$$OUT_PWD/../../../../src/NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../../../src/NepheleCore
DEPENDPATH += $$PWD/../../../../src/NepheleCore

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NepheleCore/libNepheleCore.a
## ================

