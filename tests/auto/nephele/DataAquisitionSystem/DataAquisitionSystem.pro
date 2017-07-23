#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T13:08:36
#
#-------------------------------------------------

QT       += testlib
QT       += core gui script network scripttools
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QT       += widgets

TARGET = tst_DataAquisitionSystemTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app



SOURCES += tst_DataAquisitionSystemTest.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += SRCDIR=\\\"$$PWD/\\\"

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../src/nepheleLib/release/ -lnepheleLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../src/nepheleLib/debug/ -lnepheleLibd
else:unix:!macx:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../src/nepheleLib/ -lnepheleLib
else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../src/nepheleLib/ -lnepheleLibd

INCLUDEPATH += $$PWD/../../../../src/nepheleLib
DEPENDPATH += $$PWD/../../../../src/nepheleLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/nepheleLib/release/libnepheleLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/nepheleLib/debug/libnepheleLibd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/nepheleLib/release/nepheleLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/nepheleLib/debug/nepheleLibd.lib
## ================

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/release/ -lqcustomplot2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/debug/ -lqcustomplotd2
else:unix:!macx:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplot
else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplotd

INCLUDEPATH += $$PWD/../../../../ext/qcustomplot
DEPENDPATH += $$PWD/../../../../ext/qcustomplot

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../../../../ext/qcustomplot
}
## ================

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../src/libs/qtofdaqdll/release/ -lqtofdaqdll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../src/libs/qtofdaqdll/debug/ -lqtofdaqdll

INCLUDEPATH += $$PWD/../../../../src/libs/qtofdaqdll
DEPENDPATH += $$PWD/../../../../src/libs/qtofdaqdll

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/libs/qtofdaqdll/release/libqtofdaqdll.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/libs/qtofdaqdll/debug/libqtofdaqdll.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/libs/qtofdaqdll/release/qtofdaqdll.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/libs/qtofdaqdll/debug/qtofdaqdll.lib
## ================
