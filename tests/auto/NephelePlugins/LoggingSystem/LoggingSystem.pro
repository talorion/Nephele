#-------------------------------------------------
#
# Project created by QtCreator 2016-12-08T16:17:06
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_LoggingSystem
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    tst_LoggingSystem.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../../../common_cxxflags.pri)

## ================
unix:!macx: LIBS += -L$$OUT_PWD/../../../../src/NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../../../src/NepheleCore
DEPENDPATH += $$PWD/../../../../src/NepheleCore

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NepheleCore/libNepheleCore.a
## ================

#===========
unix:!macx: LIBS += -L$$OUT_PWD/../../../../src/NephelePlugins/LoggingSystem/ -lLoggingSystem

INCLUDEPATH += $$PWD/../../../../src/NephelePlugins/LoggingSystem
DEPENDPATH += $$PWD/../../../../src/NephelePlugins/LoggingSystem

#unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NephelePlugins/LoggingSystem/libLoggingSystem.a
#===========
