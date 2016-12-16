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

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../../src/NepheleCore/release/ -lNepheleCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../../src/NepheleCore/debug/ -lNepheleCore
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../../../src/NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../../../../src/NepheleCore
DEPENDPATH += $$PWD/../../../../../src/NepheleCore

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NepheleCore/release/libNepheleCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NepheleCore/debug/libNepheleCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NepheleCore/release/NepheleCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NepheleCore/debug/NepheleCore.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NepheleCore/libNepheleCore.a
## ================

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/release/ -lTcpboxSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/debug/ -lTcpboxSystem
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/ -lTcpboxSystem

INCLUDEPATH += $$PWD/../../../../../src/NephelePlugins/TcpboxSystem
DEPENDPATH += $$PWD/../../../../../src/NephelePlugins/TcpboxSystem

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/release/libTcpboxSystem.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/debug/libTcpboxSystem.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/release/TcpboxSystem.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/debug/TcpboxSystem.lib
#unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../../../../src/NephelePlugins/TcpboxSystem/libTcpboxSystem.a
## ================

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../TcpBoxSimulator/release/ -lTcpBoxSimulator
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../TcpBoxSimulator/debug/ -lTcpBoxSimulator
else:unix:!macx: LIBS += -L$$OUT_PWD/../TcpBoxSimulator/ -lTcpBoxSimulator

INCLUDEPATH += $$PWD/../TcpBoxSimulator
DEPENDPATH += $$PWD/../TcpBoxSimulator

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../TcpBoxSimulator/release/libTcpBoxSimulator.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../TcpBoxSimulator/debug/libTcpBoxSimulator.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../TcpBoxSimulator/release/TcpBoxSimulator.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../TcpBoxSimulator/debug/TcpBoxSimulator.lib
#unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../TcpBoxSimulator/libTcpBoxSimulator.a
## ================
