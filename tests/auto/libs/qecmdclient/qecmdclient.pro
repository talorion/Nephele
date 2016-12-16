#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T18:50:05
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_QEcmdClient
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_QEcmdClient.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!macx: LIBS += -L$$OUT_PWD/../../../../src/libs/qecmdclient/ -lqecmdclient

INCLUDEPATH += $$PWD/../../../../src/libs/qecmdclient
DEPENDPATH += $$PWD/../../../../src/libs/qecmdclient
