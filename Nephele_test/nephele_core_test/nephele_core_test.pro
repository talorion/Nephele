#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T19:29:04
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_nephele_core_testtest
CONFIG   += console
CONFIG   -= app_bundle

#CONFIG += testcase

TEMPLATE = app

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtCore
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtGui
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtScript
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtWidgets
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtTest
    QMAKE_CXXFLAGS += -isystem '$ORIGIN'/../../../libs/qcustomplot
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += -Wextra
    QMAKE_CXXFLAGS += -ansi
    QMAKE_CXXFLAGS += -pedantic
    QMAKE_CXXFLAGS += -Wshadow
    QMAKE_CXXFLAGS += -Weffc++
    QMAKE_CXXFLAGS += -Wstrict-aliasing
}

CONFIG += c++11


SOURCES += \
    entity_manager/entity_manager_test.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS +=



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Nephele/nephele_core/release/ -lnephele_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Nephele/nephele_core/debug/ -lnephele_core
else:unix: LIBS += -L$$OUT_PWD/../../Nephele/nephele_core/ -lnephele_core

INCLUDEPATH += $$PWD/../../Nephele/nephele_core
DEPENDPATH += $$PWD/../../Nephele/nephele_core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Nephele/nephele_core/release/libnephele_core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Nephele/nephele_core/debug/libnephele_core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Nephele/nephele_core/release/nephele_core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Nephele/nephele_core/debug/nephele_core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../Nephele/nephele_core/libnephele_core.a
