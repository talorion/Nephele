#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T08:16:18
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_ManagerLocator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_ManagerLocator.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../../../common_cxxflags.pri)

## ================
unix:!macx: LIBS += -L$$OUT_PWD/../../../../src/NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../../../src/NepheleCore
DEPENDPATH += $$PWD/../../../../src/NepheleCore

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NepheleCore/libNepheleCore.a
## ================
