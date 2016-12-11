#-------------------------------------------------
#
# Project created by QtCreator 2016-12-08T15:33:20
#
#-------------------------------------------------

QT       += testlib
QT       += network
QT       -= gui

TARGET = tst_EcmdConnection
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../../../../../common_cxxflags.pri)


SOURCES += tst_EcmdConnection.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!macx: LIBS += -L$$OUT_PWD/../TcpBoxSimulator/ -lTcpBoxSimulator

INCLUDEPATH += $$PWD/../TcpBoxSimulator
DEPENDPATH += $$PWD/../TcpBoxSimulator

## ================
unix:!macx: LIBS += -L$$OUT_PWD/../../../../../src/NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../../../../src/NepheleCore
DEPENDPATH += $$PWD/../../../../../src/NepheleCore

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NepheleCore/libNepheleCore.a
## ================

## ================
unix:!macx: LIBS += -L$$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/ -lTcpboxSystem

INCLUDEPATH += $$PWD/../../../../../src/NephelePlugins/TcpboxSystem
DEPENDPATH += $$PWD/../../../../../src/NephelePlugins/TcpboxSystem

#unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/libTcpboxSystem.a
## ================
