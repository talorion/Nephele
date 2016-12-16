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
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../src/NepheleCore/release/ -lNepheleCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../src/NepheleCore/debug/ -lNepheleCore
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../../src/NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../../../src/NepheleCore
DEPENDPATH += $$PWD/../../../../src/NepheleCore

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NepheleCore/release/libNepheleCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NepheleCore/debug/libNepheleCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NepheleCore/release/NepheleCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NepheleCore/debug/NepheleCore.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../src/NepheleCore/libNepheleCore.a
## ================
