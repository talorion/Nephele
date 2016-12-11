#-------------------------------------------------
#
# Project created by QtCreator 2016-12-08T14:32:37
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_SystemManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_SystemManager.cpp \
    SystemDummy.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


HEADERS += \
    SystemDummy.hpp

include(../../../../common_cxxflags.pri)


## ================
unix:!macx: LIBS += -L$$OUT_PWD/../../../../src/NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../../../src/NepheleCore
DEPENDPATH += $$PWD/../../../../src/NepheleCore

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NepheleCore/libNepheleCore.a
## ================
