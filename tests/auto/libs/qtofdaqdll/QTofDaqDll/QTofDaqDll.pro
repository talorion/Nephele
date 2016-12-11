#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T13:17:21
#
#-------------------------------------------------

include(../../../../../common_cxxflags.pri)
QT       += testlib

TARGET = tst_QTofDaqDll
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_QTofDaqDll.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


#=================
unix:!macx: LIBS += -L$$OUT_PWD/../../../../../src/libs/qtofdaqdll/ -lqtofdaqdll

INCLUDEPATH += $$PWD/../../../../../src/libs/qtofdaqdll
DEPENDPATH += $$PWD/../../../../../src/libs/qtofdaqdll
#=================
