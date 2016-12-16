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
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../../src/libs/qtofdaqdll/release/ -lqtofdaqdll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../../src/libs/qtofdaqdll/debug/ -lqtofdaqdll
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../../../src/libs/qtofdaqdll/ -lqtofdaqdll

INCLUDEPATH += $$PWD/../../../../../src/libs/qtofdaqdll
DEPENDPATH += $$PWD/../../../../../src/libs/qtofdaqdll

#win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/libs/qtofdaqdll/release/qtofdaqdll.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/libs/qtofdaqdll/debug/qtofdaqdll.lib
#=================


#===================
INCLUDEPATH += $$PWD/../../../../../src/libs/TofDaq/TofDaq_1.98_API/include
DEPENDPATH += $$PWD/../../../../../src/libs/TofDaq/TofDaq_1.98_API/include

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../../../../../src/libs/TofDaq/TofDaq_1.98_API/include
}
#===================

