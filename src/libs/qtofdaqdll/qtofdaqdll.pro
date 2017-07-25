#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T13:06:07
#
#-------------------------------------------------

include(../../common_cxxflags.pri)
include(../../common_flags.pri)

TARGET = qtofdaqdll
TEMPLATE = lib
CONFIG += staticlib

DEFINES += QTOFDAQDLL_LIBRARY

SOURCES += QTofDaqDll.cpp \
    QTofDaqRec.cpp \
    userdatasource.cpp

HEADERS += QTofDaqDll.hpp\
        qtofdaqdll_global.hpp \
    QTofDaqRec.hpp \
    userdatasource.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#===================
INCLUDEPATH += $$PWD/../../../ext/TofDaq/include
DEPENDPATH += $$PWD/../../../ext/TofDaq/include

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../../../ext/TofDaq/include
}
#===================
