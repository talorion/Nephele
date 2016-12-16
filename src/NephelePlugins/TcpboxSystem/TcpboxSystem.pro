include(../../../common_cxxflags.pri)

TARGET = TcpboxSystem
TEMPLATE = lib
#CONFIG += staticlib
CONFIG += dll

DEFINES += TCPBOX_SYSTEM_LIBRARY

HEADERS += \
    Commands/AbstractEcmdCommand.hpp \
    Commands/EcmdHelp.hpp \
    Commands/EmcdVersion.hpp \
    EcmdConnection/EcmdConnection.hpp \
    EcmdConnection/EcmdConnectionManager.hpp \
    TcpboxClient.hpp \
    TcpboxFactory.hpp \
    TcpboxSystem.hpp \
    TcpboxSystemThread.hpp \
    tcpboxsystem_global.hpp

SOURCES += \
    Commands/AbstractEcmdCommand.cpp \
    Commands/EcmdHelp.cpp \
    Commands/EmcdVersion.cpp \
    EcmdConnection/EcmdConnection.cpp \
    EcmdConnection/EcmdConnectionManager.cpp \
    TcpboxClient.cpp \
    TcpboxFactory.cpp \
    TcpboxSystem.cpp \
    TcpboxSystemThread.cpp


## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../NepheleCore/release/ -lNepheleCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../NepheleCore/debug/ -lNepheleCore
else:unix:!macx: LIBS += -L$$OUT_PWD/../../NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../NepheleCore
DEPENDPATH += $$PWD/../../NepheleCore

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/release/libNepheleCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/debug/libNepheleCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/release/NepheleCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/debug/NepheleCore.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/libNepheleCore.a
## ================

