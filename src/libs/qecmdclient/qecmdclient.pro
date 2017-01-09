#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T18:49:05
#
#-------------------------------------------------

include(../../../common_cxxflags.pri)

TARGET = qecmdclient
TEMPLATE = lib
CONFIG += dll

DEFINES += QECMDCLIENT_LIBRARY

SOURCES += QEcmdClient.cpp \
    QEcmdCommand/AbstractQEcmdCommand.cpp \
    QEcmdParser/AbstractQEcmdParser.cpp

HEADERS += QEcmdClient.hpp\
        qecmdclient_global.hpp \
    QEcmdCommand/AbstractQEcmdCommand.hpp \
    QEcmdParser/AbstractQEcmdParser.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
