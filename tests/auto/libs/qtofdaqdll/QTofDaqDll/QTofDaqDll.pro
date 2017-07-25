#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T13:17:21
#
#-------------------------------------------------

#include(../../../../../common_cxxflags.pri)
#include(../../../../../common_flags.pri)
QT       += testlib

TARGET = tst_QTofDaqDll
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += tst_QTofDaqDll.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


#=================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../../src/libs/qtofdaqdll/release/ -lqtofdaqdll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../../src/libs/qtofdaqdll/debug/ -lqtofdaqdll
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../../../src/libs/qtofdaqdll/ -lqtofdaqdll

INCLUDEPATH += $$PWD/../../../../../src/libs/qtofdaqdll
DEPENDPATH += $$PWD/../../../../../src/libs/qtofdaqdll

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/libs/qtofdaqdll/release/qtofdaqdll.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/libs/qtofdaqdll/debug/qtofdaqdll.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/libs/qtofdaqdll/libqtofdaqdll.a
#=================

#===================
INCLUDEPATH += $$PWD/../../../../../ext/TofDaq/include
DEPENDPATH += $$PWD/../../../../../ext/TofDaq/include

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../../../../../ext/TofDaq/include
}
#===================

