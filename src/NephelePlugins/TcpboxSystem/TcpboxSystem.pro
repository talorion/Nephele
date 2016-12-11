include(../../../common_cxxflags.pri)

TARGET = TcpboxSystem
TEMPLATE = lib
CONFIG += staticlib

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
    TcpboxSystemThread.hpp

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
unix:!macx: LIBS += -L$$OUT_PWD/../../NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../NepheleCore
DEPENDPATH += $$PWD/../../NepheleCore

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/libNepheleCore.a
## ================

