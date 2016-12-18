#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T18:50:05
#
#-------------------------------------------------

include(../../../../common_cxxflags.pri)
QT       += testlib

TARGET = tst_QEcmdClient
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_QEcmdClient.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


#===================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../src/libs/qecmdclient/release/ -lqecmdclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../src/libs/qecmdclient/debug/ -lqecmdclient
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../../src/libs/qecmdclient/ -lqecmdclient

INCLUDEPATH += $$PWD/../../../../src/libs/qecmdclient
DEPENDPATH += $$PWD/../../../../src/libs/qecmdclient
#===================

#===================
unix:!macx: LIBS += -L$$OUT_PWD/../../NephelePlugins/TcpBoxSystem/TcpBoxSimulator/ -lTcpBoxSimulator

INCLUDEPATH += $$PWD/../../NephelePlugins/TcpBoxSystem/TcpBoxSimulator
DEPENDPATH += $$PWD/../../NephelePlugins/TcpBoxSystem/TcpBoxSimulator
#===================
